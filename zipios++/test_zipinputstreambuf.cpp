
#include "zipios++/zipios-config.h"

#if defined (HAVE_STD_IOSTREAM) && defined (USE_STD_IOSTREAM)
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

#include "zipios++/zipinputstreambuf.h"

using namespace zipios ;

using std::ifstream ;
using std::cout ;
using std::cerr ;
using std::endl ;

int main() {
  ifstream f( "test.zip" ) ;
  ZipInputStreambuf izf( f.rdbuf() ) ;
  istream is( &izf ) ;

//    string str ;
//    while ( is ) {
//      getline( is, str ) ; 
//      cout << str ;
//    }
  
  const int buflen = 2 ;
  char buf[ buflen ] ;
  
  
  while ( is && ! is.eof() ) {
    is.read( buf, buflen - 1 ) ;
    buf[ is.gcount() ] = '\0' ;
    cout << buf ;
  }

  izf.getNextEntry() ;
  istream is2( &izf ) ;

  while ( is2 && ! is2.eof() ) {
    is2.read( buf, buflen - 1 ) ;
    buf[ is2.gcount() ] = '\0' ;
    cout << buf ;
  }

  cerr << "End of main" << endl ;
}

/** \file
    Source for a test program for testing ZipInputStreambuf.
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
