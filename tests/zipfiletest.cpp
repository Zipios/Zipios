
#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"
#include <memory>
#include <vector>
#include <stdlib.h>

#include "zipios++/zipfile.h"
#include "zipios++/zipinputstream.h"
#include "src/outputstringstream.h"

#include "zipfiletest.h"

using namespace zipios ;

using std::cerr ;
using std::cout ;
using std::endl ;
using std::auto_ptr ;
using std::ifstream ;
using std::string;
using std::vector;


void zipios::ZipFileTest::runTests() {
  const string zipFileName = "gentest.zip";
  vector<string> entries;
  entries.push_back("all_tests");
  entries.push_back("Makefile.in");
  entries.push_back("zipfiletest.cpp");
  entries.push_back("zipfiletest.h");
  entries.push_back("testcase.h");
  writeZipFile(zipFileName, entries);
  compareZipFile(zipFileName, entries);
}


void zipios::ZipFileTest::writeZipFile(const string &zipFileName, vector<string> entryFileNames) {
  ZipOutputStream zos(zipFileName);
  std::vector<string>::const_iterator it = entryFileNames.begin();
  for ( ; it != entryFileNames.end() ; ++it ) {
    writeFileToZipOutputStream(zos, *it);
  }
  zos.close();
}

void zipios::ZipFileTest::compareZipFile(const string &zipFileName, vector<string> entryFileNames) {
  using namespace std;
  ZipFile zipFile(zipFileName);
  vector<string>::const_iterator it = entryFileNames.begin();
  for ( ; it != entryFileNames.end() ; ++it ) {
    auto_ptr<istream> zis(zipFile.getInputStream(*it));
    ifstream fis((*it).c_str(), ios::in | ios::binary);
    try {
      compareStreams(*zis, fis);
    } catch(Exception &ex) {
      throw Exception("For zip entry '"+*it+"': "+ex.what());
    }
  }
}

void zipios::ZipFileTest::writeFileToZipOutputStream( ZipOutputStream &zos, const string &filename ) {
  zos.putNextEntry( ZipCDirEntry( filename ) ) ;

  ifstream ifs( filename.c_str(), ios::in | ios::binary ) ;

  zos << ifs.rdbuf() ; 

//    cerr << "ostream Stream state: "  ;
//    cerr << "good() = " << zos.good() << ",\t" ;
//    cerr << "fail() = " << zos.fail() << ",\t" ;
//    cerr << "bad()  = " << zos.bad()  << ",\t" ;
//    cerr << "eof()  = " << zos.eof()  << endl  ;

//    cerr << "istream Stream state: "  ;
//    cerr << "good() = " << ifs.good() << ",\t" ;
//    cerr << "fail() = " << ifs.fail() << ",\t" ;
//    cerr << "bad()  = " << ifs.bad()  << ",\t" ;
//    cerr << "eof()  = " << ifs.eof()  << endl  ;

}

void zipios::ZipFileTest::compareStreams(istream &is1, istream &is2) {
  OutputStringStream buf1, buf2;
  buf1 << is1.rdbuf();
  buf2 << is2.rdbuf();
  if ( buf1.str() != buf2.str() ) {
	  ofstream of1("hej1.txt", ios::out | ios::binary );
	  of1 << buf1.str() << std::endl 
	         << "-------------------------------------------------------" << std::endl 
	         << buf2.str() << std::endl ;;
	  throw Exception("contents differ"); // defined at the bottom of fcolexceptions.h
  }
}

/** \file
    \anchor zipfiletest_anchor
    Source code to a small program that tests the functionality of Zipios++.
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

