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

/** \mainpage Zipios++
 *
 * \image html   zipios++.jpg
 *
 * \section intro Introduction
 *
 * Zipios++ is a java.util.zip-like C++ library for reading and
 * writing Zip files. Access to individual entries is provided through
 * standard C++ iostreams. A simple read-only virtual file system that
 * mounts regular directories and zip files is also provided.
 *
 * The source code is released under the <a
 * href="http://www.gnu.org/copyleft/lesser.html">GNU Lesser General Public
 * License</a>.
 *
 * \section status Status
 *
 * This was the status of version 1.x. At this point, 2.x is being worked on.
 *
 * Spanned archives are not supported, and support is not planned.
 *
 * The library v1.x has been tested and appears to be working with
 *
 * <ul>
 * <li><a href="http://www.freebsd.org/ports/archivers.html#zipios++-0.1.5">FreeBSD stable and current / gcc 2.95.3</a></li>
 * <li>Red Hat Linux release 7.0  / gcc 2.96</li>
 * <li>Red Hat Linux release 6.2 (Zoot) / egcs-2.91.66</li>
 * <li>Linux Mandrake release 7.0 (Air) / gcc 2.95.2</li>
 * <li>SGI IRIX64 6.5 / gcc 2.95.2</li>
 * <li>SGI IRIX64 6.5 / MIPSpro Compilers: Version 7.30</li>
 * </ul>
 *
 * If you make zipios++ work on other platforms, let us know by posting
 * a message on Sourceforge.net
 *
 *   http://sourceforge.net/projects/zipios/
 *
 * \section documentation Documentation
 *
 * This web page is the front page to the library documentation which
 * is generated from the source files using <a
 * href="http://www.stack.nl/~dimitri/doxygen/index.html">Doxygen</a>.
 * Use the links at the top of the page to browse the API
 * documentation. Your Doxygen installation may also work be capable
 * of generating other formats (Latex, PDF, etc.)
 *
 * \subsection zipfiles Zip file access
 *
 * The two most important classes are ZipFile and ZipInputStream.'
 * ZipInputStream is an istream for reading zipfiles. It can be
 * instantiated directly, without the use of ZipFile.
 *
 * ZipInputStream::getNextEntry() positions a new ZipInputStream on
 * the first entry. The following entry can be accessed by calling
 * ZipInputStream::getNextEntry() again.
 *
 * ZipFile scans the central directory of a zipfile and provides an
 * interface to access that directory. The user may search for entries
 * with a particular filename using \ref fcoll_getentry_anchor "ZipFile::getEntry()",
 * or simply get the complete list of entries
 * with \ref fcoll_entries_anchor "ZipFile::entries()". To get an
 * istream (ZipInputStream) to a particular entry simply use
 * \ref fcoll_getinputstream "ZipFile::getInputStream()".
 *
 * \ref example_zip_anchor "example_zip.cpp" demonstrates the central
 * elements of Zipios++.
 *
 * A Zip file appended to another file, e.g. a binary program, with the program
 * \ref appendzip_anchor "appendzip", can be read with
 * \ref zipfile_openembeddedzipfile "ZipFile::openEmbeddedZipFile()".
 *
 * \subsection filecollection FileCollection
 *
 * A ZipFile is actually just a special kind of
 * \ref fcoll_anchor "FileCollection" that
 * obtains its entries from a .zip Zip archive. Zipios++ also implements
 * a \ref dircol_anchor "DirectoryCollection" that obtains its entries
 * from a specified directory, and a \ref collcoll_anchor "CollectionCollection"
 * that obtains its entries from
 * other collections. Using a single CollectionCollection any number of
 * other FileCollections can be placed under its control and accessed
 * through the same single interface that is used to access a ZipFile or
 * a DirectoryCollection.
 *
 * \section download Download
 *
 * Go to Zipios++ project page on SourceForge for tar balls, source code
 * (CVS for v1.x and GIT for v2.x), and ChangeLog.
 * <a href="https://sourceforge.net/projects/zipios/" >
 * https://sourceforge.net/projects/zipios/</a>
 *
 * \section links Links
 *
 * <a href="http://www.zlib.net/">zlib</a>.
 * The compression library that Zipios++ uses to perform the actual
 * compression and decompression.
 *
 * <a href="http://www.oracle.com/technetwork/java/index.html">
 * Java</a>. Zipios++ version 1.0 is heavily inspired by the
 * java.util.zip package. Version 2.0 is following the same
 * philosophy without (1) attempting to follow the interface one
 * to one and (2) without updating to the newer version, if there
 * were changes...
 *
 * You will find a text file in the doc directory named zip-format.txt
 * with a complete description of the zip file format.
 *
 * \section bugs Bugs
 *
 * Submit bug reports and patches on:
 *
 *   http://sourceforge.net/projects/zipios/
 *
 * Project hosted by <a href="http://sourceforge.net">
 * <img src="http://sourceforge.net/sflogo.php?group_id=5418&type=1"/>
 * </a>
 */


