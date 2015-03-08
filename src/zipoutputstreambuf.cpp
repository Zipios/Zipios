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
 * \brief Implementation of ZipOutputStreambuf.
 */

#include "zipios++/zipoutputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


/** \class ZipOutputStreambuf
 * \brief Handle the output buffer of a zip archive.
 *
 * The ZipOutputStreambuf class is a zip archive output
 * streambuf filter.
 */


/** \typedef CompressionLevel
 *
 * The compression level to be used to save an entry.
 *
 * If unchanged, use the DEFAULT_COMPRESSION value.
 *
 * It is possible to change the compression level
 * to NO_COMPRESSION or use the setMethod() with
 * STORED to avoid any compression (i.e. create
 * a zip file which awfully looks like a tarball).
 */


/** \brief Initialize a ZipOutputStreambuf object.
 *
 * Note that a new initialized ZipOutputStreambuf is not ready to
 * accept data, putNextEntry() must be invoked at least once first.
 *
 * \param outbuf  The streambuf to use for output.
 */
ZipOutputStreambuf::ZipOutputStreambuf(std::streambuf *outbuf)
    : DeflateOutputStreambuf(outbuf, false)
    //, m_open_entry(false) -- auto-init
    //, m_open(true) -- auto-init
    //, m_method(StorageMethod::DEFLATED) -- auto-init
    //, m_level(6) -- auto-init
{
}


/** \brief Clean up the buffer.
 *
 * This function cleans up this output buffer. In general this ensures
 * that the data still cached gets flushed.
 */
ZipOutputStreambuf::~ZipOutputStreambuf()
{
    finish();
}


/** \brief Close this buffer entry.
 *
 * Closes the current output buffer entry and positions the stream
 * write pointer at the beginning of the next entry.
 */
void ZipOutputStreambuf::closeEntry()
{
    if(!m_open_entry)
    {
        return;
    }

    closeStream();

    updateEntryHeaderInfo();
    setEntryClosedState();
}


/** \brief Close the output stream buffer.
 *
 * This function calls finish to make sure that any cached
 * data is saved and then close the stream buffer.
 */
void ZipOutputStreambuf::close()
{
    finish();
}


/** \brief Finish up an output stream buffer.
 *
 * Closes the current entry (if one is open), then writes the Zip
 * Central Directory Structure closing the ZipOutputStream. The
 * output stream (std::ostream) that the zip archive is being
 * written to is not closed.
 */
void ZipOutputStreambuf::finish()
{
  if(!m_open)
  {
    return;
  }

  closeEntry();
  std::ostream os(m_outbuf);
  writeCentralDirectory(m_entries, EndOfCentralDirectory(m_zip_comment), os);
  m_open = false;
}


/** \brief Start saving an entry in the output buffer.
 *
 * Opens the next entry in the zip archive and returns a const pointer to a
 * FileEntry object for the entry.
 *
 * If a previous entry was still open, the function calls closeEntry()
 * first.
 *
 * \param[in] entry  The entry to be saved and made current.
 */
void ZipOutputStreambuf::putNextEntry(ZipCDirEntry const& entry)
{
    closeEntry();

    if(!init(m_level))
    {
        std::cerr << "ZipOutputStreambuf::putNextEntry(): init() failed!\n";
    }

    m_entries.push_back(entry);
    ZipCDirEntry& ent(m_entries.back());

    std::ostream os(m_outbuf);

    // Update entry header info
    ent.setLocalHeaderOffset(os.tellp());
    ent.setMethod(m_method);

    os << static_cast<ZipLocalEntry>(ent);

    m_open_entry = true;
}


/** \brief Set the archive comment.
 *
 * This function saves a global comment for the Zip archive.
 *
 * You may set it to an empty string which means that no comment
 * will be saved.
 *
 * The comment is saved when the first entry is saved so it
 * has to be put in there early on.
 */
void ZipOutputStreambuf::setComment(std::string const& comment)
{
    m_zip_comment = comment;
}


/** \brief Change the compression level.
 *
 * This function sets the compression level to be used by subsequent
 * entries. It has to be called before the putNextEntry() function.
 *
 * \param[in] level  A zlib compression level (0 to 9).
 */
void ZipOutputStreambuf::setLevel(CompressionLevel level)
{
    if(level < 0 || level > 9)
    {
        throw InvalidException("Invalid level used with ZipOutputStreambuf::setLevel(). A value from 1 to 9 only is allowed.");
    }

    m_level = level;
}


/** \brief Change the compression method.
 *
 * This function sets the compression method to be used.
 *
 * Only STORED (uncompressed) and DEFLATED (compressed with zlib)
 * are supported in this version.
 *
 * This function calls setLevel() with the correct compression
 * level. It has to be called before putNextEntry() to be effective
 * with the following entry.
 *
 * \note
 * If you set the method to DEFLATED and the compression level is
 * not NO_COMPRESSION, then the compression level is not changed.
 *
 * \param[in] method  The compression method to be used next.
 */
void ZipOutputStreambuf::setMethod(StorageMethod method)
{
    m_method = method;
    if(method == StorageMethod::STORED)
    {
        setLevel(NO_COMPRESSION);
    }
    else if(method == StorageMethod::DEFLATED)
    {
        // if the user already defined a compression level, leave it as is
        if(m_level == NO_COMPRESSION)
        {
            setLevel(DEFAULT_COMPRESSION);
        }
    }
    else
    {
        throw FCollException("Specified compression method not supported");
    }
}

//
// Protected and private methods
//

int ZipOutputStreambuf::overflow(int c)
{
    return DeflateOutputStreambuf::overflow(c);

    // FIXME: implement
    //cout << "ZipOutputStreambuf::overflow() not implemented yet!\n";
    //return EOF ;
}



int ZipOutputStreambuf::sync()
{
    return DeflateOutputStreambuf::sync();

    // FIXME: implement
    //cout << "ZipOutputStreambuf::sync() not implemented yet!\n";
    //return EOF ;
}



void ZipOutputStreambuf::setEntryClosedState()
{
    m_open_entry = false ;

    // FIXME: update put pointers to trigger overflow on write. overflow
    //        should then return EOF while _open_entry is false.
}


void ZipOutputStreambuf::updateEntryHeaderInfo()
{
    if(!m_open_entry)
    {
        return;
    }

    std::ostream os(m_outbuf);
    int const curr_pos(os.tellp());

    // update fields in m_entries.back()
    ZipCDirEntry& entry(m_entries.back());
    entry.setSize(getCount());
    entry.setCrc(getCrc32());
    entry.setCompressedSize(curr_pos - entry.getLocalHeaderOffset() - entry.getLocalHeaderSize());
    entry.setUnixTime(std::time(nullptr));

    // write ZipLocalEntry header to header position
    os.seekp(entry.getLocalHeaderOffset());
    os << static_cast<ZipLocalEntry>(entry);
    os.seekp(curr_pos);
}


void ZipOutputStreambuf::writeCentralDirectory(std::vector<ZipCDirEntry> const& entries,
                                               EndOfCentralDirectory eocd,
                                               std::ostream &os)
{
    int cdir_start(os.tellp());
    int cdir_size(0);

    for(auto it = entries.begin(); it != entries.end(); ++it)
    {
        os << *it;
        cdir_size += it->getCDirHeaderSize();
    }
    eocd.setOffset(cdir_start);
    eocd.setCDirSize(cdir_size);
    eocd.setTotalCount(entries.size());
    eocd.write(os);
}


} // zipios namespace
// vim: ts=4 sw=4 et
