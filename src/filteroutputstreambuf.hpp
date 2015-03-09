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
 * \brief Header file that defines FilterOutputStreambuf.
 */

#include <iostream>


namespace zipios
{


class FilterOutputStreambuf : public std::streambuf
{
public:
    explicit            FilterOutputStreambuf(std::streambuf *outbuf);
    virtual             ~FilterOutputStreambuf();

protected:
    std::streambuf *    m_outbuf;
};


} // namespace
// vim: ts=4 sw=4 et
