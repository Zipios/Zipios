#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"
#include <string>
#include <exception>

#include "zipios++/gzipoutputstream.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::ifstream ;
using std::ofstream ;
using std::ios ;
using std::string ;
using std::exception ;

void writeFileToGZIPOutputStream( GZIPOutputStream &zos, const string &filename ) ;

int main() {
  try {

    GZIPOutputStream ozs("zos.gz") ;

    writeFileToGZIPOutputStream( ozs, "test.xml" ) ;

    cerr << "End of main" << endl ;

    return 0;
  }
  catch( exception &excp ) {
    cerr << "Exception caught in main() :" << endl ;
    cerr << excp.what() << endl ;
  }
  return -1;
}

void writeFileToGZIPOutputStream( GZIPOutputStream &zos, const string &filename ) {

  ifstream ifs( filename.c_str(), ios::in | ios::binary ) ;

  zos.setFilename(filename);
  zos.setComment("ZIPIOS++ TestFile");
  zos << ifs.rdbuf() ;

  cerr << "ostream Stream state: "  ;
  cerr << "good() = " << zos.good() << ",\t" ;
  cerr << "fail() = " << zos.fail() << ",\t" ;
  cerr << "bad()  = " << zos.bad()  << ",\t" ;
  cerr << "eof()  = " << zos.eof()  << endl  ;

  cerr << "istream Stream state: "  ;
  cerr << "good() = " << ifs.good() << ",\t" ;
  cerr << "fail() = " << ifs.fail() << ",\t" ;
  cerr << "bad()  = " << ifs.bad()  << ",\t" ;
  cerr << "eof()  = " << ifs.eof()  << endl  ;

}

/** \file
    Source for a test program for testing GZIPOutputStream.
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
