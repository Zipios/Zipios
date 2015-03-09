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
 * \brief Implementation of ZipOutputStream.
 */

#include "zipoutputstream.hpp"


namespace zipios
{


ZipOutputStream::ZipOutputStream(std::ostream& os)
    //: std::ostream(nullptr)
    //, m_ofs(nullptr)
    : m_ozf(new ZipOutputStreambuf(os.rdbuf()))
{
    init(m_ozf.get());
}


ZipOutputStream::ZipOutputStream(std::string const& filename)
    //: std::ostream(nullptr)
    : m_ofs(new std::ofstream(filename.c_str(), std::ios::out | std::ios::binary))
    , m_ozf(new ZipOutputStreambuf(m_ofs->rdbuf()))
{
    init(m_ozf.get());
}


ZipOutputStream::~ZipOutputStream()
{
}


void ZipOutputStream::closeEntry()
{
    m_ozf->closeEntry();
}


void ZipOutputStream::close()
{
    m_ozf->close();
    if(m_ofs)
    {
        m_ofs->close();
    }
}


void ZipOutputStream::finish()
{
    m_ozf->finish();
}


void ZipOutputStream::putNextEntry(ZipCDirEntry const& entry)
{
    m_ozf->putNextEntry(entry);
}


void ZipOutputStream::putNextEntry(std::string const& entryName)
{
    putNextEntry(ZipCDirEntry(entryName));
}


void ZipOutputStream::setComment(std::string const& comment)
{
    m_ozf->setComment(comment);
}


void ZipOutputStream::setLevel(int level)
{
    m_ozf->setLevel(level);
}


void ZipOutputStream::setMethod(StorageMethod method)
{
    m_ozf->setMethod(method);
}


} // zipios namespace
// vim: ts=4 sw=4 et
