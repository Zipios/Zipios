
#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"

#include "zipios++/zipinputstreambuf.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::ifstream ;
using std::ios ;

int main() {
  ifstream f( "test.zip", ios::in | ios::binary ) ;
  ZipInputStreambuf izf( f.rdbuf() ) ;
  istream is( &izf ) ;

//    string str ;
//    while ( is ) {
//      getline( is, str ) ; 
//      cout << str ;
//    }
  
  
  cout << is.rdbuf() ;

  izf.getNextEntry() ;
  istream is2( &izf ) ;

  cout << is2.rdbuf() ;

  cerr << "End of main" << endl ;

  return 0;
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
