
#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"

#include "zipios++/zipoutputstreambuf.h"
#include "zipios++/zipoutputstream.h"

namespace zipios {

ZipOutputStream::ZipOutputStream( ostream &os, streampos pos ) 
  : ostream( 0 ), 
// SGIs basic_ifstream calls istream with 0, but calls basic_ios constructor first??
    ofs( 0 )
{
  ozf = new ZipOutputStreambuf( os.rdbuf(), pos ) ;
  
  init( ozf ) ;
}

ZipOutputStream::ZipOutputStream( const string &filename, streampos pos )
  : ostream( 0 ),
    ofs( 0 )
{
  ofs = new ofstream( filename.c_str() ) ;
  ozf = new ZipOutputStreambuf( ofs->rdbuf(), pos ) ;
  this->init( ozf ) ;
}

void ZipOutputStream::closeEntry() {
  ozf->closeEntry() ;
}

void ZipOutputStream::close() {
  ozf->close() ;  
}


//  ConstEntryPointer ZipOutputStream::getNextEntry() {
//    clear() ; // clear eof and other flags.
//    return izf->getNextEntry() ;
//  }

ZipOutputStream::~ZipOutputStream() {
  // It's ok to call delete with a Null pointer.
  delete ofs ;
  delete ozf ;
}

} // namespace

/** \file
    Implementation of ZipOutputStream.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
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