#include "zipios++/zipfile.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "backbuffer.hpp"
#include "endofcentraldirectory.hpp"
#include "zipcdirentry.hpp"
#include "zipinputstream.hpp"
#include "zipios_common.hpp"

#include <fstream>


namespace zipios
{


/** \class ZipFile
 * \brief The ZipFile class represents a collection of files.
 * \anchor zipfile_anchor
 *
 * ZipFile is a FileCollection, where the files are stored
 * in a .zip file.
 */



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
    uint32_t start_offset;
    zipRead(ifs, start_offset);
    ifs.close();
    return ZipFile::pointer_t(new ZipFile(name, start_offset, 4));
}


/** \brief Initialize a ZipFile object.
 *
 * This is the default constructor of the ZipFile object.
 *
 * Note that an empty ZipFile is marked as invalid. More or less, such
 * an object is useless although it is useful if you work with maps or
 * vectors of ZipFile objects.
 */
ZipFile::ZipFile()
    //: m_vs(...) -- auto-init
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
ZipFile::ZipFile(std::string const& filename, offset_t s_off, offset_t e_off)
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


ZipFile::stream_pointer_t ZipFile::getInputStream(std::string const& entry_name, MatchPath matchpath)
{
    mustBeValid();

    FileEntry::pointer_t ent(getEntry(entry_name, matchpath));
    if(!ent)
    {
        return 0;
    }

    // TODO: make sure the entry offset is properly defined by ZipCDirEntry
    //static_cast<ZipCDirEntry const *>(ent.get())->getLocalHeaderOffset() + m_vs.startOffset()
    stream_pointer_t zis(new ZipInputStream(m_filename, m_entry_offset + m_vs.startOffset()));
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
    EndOfCentralDirectory eocd;

    // Find and read eocd.
    BackBuffer bb(zipfile, m_vs);
    ssize_t read_p(-1);
    for(;;)
    {
        if(read_p < 0)
        {
            if(!bb.readChunk(read_p))
            {
                throw FCollException("Unable to find zip structure: End-of-central-directory");
            }
        }
        if(eocd.read(bb, read_p))
        {
            break;
        }
        --read_p;
    }

    // Position read pointer to start of first entry in central dir.
    m_vs.vseekg(zipfile, eocd.offset(), std::ios::beg);

    int entry_num(0);
    // Giving the default argument in the next line to keep Visual C++ quiet
    m_entries.resize(eocd.totalCount(), 0);
    while(entry_num < eocd.totalCount())
    {
        ZipCDirEntry::pointer_t ent(new ZipCDirEntry);
        m_entries[entry_num] = ent;
        static_cast<ZipCDirEntry *>(ent.get())->read(zipfile);
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
    if(remaining != eocd.eocdOffSetFromEnd())
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


bool ZipFile::confirmLocalHeaders(std::istream& zipfile)
{
    int inconsistencies(0);
    ZipLocalEntry zlh ;
    for(auto it = m_entries.begin(); it != m_entries.end(); it++)
    {
        ZipCDirEntry *ent(static_cast<ZipCDirEntry *>((*it).get()));
        m_vs.vseekg(zipfile, ent->getLocalHeaderOffset(), std::ios::beg);
        zlh.read(zipfile);
        if(!zipfile || !zlh.isEqual(*ent))
        {
            inconsistencies++;
            zipfile.clear();
        }
    }

    return !inconsistencies;
}



} // zipios namespace
// vim: ts=4 sw=4 et
