/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015  Made to Order Software Corporation
  
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
    Source for a test program for testing ZipOutputStreambuf.
*/


#include "zipios++/zipoutputstreambuf.h"


void writeFileToZipOutputStreambuf( zipios::ZipOutputStreambuf& zosb, std::string const& filename )
{
  zosb.putNextEntry( zipios::ZipCDirEntry( filename ) ) ;

  std::ifstream ifs( filename.c_str(), std::ios::in | std::ios::binary ) ;
  std::ostream ozs( &zosb ) ;
  ozs << ifs.rdbuf() ; 
  std::cerr << ifs.rdbuf() ;
//    char buf[ 100 ] ;
//    int rds ;
//    while ( ( rds = ifs.rdbuf()->sgetn( buf, 100 ) ) > 0 ) 
//      ozs.write( buf, rds ) ;

  std::cerr << "ostream Stream state: "  ;
  std::cerr << "good() = " << ozs.good() << ",\t" ;
  std::cerr << "fail() = " << ozs.fail() << ",\t" ;
  std::cerr << "bad()  = " << ozs.bad()  << ",\t" ;
  std::cerr << "eof()  = " << ozs.eof()  << std::endl << std::endl;

  std::cerr << "istream Stream state: "  ;
  std::cerr << "good() = " << ifs.good() << ",\t" ;
  std::cerr << "fail() = " << ifs.fail() << ",\t" ;
  std::cerr << "bad()  = " << ifs.bad()  << ",\t" ;
  std::cerr << "eof()  = " << ifs.eof()  << std::endl << std::endl;

}

int main() {
  try {
    std::ofstream of( "zosb.zip", std::ios::out | std::ios::binary ) ;
    
    zipios::ZipOutputStreambuf ozf( of.rdbuf() ) ;
    
    writeFileToZipOutputStreambuf( ozf, "test_zip" ) ;
    writeFileToZipOutputStreambuf( ozf, "test_dircoll" ) ;
    
    std::cerr << "End of main" << std::endl ;
    
  }
  catch( std::exception &e )
  {
    std::cerr << "Exception caught in main() :" << std::endl ;
    std::cerr << e.what() << std::endl ;
    return 1;
  }

  return 0;
}

// vim: ts=2 sw=2 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
