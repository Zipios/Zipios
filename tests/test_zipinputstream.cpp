
#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"

#include "zipios++/fcollexceptions.h"
#include "zipios++/zipinputstream.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;

int main() {
  try {
    ZipInputStream is( "test.zip" ) ;
    
    //    string str ;
    //    while ( is ) {
    //      getline( is, str ) ; 
    //      cout << str ;
    //    }
    
    cout << is.rdbuf() ;
    
    is.getNextEntry() ;
    
    cout << is.rdbuf() ;
    
    is.getNextEntry() ;

    cout << is.rdbuf() ;

    cerr << "Stream.state:" << endl ;
    cerr << "is.good() = " << is.good() << endl ;
    cerr << "is.fail() = " << is.fail() << endl ;
    cerr << "is.bad()  = " << is.bad()  << endl ;
    cerr << "is.eof()  = " << is.eof()  << endl << endl;
    cerr << "End of main" << endl ;
  }
  catch( IOException &e ) {
    cerr << "IOException caught in main:" << endl ;
    cerr << e.what() << endl ;
  }
  catch( ... ) {
    cerr << "Unspecified exception caught in main:" << endl ;
  }

  return 0;
}

/** \file
    Source for a test program for testing ZipInputStream.
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
