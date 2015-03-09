#pragma once
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
 * \brief Header file containing classes and functions for reading the central
 * directory and local header fields in a zip archive.
 */

#include "zipios_common.hpp"

#include <string>
#include <vector>


namespace zipios
{


class EndOfCentralDirectory
{
public:
                        EndOfCentralDirectory(std::string const& zip_comment = "");

    uint32_t            offset() const;
    uint16_t            totalCount() const;
    void                setCDirSize(uint32_t size);
    void                setOffset(uint32_t new_offset);

    void                setTotalCount(uint16_t c);
    std::streampos      eocdOffSetFromEnd() const;
    bool                read(::zipios::buffer_t const& buf, size_t pos);

    //void                read(std::istream& is);
    void                write(std::ostream& os);

private:
    uint16_t            m_disk_num = 0;
    uint16_t            m_cdir_disk_num = 0;
    uint16_t            m_cdir_entries = 0;
    uint16_t            m_cdir_total_entries = 0;
    uint32_t            m_cdir_size = 0;
    uint32_t            m_cdir_offset = 0;

    std::streampos      m_eocd_offset_from_end = 0; // Not a Zip defined field
    std::string         m_zip_comment;
};


} // zipios namespace
// vim: ts=4 sw=4 et
