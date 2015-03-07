#pragma
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
    The header file for BackBuffer
*/

#include "zipios++/virtualseeker.hpp"

#include "zipheadio.hpp"


namespace zipios
{


class BackBuffer : public std::vector<unsigned char>
{
public:
    explicit                BackBuffer(std::istream& is, VirtualSeeker const& vs = VirtualSeeker(), int const chunk_size = 1024 );

    int                     readChunk(int &read_pointer);

private:
    VirtualSeeker           m_vs;
    int                     m_chunk_size;
    std::istream&           m_is;
    std::streampos          m_file_pos;
};


} // zipios namespace
// vim: ts=4 sw=4 et
