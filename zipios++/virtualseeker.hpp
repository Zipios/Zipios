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
 * \brief Header file that defines VirtualSeeker.
 */

#include "zipios++/meta-iostreams.hpp"

#include <stdexcept>


namespace zipios
{


class VirtualSeeker
{
public:
    typedef off_t   offset_t;

                    VirtualSeeker(offset_t start_offset = 0, offset_t end_offset = 0);

    void            setOffsets(offset_t start_offset, offset_t end_offset);
    void            getOffsets(offset_t& start_offset, offset_t& end_offset) const;
    offset_t        startOffset() const;
    offset_t        endOffset() const;
    void            vseekg(std::istream& is, offset_t offset, std::ios::seekdir sd) const;
    std::streampos  vtellg(std::istream& is) const;

private:
    offset_t        m_start_offset = 0;
    offset_t        m_end_offset = 0;
};


} // namespace
// vim: ts=4 sw=4 et
