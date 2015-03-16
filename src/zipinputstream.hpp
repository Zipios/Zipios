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



/** \brief The ZipInputStream to read data from a Zip archive.
 *
 * ZipInputStream is an istream that gets it's input from a zip file. The
 * interface approximates the interface of the Java
 * ZipInputStream.
 */
class ZipInputStream : public std::istream
{
public:

                    ZipInputStream(std::string const& filename, std::streampos pos = 0);
                    ZipInputStream(ZipInputStream const& src) = delete;
                    ZipInputStream const& operator = (ZipInputStream const& src) = delete;
    virtual         ~ZipInputStream() override;

    void            closeEntry();
    void            close();

    FileEntry::pointer_t    getNextEntry();

private:
    std::unique_ptr<std::ifstream>      m_ifs;
    std::unique_ptr<ZipInputStreambuf>  m_izf;
};


} // zipios namespace
// vim: ts=4 sw=4 et
