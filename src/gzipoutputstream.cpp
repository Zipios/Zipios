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
    Implementation of GZIPOutputStream.
*/

#include "zipios++/gzipoutputstream.h"


namespace zipios
{


GZIPOutputStream::GZIPOutputStream( std::ostream &os )
  : std::ostream( 0 )
  , ofs( 0 )
{
  ozf = new GZIPOutputStreambuf( os.rdbuf() ) ;

  init( ozf ) ;
}


GZIPOutputStream::GZIPOutputStream( std::string const& filename )
  : std::ostream( 0 )
  , ofs( 0 )
{
  ofs = new std::ofstream( filename.c_str(), std::ios::out | std::ios::binary ) ;
  ozf = new GZIPOutputStreambuf( ofs->rdbuf() ) ;
  init( ozf ) ;
}


void GZIPOutputStream::setFilename( std::string const& filename )
{
  ozf->setFilename(filename);
}


void GZIPOutputStream::setComment( std::string const& comment )
{
  ozf->setComment(comment);
}


void GZIPOutputStream::close()
{
  ozf->close() ;
  if ( ofs )
  {
    ofs->close() ;
  }
}


void GZIPOutputStream::finish()
{
  ozf->finish() ;
}


GZIPOutputStream::~GZIPOutputStream()
{
  // It's ok to call delete with a Null pointer.
  delete ozf ;
  delete ofs ;
}


} // zipios namespace

// vim: ts=2 sw=2 et
