/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015  Made to Order Software Corporation

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
 * \brief Implementation of the zipios::ZipOutputStreambuf class.
 *
 * This file includes the functions necessary to write data to a Zip
 * archive.
 */

#include "zipoutputstreambuf.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include "ziplocalentry.hpp"
#include "zipendofcentraldirectory.hpp"


namespace zipios
{


namespace
{

/** \brief Help function used to write the central directory.
 *
 * When you create a Zip archive, it includes a central directory where
 * all the meta data about each file is saved. This function saves an
 * array of entries in an output stream to generate the Zip file
 * central directory.
 *
 * \param[in] os  The output stream.
 * \param[in] entries  The array of entries to save in this central directory.
 * \param[in] comment  The zip archive global comment.
 */
void writeZipCentralDirectory(std::ostream &os, FileEntry::vector_t& entries, std::string const& comment)
{
    ZipEndOfCentralDirectory eocd(comment);
    eocd.setOffset(os.tellp());  // start position
    eocd.setCount(entries.size());

    size_t central_directory_size(0);
    for(auto it = entries.begin(); it != entries.end(); ++it)
    {
        (*it)->write(os);
        central_directory_size += (*it)->getHeaderSize();
    }

    eocd.setCentralDirectorySize(central_directory_size);
    eocd.write(os);
}


} // no name namespace


/** \class ZipOutputStreambuf
 * \brief Handle the output buffer of a zip archive.
 *
 * The ZipOutputStreambuf class is a zip archive output
 * streambuf filter.
 */


/** \typedef int ZipOutputStreambuf::CompressionLevel
 * \brief The compression level to be used to save an entry.
 *
 * Values defined using this time represent the compression level to
 * be used when compressing an entry.
 *
 * If unchanged, use the DEFAULT_COMPRESSION value.
 *
 * It is possible to change the compression level
 * to NO_COMPRESSION or use the setMethod() with
 * STORED to avoid any compression (i.e. create
 * a zip file which awfully looks like a tarball).
 *
 * \todo
 * These values are one to one mapped to zlib compression values. This
 * is likely to change once we start offering other compression scheme
 * for a number defined between 0 and 100 instead.
 */


/** \brief Initialize a ZipOutputStreambuf object.
 *
 * Note that a new initialized ZipOutputStreambuf is not ready to
 * accept data, putNextEntry() must be invoked at least once first.
 *
 * \param outbuf  The streambuf to use for output.
 */
ZipOutputStreambuf::ZipOutputStreambuf(std::streambuf *outbuf)
    : DeflateOutputStreambuf(outbuf)
    //, m_open_entry(false) -- auto-init
    //, m_open(true) -- auto-init
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
    writeZipCentralDirectory(os, m_entries, m_zip_comment);
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
void ZipOutputStreambuf::putNextEntry(FileEntry::pointer_t entry)
{
    closeEntry();

    // if the method is STORED force uncompressed data
    init(entry->getMethod() == StorageMethod::STORED ? FileEntry::COMPRESSION_LEVEL_NONE : entry->getLevel());

    m_entries.push_back(entry);

    std::ostream os(m_outbuf);

    // Update entry header info
    entry->setEntryOffset(os.tellp());
    /** \TODO
     * Rethink the design as we have to force a call to the correct
     * write() function?
     */
    static_cast<ZipLocalEntry *>(entry.get())->ZipLocalEntry::write(os);

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
 *
 * \param[in] comment  The comment to save in the Zip archive.
 */
void ZipOutputStreambuf::setComment(std::string const& comment)
{
    m_zip_comment = comment;
}


//
// Protected and private methods
//

int ZipOutputStreambuf::overflow(int c)
{
    return DeflateOutputStreambuf::overflow(c);
}



int ZipOutputStreambuf::sync()
{
    return DeflateOutputStreambuf::sync();
}



void ZipOutputStreambuf::setEntryClosedState()
{
    m_open_entry = false;

    /** \FIXME
     * Update put pointers to trigger overflow on write. Overflow
     * should then return EOF while m_open_entry is false.
     */
}


/** \brief Save the header information.
 *
 * This function saves parameters that are now available in the header
 * of the local entry.
 *
 * These parameters include:
 *
 * \li The uncompressed size of the entry
 * \li The compressed size of the entry
 * \li The CRC32 of the input file (before the compression)
 */
void ZipOutputStreambuf::updateEntryHeaderInfo()
{
    if(!m_open_entry)
    {
        return;
    }

    std::ostream os(m_outbuf);
    int const curr_pos(os.tellp());

    // update fields in m_entries.back()
    FileEntry::pointer_t entry(m_entries.back());
    entry->setSize(getSize());
    entry->setCrc(getCrc32());
    /** \TODO
     * Rethink the design as we have to force a call to the correct
     * getHeaderSize() function?
     */
    entry->setCompressedSize(curr_pos - entry->getEntryOffset() - static_cast<ZipLocalEntry *>(entry.get())->ZipLocalEntry::getHeaderSize());

    // write ZipLocalEntry header to header position
    os.seekp(entry->getEntryOffset());
    /** \TODO
     * Rethink the design as we have to force a call to the correct write()
     * function?
     */
    static_cast<ZipLocalEntry *>(entry.get())->ZipLocalEntry::write(os);
    os.seekp(curr_pos);
}


} // zipios namespace
// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
