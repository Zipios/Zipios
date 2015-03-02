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
    Implementation of ZipInputStream.
*/

#include "zipios++/zipinputstream.h"


namespace zipios
{


ZipInputStream::ZipInputStream( std::istream &is, std::streampos pos )
  : std::istream( 0 )
  , f_ifs( 0 )
  , f_izf( 0 )
{
  f_izf = new ZipInputStreambuf( is.rdbuf(), pos ) ;
//  rdbuf( f_izf ) ; is replaced by:
  try
  {
    init( f_izf ) ;
  }
  catch(...)
  {
    delete f_izf;
    throw;
  }
}


ZipInputStream::ZipInputStream( std::string const& filename, std::streampos pos )
  : std::istream( 0 )
  , f_ifs( 0 )
  , f_izf( 0 )
{
  f_ifs = new std::ifstream( filename.c_str(), std::ios::in | std::ios::binary ) ;
  try
  {
    f_izf = new ZipInputStreambuf( f_ifs->rdbuf(), pos ) ;
  }
  catch(...)
  {
    delete f_ifs;
    throw;
  }
//  rdbuf( f_izf ) ; is replaced by:
  try
  {
    init( f_izf ) ;
  }
  catch(...)
  {
    delete f_izf;
    delete f_ifs;
    throw;
  }
}


int ZipInputStream::available()
{
  return 1 ; // FIXME: Dummy implementation
}


void ZipInputStream::closeEntry()
{
  f_izf->closeEntry() ;
}


void ZipInputStream::close()
{
  f_izf->close() ;  
}

//    ZipLocalEntry *ZipInputStream::createZipCDirEntry( const string &name ) {}

ConstEntryPointer ZipInputStream::getNextEntry()
{
  clear() ; // clear eof and other flags.
  return f_izf->getNextEntry() ;
}


ZipInputStream::~ZipInputStream()
{
  // It's ok to call delete with a Null pointer.
  delete f_izf ;
  delete f_ifs ;
}


} // zipios namespace
// vim: ts=2 sw=2 et
