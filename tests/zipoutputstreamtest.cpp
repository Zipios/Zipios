/** \file
    \anchor zipoutputstreamtest_anchor
    ZipOutputStream test suite.
*/

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

#include "zipoutputstreamtest.h"

#include <cstdlib>


namespace zipios_test
{

std::string const ZipOutputStreamTest::TEST_ZIPFILE_NAME = "testout.zip";
TestFiles const ZipOutputStreamTest::TEST_FILES;


void ZipOutputStreamTest::testNativeUnzip()
{
  if (! hasUnzip())
  {
    std::cout << "warning: 'unzip' not present, skipping ZipFileTest::testNativeUnzip" 
              << std::endl;
    return;
  }

  zipios::ZipOutputStream zos(TEST_ZIPFILE_NAME);

  for(auto it = TEST_FILES.begin(); it != TEST_FILES.end(); ++it)
  {
    writeFileToZipOutputStream(zos, *it);
  }
  zos.close();

  for(auto it = TEST_FILES.begin(); it != TEST_FILES.end(); ++it)
  {
    assertEntry(TEST_ZIPFILE_NAME, *it);
  }
}


void ZipOutputStreamTest::writeFileToZipOutputStream(zipios::ZipOutputStream& /*zos*/,
                                                     std::string const& /*entryName*/)
{
  // TODO: actually implement test
  //CPPUNIT_FAIL("Implement this");
}


void ZipOutputStreamTest::assertEntry(std::string const& /*zipFileName*/,
                                      std::string const& /*entryName*/)
{
  // TODO: actually implement test
  //CPPUNIT_FAIL("Implement this");
}


bool ZipOutputStreamTest::hasUnzip()
{
  return system("unzip >/dev/null") == 0;
}


void ZipOutputStreamTest::entryToFile(std::string const& ent_name,
                                      std::istream& is, 
                                      std::string const& outfile,
                                      bool cerr_report)
{
  std::ofstream ofs( outfile.c_str(), std::ios::out | std::ios::binary ) ;

  ofs << is.rdbuf() ;
  if ( cerr_report )
  {
    std::cerr << "writing " << ent_name << " to " << outfile << std::endl ;
    std::cerr << "Stream state: "  ;
    std::cerr << "good() = " << is.good() << ",\t" ;
    std::cerr << "fail() = " << is.fail() << ",\t" ;
    std::cerr << "bad()  = " << is.bad()  << ",\t" ;
    std::cerr << "eof()  = " << is.eof()  << std::endl << std::endl;
  }
  ofs.close() ;
}


} // zipios_test namespace
// vim: ts=2 sw=2 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
