#pragma once
#ifndef FILTEROUTPUTSTREAMBUF_HPP
#define FILTEROUTPUTSTREAMBUF_HPP

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
 * \brief Declarations of the zipios::FilterOutputStreambuf.
 *
 * This file includes the declaration of the zipios::FileterOutputStreambuf
 * class which is used as a base class to filter output data.
 */

#include <iostream>


namespace zipios
{


class FilterOutputStreambuf : public std::streambuf
{
public:
                                    FilterOutputStreambuf(std::streambuf *outbuf);
                                    FilterOutputStreambuf(FilterOutputStreambuf const& src) = delete;
    FilterOutputStreambuf const&    operator = (FilterOutputStreambuf const& src) = delete;
    virtual                         ~FilterOutputStreambuf();

protected:
    std::streambuf *                m_outbuf;
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
