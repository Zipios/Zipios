#pragma once
#ifndef BACKBUFFER_HPP
#define BACKBUFFER_HPP

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
 * \brief The header file for zipios::BackBuffer
 *
 * The zipios::BackBuffer class is used to read a file backward.
 */

#include "zipios++/virtualseeker.hpp"

#include "zipios_common.hpp"


namespace zipios
{


class BackBuffer : public ::zipios::buffer_t
{
public:
                            BackBuffer(std::istream& is, VirtualSeeker const& vs = VirtualSeeker(), ssize_t const chunk_size = 1024);

    ssize_t                 readChunk(ssize_t& read_pointer);

private:
    VirtualSeeker           m_vs;
    ssize_t                 m_chunk_size;
    std::istream&           m_is;
    std::streampos          m_file_pos;
};


} // zipios namespace

// vim: ts=4 sw=4 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

#endif
