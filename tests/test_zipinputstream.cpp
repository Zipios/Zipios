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
    Source for a test program for testing ZipInputStream.
*/


#include "zipios++/zipinputstream.h"
#include "zipios++/zipiosexceptions.h"


int main()
{
  try
  {
    zipios::ZipInputStream is( "test.zip" ) ;

    //    string str ;
    //    while ( is ) {
    //      getline( is, str ) ;
    //      cout << str ;
    //    }

    std::cout << is.rdbuf() ;

    is.getNextEntry() ;

    std::cout << is.rdbuf() ;

    is.getNextEntry() ;

    std::cout << is.rdbuf() ;

    std::cerr << "Stream.state:" << std::endl ;
    std::cerr << "is.good() = " << is.good() << std::endl ;
    std::cerr << "is.fail() = " << is.fail() << std::endl ;
    std::cerr << "is.bad()  = " << is.bad()  << std::endl ;
    std::cerr << "is.eof()  = " << is.eof()  << std::endl << std::endl;
    std::cerr << "End of main" << std::endl ;
  }
  catch( zipios::IOException const& e )
  {
    std::cerr << "IOException caught in main:" << std::endl ;
    std::cerr << e.what() << std::endl ;
  }
  catch( ... )
  {
    std::cerr << "Unspecified exception caught in main:" << std::endl ;
  }

  return 0;
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
