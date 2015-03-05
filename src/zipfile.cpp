/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
 * \brief The implementation of ZipFile.
 */


#include "zipios++/zipfile.h"

#include "zipios++/zipiosexceptions.h"

#include "zipios++/zipinputstream.h"


#include "backbuffer.h"

namespace zipios
{

//
// Public
//

/** \anchor zipfile_openembeddedzipfile
 * \brief Open a zip archive that was previously appened to another file.
 *
 * Opens a Zip archive embedded in another file, by writing the zip
 * archive to the end of the file followed by the start offset of
 * the zip file on 4 bytes. The offset must be written in zip-file
 * byte-order (little endian).
 *
 * The program appendzip, which is part of the Zipios++ distribution can
 * be used to append a Zip archive to a file, e.g. a binary program.
 *
 * The function may throw various exception if the named file does not
 * seem to include a valid zip archive attached.
 *
 * \note
 * Only one file can be appended and opened in this way. Although
 * the appendzip tool can be used to append any number of files,
 * only the last one is accessible.
 *
 * \return A ZipFile that one can use to read compressed data.
 */
ZipFile::pointer_t ZipFile::openEmbeddedZipFile(std::string const& name)
{
    // open zipfile, read 4 last bytes close file
    // create ZipFile object.
    std::ifstream ifs(name.c_str(), std::ios::in | std::ios::binary);
    ifs.seekg(-4, std::ios::end);
    uint32_t const start_offset(readUint32(ifs));
    ifs.close();
    return ZipFile::pointer_t(new ZipFile(name, start_offset, 4));
}


/** \brief Initialize a ZipFile object.
 *
 * This is the default constructor of the ZipFile object.
 */
ZipFile::ZipFile()
    //: m_vs(...) -- auto-init
    //, m_eocd(...) -- auto-init
{
}


/** \brief Initialize a ZipFile object from an input file.
 *
 * This constructor opens the named zip file. If the zip "file" is
 * embedded in a file that contains other data, e.g. a binary
 * program, the offset of the zip file start and end must be
 * specified.
 *
 * If the file cannot be opened or the Zip directory cannot
 * be read, then the constructor throws an exception.
 *
 * \param[in] filename  The filename of the zip file to open.
 * \param[in] s_off  Offset relative to the start of the file, that
 *                   indicates the beginning of the zip data in the file.
 * \param[in] e_off  Offset relative to the end of the file, that
 *                   indicates the end of the zip data in the file.
 *                   The offset is a positive number, even though the
 *                   offset is towards the beginning of the file.
 */
ZipFile::ZipFile(std::string const& filename, int s_off, int e_off)
    : m_vs(s_off, e_off)
{
    m_filename = filename;
    std::ifstream zipfile(m_filename.c_str(), std::ios::in | std::ios::binary);
    init(zipfile);
}


FileCollection::pointer_t ZipFile::clone() const
{
    return FileCollection::pointer_t(new ZipFile(*this));
}


ZipFile::~ZipFile()
{
    close();
}


void ZipFile::close()
{
    m_valid = false ;
}


ZipFile::stream_pointer_t ZipFile::getInputStream(FileEntry::pointer_t entry)
{
    mustBeValid();
    return getInputStream(entry->getName());
}


ZipFile::stream_pointer_t ZipFile::getInputStream(std::string const& entry_name, MatchPath matchpath)
{
    mustBeValid();

    FileEntry::pointer_t ent(getEntry(entry_name, matchpath));
    if(!ent)
    {
        return 0;
    }

    stream_pointer_t zis(new ZipInputStream(m_filename, static_cast<ZipCDirEntry const *>(ent.get())->getLocalHeaderOffset() + m_vs.startOffset()));
    //
    // Wed Mar 19 18:16:34 PDT 2014 (RDB)
    // This was causing a basic_ios::clear exception.
    // Not sure why we would need to do this here!
    //zis->getNextEntry();
    return zis;
}


//
// Private
//

bool ZipFile::init(std::istream& zipfile)
{
    // Check stream error state
    if(!zipfile)
    {
        m_valid = false;
        // was returning false in previous versions...
        throw IOException("Error reading from file.");
    }

    m_valid = readCentralDirectory(zipfile);

    return m_valid;
}


bool ZipFile::readCentralDirectory(std::istream& zipfile)
{
    // Find and read eocd.
    if(!readEndOfCentralDirectory(zipfile))
    {
        throw FCollException("Unable to find zip structure: End-of-central-directory");
    }

    // Position read pointer to start of first entry in central dir.
    m_vs.vseekg(zipfile, m_eocd.offset(), std::ios::beg);

    int entry_num(0);
    // Giving the default argument in the next line to keep Visual C++ quiet
    m_entries.resize(m_eocd.totalCount(), 0);
    while(entry_num < m_eocd.totalCount())
    {
        ZipCDirEntry::pointer_t ent(new ZipCDirEntry);
        m_entries[entry_num] = ent;
        zipfile >> *static_cast<ZipCDirEntry *>(ent.get());
        if(!zipfile)
        {
            if(zipfile.bad())
            {
                throw IOException("Error reading zip file while reading zip file central directory");
            }
            else if(zipfile.fail())
            {
                throw FCollException("Zip file consistency problem. Failure while reading zip file central directory");
            }
            else if(zipfile.eof())
            {
                throw IOException("Premature end of file while reading zip file central directory");
            }
        }
        ++entry_num ;
    }

    // Consistency check. eocd should start here

    int const pos(m_vs.vtellg(zipfile));
    m_vs.vseekg(zipfile, 0, std::ios::end);
    int const remaining(static_cast< int >(m_vs.vtellg(zipfile)) - pos);
    if(remaining != m_eocd.eocdOffSetFromEnd())
    {
        throw FCollException("Zip file consistency problem. Zip file data fields are inconsistent with zip file layout");
    }

    // Consistency check 2, are local headers consistent with
    // cd headers
    if(!confirmLocalHeaders(zipfile))
    {
        throw FCollException("Zip file consistency problem. Zip file data fields are inconsistent with zip file layout");
    }

    return true;
}


bool ZipFile::readEndOfCentralDirectory(std::istream& zipfile)
{
    BackBuffer bb(zipfile, m_vs);
    int read_p(-1);
    for(;;)
    {
        if(read_p < 0)
        {
            if(!bb.readChunk(read_p))
            {
                return false;
            }
        }
        if(m_eocd.read(bb, read_p))
        {
            return true;
        }
        --read_p;
    }
}


bool ZipFile::confirmLocalHeaders(std::istream& zipfile)
{
    int inconsistencies(0);
    ZipLocalEntry zlh ;
    for(auto it = m_entries.begin(); it != m_entries.end(); it++)
    {
        ZipCDirEntry *ent(static_cast<ZipCDirEntry *>((*it).get()));
        m_vs.vseekg(zipfile, ent->getLocalHeaderOffset(), std::ios::beg);
        zipfile >> zlh;
        if(!zipfile || zlh != *ent)
        {
            inconsistencies++;
            zipfile.clear();
        }
    }

    return !inconsistencies;
}



} // zipios namespace

// vim: ts=4 sw=4 et
