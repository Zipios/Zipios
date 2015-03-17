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


#include <stdlib.h>

#include "zipios++/zipios-config.h"
#include "zipios++/meta-iostreams.h"

#include "zipios++/zipinputstream.h"

#include "zipinputstreamtest.h"

namespace zipios_test
{

TestFiles const ZipInputStreamTest::TEST_FILES;


void ZipInputStreamTest::testFirstMethod()
{
  //CPPUNIT_FAIL( "Implement this" );
}

void ZipInputStreamTest::testZipContents()
{
  zipios::ZipInputStream zis("test.zip");
  int count(0);
  while (zis.getNextEntry()->isValid())
  {
    count++;
  }
  CPPUNIT_ASSERT_EQUAL(4, count);
}

void ZipInputStreamTest::testZipContentNames()
{
  std::vector<std::string> entries;
  entries.push_back("file1.txt"); // got these from unzip -l test.zip
  entries.push_back("file2.txt");
  entries.push_back("file3.txt");
  entries.push_back("testfile.bin");
  zipios::ZipInputStream zis("test.zip");
  zipios::ConstEntryPointer poi(zis.getNextEntry());
  int count(0);
  while( poi->isValid() )
  {
      CPPUNIT_ASSERT_EQUAL( entries[count], poi->getName() );
      poi = zis.getNextEntry();
      count++;
  }
}

void ZipInputStreamTest::testZipFileSizes()
{
  std::vector<uint32_t> entries;
  entries.push_back(1327); // got these from unzip -l test.zip
  entries.push_back(17992);
  entries.push_back(8);
  entries.push_back(76468);
  zipios::ZipInputStream zis("test.zip");
  zipios::ConstEntryPointer poi(zis.getNextEntry());
  int count(0);
  while( poi->isValid() )
  {
      CPPUNIT_ASSERT_EQUAL( entries[count], poi->getSize() );
      poi = zis.getNextEntry();
      count++;
  }
}


void ZipInputStreamTest::testDirectory()
{
  zipios::ZipInputStream zis("test.zip"); //only files in this
  zipios::ConstEntryPointer poi(zis.getNextEntry());
  while( poi->isValid() )
  {
      CPPUNIT_ASSERT_EQUAL( false, poi->isDirectory() );
      poi = zis.getNextEntry();
  }
}

} // zipios_test namespace

// vim: ts=2 sw=2 et

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
