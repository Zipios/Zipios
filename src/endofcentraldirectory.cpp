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


// "PK 5.6"
uint32_t const g_signature = 0x06054b50;


} // no name namespace


EndOfCentralDirectory::EndOfCentralDirectory(std::string const& zip_comment)
    //: m_disk_num(0) -- auto-init
    //, m_cdir_disk_num(0) -- auto-init
    //, m_cdir_entries(0) -- auto-init
    //, m_cdir_tot_entries(0) -- auto-init
    //, m_cdir_size(0) -- auto-init
    //, m_cdir_offset(0) -- auto-init
    : m_zip_comment(zip_comment)
{
}

uint32_t EndOfCentralDirectory::offset() const
{
    return m_cdir_offset;
}


uint16_t EndOfCentralDirectory::totalCount() const
{
    return m_cdir_total_entries;
}


void EndOfCentralDirectory::setCDirSize(uint32_t size)
{
    m_cdir_size = size;
}


void EndOfCentralDirectory::setOffset(uint32_t new_offset)
{
    m_cdir_offset = new_offset;
}


void EndOfCentralDirectory::setTotalCount(uint16_t c)
{
    m_cdir_entries = c;
    m_cdir_total_entries = c;
}


std::streampos EndOfCentralDirectory::eocdOffSetFromEnd() const
{
    return m_eocd_offset_from_end;
}


bool EndOfCentralDirectory::read(::zipios::buffer_t const& buf, size_t pos)
{
    // the number of bytes we are going to read in the buffer
    // (including the signature)
    ssize_t const size(static_cast<ssize_t>(sizeof(uint32_t) * 3 + sizeof(uint16_t) * 5));

    m_eocd_offset_from_end = buf.size() - pos;

    // enough data?
    if(static_cast<ssize_t>(buf.size() - pos) < size)
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
    zipRead(buf, pos, m_disk_num);              // 16
    zipRead(buf, pos, m_cdir_disk_num);         // 16
    zipRead(buf, pos, m_cdir_entries);          // 16
    zipRead(buf, pos, m_cdir_total_entries);    // 16
    zipRead(buf, pos, m_cdir_size);             // 32
    zipRead(buf, pos, m_cdir_offset);           // 32
    uint16_t comment_len;
    zipRead(buf, pos, comment_len);             // 16

    // WARNING: we do not read the comment at this point because
    //          we did not check the size "properly" for it...
    //
    //buffer_t comment;
    //zipRead(buf, pos, comment, comment_len);    // string
    //m_zip_comment = std::string(reinterpret_cast<char *>(&comment[0]), comment_len);

//cerr << "Zip comment length = " << zip_comment_len << endl ;
//cerr << "Length of remaining file = " << buf.size() - pos << endl ;

    return true;
}


void EndOfCentralDirectory::write(std::ostream& os)
{
    if(os)
    {
        uint16_t comment_len(m_zip_comment.length());
        buffer_t comment(m_zip_comment.begin(), m_zip_comment.end());

        zipWrite(os, g_signature         );    // 32
        zipWrite(os, m_disk_num          );    // 16
        zipWrite(os, m_cdir_disk_num     );    // 16
        zipWrite(os, m_cdir_entries      );    // 16
        zipWrite(os, m_cdir_total_entries);    // 16
        zipWrite(os, m_cdir_size         );    // 32
        zipWrite(os, m_cdir_offset       );    // 32
        zipWrite(os, comment_len         );    // 16
        zipWrite(os, comment             );    // string
    }
}


} // zipios namespace
// vim: ts=4 sw=4 et
