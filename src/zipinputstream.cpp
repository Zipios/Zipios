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
 * \brief Implementation of ZipInputStream.
 */

#include "zipinputstream.hpp"


namespace zipios
{


ZipInputStream::ZipInputStream(std::istream &is, std::streampos pos)
    //: std::istream(nullptr)
    //, m_ifs(nullptr) -- auto-init
    : m_izf(new ZipInputStreambuf(is.rdbuf(), pos))
{
    //  rdbuf(m_izf); is replaced by:
    init(m_izf.get());
}


ZipInputStream::ZipInputStream(std::string const& filename, std::streampos pos)
    //: std::istream(nullptr)
    : m_ifs(new std::ifstream(filename.c_str(), std::ios::in | std::ios::binary))
    , m_izf(new ZipInputStreambuf(m_ifs->rdbuf(), pos))
{
    //  rdbuf(m_izf); is replaced by:
    init(m_izf.get());
}


ZipInputStream::~ZipInputStream()
{
}


int ZipInputStream::available()
{
    return 1; // FIXME: Dummy implementation
}


void ZipInputStream::closeEntry()
{
    m_izf->closeEntry();
}


void ZipInputStream::close()
{
    m_izf->close();
}

//    ZipLocalEntry *ZipInputStream::createZipCDirEntry( const string &name ) {}

FileEntry::pointer_t ZipInputStream::getNextEntry()
{
    clear(); // clear eof and other flags.
    return m_izf->getNextEntry();
}


} // zipios namespace
// vim: ts=4 sw=4 et
