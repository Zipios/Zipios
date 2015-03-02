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
    Implementation of ZipOutputStream.
*/

#include "zipios++/zipoutputstream.h"


namespace zipios
{

ZipOutputStream::ZipOutputStream( std::ostream& os )
  : std::ostream( 0 )
// SGIs basic_ifstream calls istream with 0, but calls basic_ios constructor first??
  , f_ofs( 0 )
  , f_ozf( 0 )
{
  f_ozf = new ZipOutputStreambuf( os.rdbuf() ) ;

  try
  {
    init( f_ozf ) ;
  }
  catch(...)
  {
    delete f_ozf;
    throw;
  }
}


ZipOutputStream::ZipOutputStream( std::string const& filename )
  : std::ostream( 0 )
  , f_ofs( 0 )
  , f_ozf( 0 )
{
  f_ofs = new std::ofstream( filename.c_str(), std::ios::out | std::ios::binary ) ;
  try
  {
    f_ozf = new ZipOutputStreambuf( f_ofs->rdbuf() ) ;
  }
  catch(...)
  {
    delete f_ofs;
    throw;
  }
  try
  {
    init( f_ozf ) ;
  }
  catch(...)
  {
    delete f_ofs;
    delete f_ozf;
    throw;
  }
}


void ZipOutputStream::closeEntry()
{
  f_ozf->closeEntry() ;
}


void ZipOutputStream::close()
{
  f_ozf->close() ;  
  if ( f_ofs )
  {
    f_ofs->close() ;
  }
}


void ZipOutputStream::finish()
{
  f_ozf->finish() ;
}


void ZipOutputStream::putNextEntry( ZipCDirEntry const& entry )
{
  f_ozf->putNextEntry( entry ) ;
}

void ZipOutputStream::putNextEntry( std::string const& entryName)
{
  putNextEntry( ZipCDirEntry(entryName));
}


void ZipOutputStream::setComment( std::string const& comment )
{
  f_ozf->setComment( comment ) ;
}


void ZipOutputStream::setLevel( int level )
{
  f_ozf->setLevel( level ) ;
}


void ZipOutputStream::setMethod( StorageMethod method )
{
  f_ozf->setMethod( method ) ;
}


ZipOutputStream::~ZipOutputStream()
{
  // It's ok to call delete with a Null pointer.
  delete f_ozf ;
  delete f_ofs ;
}

} // namespace
// vim: ts=2 sw=2 et
