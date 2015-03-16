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
 * \brief Declare zipios::EndOfCentralDirectory whichs handles entries found
 *        in a Zip archive directory.
 *
 * This header file contains the zipios::ZipLocalEntry, which is used
 * to handle entries found in a Zip archive.
 */

#include "endofcentraldirectory.hpp"

#include "zipios++/zipiosexceptions.hpp"


namespace zipios
{


/** \class EndOfCentralDirectory
 * \brief Marker at the end of a Zip archive.
 *
 * The end of the Central directory structure. This structure is
 * stored in the end of the zipfile, and contains information about
 * the zipfile, including the position of the start of the central
 * directory.
 */


namespace
{


// "PK 5.6" -- End of Central Directory
uint32_t const g_signature = 0x06054b50;


} // no name namespace


EndOfCentralDirectory::EndOfCentralDirectory(std::string const& zip_comment)
    //: m_disk_num(0) -- auto-init
    //, m_central_directory_disk_num(0) -- auto-init
    //, m_central_directory_entries(0) -- auto-init
    //, m_central_directory_size(0) -- auto-init
    //, m_central_directory_offset(0) -- auto-init
    : m_zip_comment(zip_comment)
{
}

size_t EndOfCentralDirectory::getOffset() const
{
    return m_central_directory_offset;
}


size_t EndOfCentralDirectory::getCount() const
{
    return m_central_directory_entries;
}


void EndOfCentralDirectory::setCentralDirectorySize(size_t size)
{
    m_central_directory_size = size;
}


void EndOfCentralDirectory::setOffset(offset_t start_offset)
{
    m_central_directory_offset = start_offset;
}


void EndOfCentralDirectory::setCount(size_t count)
{
    m_central_directory_entries = count;
}


std::streampos EndOfCentralDirectory::getOffsetFromEnd() const
{
    return m_eocd_offset_from_end;
}


bool EndOfCentralDirectory::read(::zipios::buffer_t const& buf, size_t pos)
{
    // the number of bytes we are going to read in the buffer
    // (including the signature)
    ssize_t const HEADER_SIZE(static_cast<ssize_t>(sizeof(uint32_t) * 3 + sizeof(uint16_t) * 5));

    m_eocd_offset_from_end = buf.size() - pos;

    // enough data in the buffer?
    //
    // Note: this quick check assumes a 0 length comment which is possible;
    //       if there is a comment and we find the signature too early, then
    //       it will throw
    //
    if(static_cast<ssize_t>(buf.size() - pos) < HEADER_SIZE)
    {
        return false;
    }

    // first read and check the signature
    uint32_t signature;
    zipRead(buf, pos, signature);               // 32
    if(signature != g_signature)
    {
        return false;
    }

    // next we read the other parameters
    uint16_t disk_number;
    uint16_t central_directory_entries;
    uint16_t central_directory_total_entries;
    uint32_t central_directory_size;
    uint32_t central_directory_offset;
    uint16_t comment_len;

    zipRead(buf, pos, disk_number                       );  // 16
    zipRead(buf, pos, disk_number                       );  // 16
    zipRead(buf, pos, central_directory_entries         );  // 16
    zipRead(buf, pos, central_directory_total_entries   );  // 16
    zipRead(buf, pos, central_directory_size            );  // 32
    zipRead(buf, pos, central_directory_offset          );  // 32
    zipRead(buf, pos, comment_len                       );  // 16
    zipRead(buf, pos, m_zip_comment, comment_len        );  // string

    // note that if disk_number is defined, then these following two
    // numbers should differ too
    if(central_directory_entries != central_directory_total_entries)
    {
        throw FileCollectionException("EndOfCentralDirectory with a number of entries and total entries that differ is not supported, spanned zip files are not supported");
    }

    m_central_directory_entries = central_directory_entries;
    m_central_directory_size    = central_directory_size;
    m_central_directory_offset  = central_directory_offset;

    return true;
}


/** \brief Write the End of Central Directory to a stream.
 *
 * This function writes the currently defined end of central
 * directory to disk. This entry is expected to be written at
 * the very end of a Zip archive file.
 *
 * \note
 * The function does not change the put pointer of the stream
 * before writing to it.
 *
 * \note
 * If the output stream is not currently valid, the function
 * does nothing and returns immediately.
 *
 * \param[in] os  The output stream where the data is to be saved.
 */
void EndOfCentralDirectory::write(std::ostream& os)
{
    if(os)
    {
        /** \TODO
         * Add support for 64 bit Zip archive. This would allow for pretty
         * much all the following conditions to be dropped out.
         */
        if(m_zip_comment.length() > 65535)
        {
            throw IOException("the Zip archive comment is too large");
        }
        if(m_central_directory_entries > 65535)
        {
            throw IOException("the number of entries in the Zip archive is too large");
        }
// Solaris defines _ILP32 for 32 bit platforms
#if !defined(_ILP32)
        if(m_central_directory_size   >= 0x100000000UL
        || m_central_directory_offset >= 0x100000000L)
        {
            throw IOException("the Zip archive size or offset are too large");
        }
#endif

        uint16_t const disk_number(0);
        uint16_t const central_directory_entries(m_central_directory_entries);
        uint32_t const central_directory_size(m_central_directory_size);
        uint32_t const central_directory_offset(m_central_directory_offset);
        uint16_t const comment_len(m_zip_comment.length());

        // the total number of entries, across all disks is the same in our
        // case so we use one number for both fields

        zipWrite(os, g_signature                  );    // 32
        zipWrite(os, disk_number                  );    // 16
        zipWrite(os, disk_number                  );    // 16
        zipWrite(os, central_directory_entries    );    // 16
        zipWrite(os, central_directory_entries    );    // 16
        zipWrite(os, central_directory_size       );    // 32
        zipWrite(os, central_directory_offset     );    // 32
        zipWrite(os, comment_len                  );    // 16
        zipWrite(os, m_zip_comment                );    // string
    }
}


} // zipios namespace
// vim: ts=4 sw=4 et
