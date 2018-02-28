#pragma once
#ifndef ZIPINPUTSTREAM_HPP
#define ZIPINPUTSTREAM_HPP

/*
  Zipios – a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2017  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 * \brief Define zipios::ZipInputStream.
 *
 * This file declares the zipios::ZipInputStream class.
 *
 * The class is used to read data from a Zip archive, data that may
 * have been compressed using the zlib library.
 */

#include "zipinputstreambuf.hpp"


namespace zipios
{



class ZIPIOSDLL_API ZipInputStream : public std::istream
{
public:
                    ZipInputStream(std::string const& filename, std::streampos pos = 0);
                    ZipInputStream(ZipInputStream const& src) = delete;
                    ZipInputStream const& operator = (ZipInputStream const& src) = delete;
    virtual         ~ZipInputStream() override;

private:
    std::unique_ptr<std::ifstream>      m_ifs;
    std::unique_ptr<ZipInputStreambuf>  m_izf;
};


} // zipios namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
#endif
