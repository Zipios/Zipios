
#include <stdlib.h>

#include "zipios++/zipios-config.h"
#include "zipios++/meta-iostreams.h"

#include "zipios++/zipoutputstream.h"

#include "zipoutputstreamtest.h"

using namespace zipios ;

using std::cout ;
using std::cerr ;
using std::endl ;
using std::istream ;
using std::ios ;
using std::ofstream ;
using std::string ;

const string zipios::ZipOutputStreamTest::TEST_ZIPFILE_NAME = "testout.zip";
const TestFiles zipios::ZipOutputStreamTest::TEST_FILES;


void zipios::ZipOutputStreamTest::testNativeUnzip() {
  if (! hasUnzip()) {
    cout << "'unzip' not present, skipping ZipFileTest::testNativeUnzip" 
	      << endl;
    return;
  }

  ZipOutputStream zos(TEST_ZIPFILE_NAME);
  
  std::vector<string>::const_iterator it;
  for(it=TEST_FILES.begin(); it!=TEST_FILES.end(); ++it)
    writeFileToZipOutputStream(zos, *it);
  zos.close();

  for(it=TEST_FILES.begin(); it!=TEST_FILES.end(); ++it)
    assertEntry(TEST_ZIPFILE_NAME, *it);
}

void zipios::ZipOutputStreamTest::writeFileToZipOutputStream(ZipOutputStream& zos,
 const string& entryName) {
  CPPUNIT_FAIL("Implement this");
}

void zipios::ZipOutputStreamTest::assertEntry(const string& zipFileName,
					      const string& entryName) {
  CPPUNIT_FAIL("Implement this");
}

bool zipios::ZipOutputStreamTest::hasUnzip() {
  return system("unzip >/dev/null") == 0;
}


void zipios::ZipOutputStreamTest::entryToFile(const string &ent_name, istream &is, 
					      const string &outfile,
                                              bool cerr_report) {
  ofstream ofs( outfile.c_str(), ios::out | ios::binary ) ;
  
  
  ofs << is.rdbuf() ;
  if ( cerr_report ) {
    cerr << "writing " << ent_name << " to " << outfile << endl ;
    cerr << "Stream state: "  ;
    cerr << "good() = " << is.good() << ",\t" ;
    cerr << "fail() = " << is.fail() << ",\t" ;
    cerr << "bad()  = " << is.bad()  << ",\t" ;
    cerr << "eof()  = " << is.eof()  << endl << endl;
  }
  ofs.close() ;
}


/** \file
    \anchor zipoutputstreamtest_anchor
    ZipOutputStream test suite.
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


