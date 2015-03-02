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
    \anchor zipfiletest_anchor
    Source code to a small program that tests the functionality of Zipios++.
*/

#include "zipfiletest.h"

#include "zipios++/zipfile.h"
//#include "zipios++/zipinputstream.h"
//#include "src/outputstringstream.h"

//#include <memory>
//#include <vector>
//#include <sstream>
//#include <stdlib.h>
//#include <fstream>



namespace zipios_test
{


void ZipFileTest::testUnzip()
{
  {
    // verify that we have 4 files in "test.zip"
    zipios::ZipFile zipFile("test.zip");
    CPPUNIT_ASSERT_EQUAL(4, zipFile.size());
  }

  {
    // verify that we can properly extract the content of
    // all the files in "test.zip"
    std::vector<std::string> entries;
    entries.push_back("file1.txt");
    entries.push_back("file2.txt");
    entries.push_back("file3.txt");
    entries.push_back("testfile.bin");

    compareZipFile("test.zip", entries);
  }
}


void ZipFileTest::testZipUnzip()
{
  // list of files to test with
  std::vector<std::string> entries;
  entries.push_back("testfile.bin");
  entries.push_back("all_tests.cpp");
  entries.push_back("zipfiletest.cpp");
  entries.push_back("zipfiletest.h");
  entries.push_back("commontest.cpp");
  std::string const zipFileName = "gentest.zip";

  // create zip file
  writeZipFile(zipFileName, entries);

  // verify that we can unzip that file
  compareZipFile(zipFileName, entries);
}


void ZipFileTest::testComment()
{
  // TODO: actually implement the test
  //
  // ZipFile zipFile("test.zip");
  //CPPUNIT_ASSERT_EQUAL("something", zipFile.getComment());
}


void ZipFileTest::writeZipFile(std::string const& zipFileName,
                               std::vector<std::string> const& entryFileNames)
{
  zipios::ZipOutputStream zos(zipFileName);
  for (auto it = entryFileNames.begin();
       it != entryFileNames.end();
       ++it )
  {
    writeFileToZipOutputStream(zos, *it);
  }
  zos.close();
}


void ZipFileTest::compareZipFile(std::string const& zipFileName,
                                 std::vector<std::string> const& entryFileNames)
{
  zipios::ZipFile zipFile(zipFileName);
  for ( auto it = entryFileNames.begin();
        it != entryFileNames.end();
        ++it )
  {
    std::auto_ptr<std::istream> zis(zipFile.getInputStream(*it));
    CPPUNIT_ASSERT_MESSAGE("Entry '" + *it + "' not found in zip file", zis.get());
    std::ifstream fis(it->c_str(), std::ios::in | std::ios::binary);
    compareStreams(*it, *zis, fis);
  }
}


void ZipFileTest::writeFileToZipOutputStream( zipios::ZipOutputStream &zos, std::string const& filename )
{
  zos.putNextEntry( zipios::ZipCDirEntry( filename ) ) ;
  std::ifstream ifs( filename.c_str(), std::ios::in | std::ios::binary ) ;
  CPPUNIT_ASSERT_MESSAGE("Could not open file '" + filename + "'", ifs);
  zos << ifs.rdbuf() ;

//    std::cerr << "ostream Stream state: "  ;
//    std::cerr << "good() = " << zos.good() << ",\t" ;
//    std::cerr << "fail() = " << zos.fail() << ",\t" ;
//    std::cerr << "bad()  = " << zos.bad()  << ",\t" ;
//    std::cerr << "eof()  = " << zos.eof()  << std::endl  ;

//    std::cerr << "istream Stream state: "  ;
//    std::cerr << "good() = " << ifs.good() << ",\t" ;
//    std::cerr << "fail() = " << ifs.fail() << ",\t" ;
//    std::cerr << "bad()  = " << ifs.bad()  << ",\t" ;
//    std::cerr << "eof()  = " << ifs.eof()  << std::endl  ;

}

void ZipFileTest::compareStreams(std::string const& entryName,
                                 std::istream& is1,
                                 std::istream& is2)
{
  zipios::OutputStringStream buf1, buf2;
  buf1 << is1.rdbuf();
  buf2 << is2.rdbuf();

std::cerr << "\nchecking " << entryName << ":\n";
if(buf1.str() != buf2.str())
{
  std::string s1(buf1.str());
  std::string s2(buf2.str());
  std::cerr << "s1 = " << s1.size() << " and s2 = " << s2.size() << "\n";
  for(size_t idx(0); idx < s1.size() && idx < s2.size(); ++idx)
  {
    std::cerr << std::hex << "  " << static_cast<int>(s1[idx]) << " " << static_cast<int>(s2[idx]) << "\n";
  }
  std::cerr << std::dec;
}

  CPPUNIT_ASSERT_MESSAGE("Streams differ for entry '" + entryName + "'",
                         buf1.str() == buf2.str());
}


void ZipFileTest::testClone()
{
  // TODO: actually implement test
  zipios::ZipFile zipFile("test.zip");
  std::cout << "Testing cloning..need some help here" << std::endl;

  // FileCollection newzip = clone("test.zip");
  //newzip.clone("test.zip");
}


} // zipios_test namespace

// vim: ts=2 sw=2 et
