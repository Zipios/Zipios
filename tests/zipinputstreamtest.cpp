#include <stdlib.h>

#include "zipios++/zipios-config.h"
#include "zipios++/meta-iostreams.h"

#include "zipios++/zipinputstream.h"

#include "zipinputstreamtest.h"

namespace zipios {

const TestFiles ZipInputStreamTest::TEST_FILES;


void ZipInputStreamTest::testFirstMethod() {
  CPPUNIT_FAIL( "Implement this" );
}

void ZipInputStreamTest::testZipContents() {
  ZipInputStream zis("test.zip");
  int count(0);
  while (zis.getNextEntry()->isValid())
    count++;
  CPPUNIT_ASSERT_EQUAL(4, count);
}

void ZipInputStreamTest::testZipContentNames() {
  vector<string> entries;
  entries.push_back("file1.txt"); // got these from unzip -l test.zip
  entries.push_back("file2.txt");
  entries.push_back("file3.txt");
  entries.push_back("testfile.bin");
  ZipInputStream zis("test.zip");
  ConstEntryPointer poi(zis.getNextEntry());
  int count(0);
  while( poi->isValid() ) {
      CPPUNIT_ASSERT_EQUAL( entries[count], poi->getName() );
      poi = zis.getNextEntry();
      count++;
  }
}

void ZipInputStreamTest::testZipFileSizes() {
  vector<uint32> entries;
  entries.push_back(1327); // got these from unzip -l test.zip
  entries.push_back(17992);
  entries.push_back(8);
  entries.push_back(76468);
  ZipInputStream zis("test.zip");
  ConstEntryPointer poi(zis.getNextEntry());
  int count(0);
  while( poi->isValid() ) {
      CPPUNIT_ASSERT_EQUAL( entries[count], poi->getSize() );
      poi = zis.getNextEntry();
      count++;
  }
}

void ZipInputStreamTest::testDirectory() {
  ZipInputStream zis("test.zip"); //only files in this
  ConstEntryPointer poi(zis.getNextEntry());
  while( poi->isValid() ) {
      CPPUNIT_ASSERT_EQUAL( false, poi->isDirectory() );
      poi = zis.getNextEntry();
  }
}

}
