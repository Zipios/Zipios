#include <stdlib.h>

#include "zipios++/zipios-config.h"
#include "zipios++/meta-iostreams.h"

#include "zipios++/zipinputstream.h"

#include "zipinputstreamtest.h"

const zipios::TestFiles zipios::ZipInputStreamTest::TEST_FILES;


void zipios::ZipInputStreamTest::testFirstMethod() {

  std::cout << " Trying to test methods..but I have no clue\n" << std::endl;
 
}

void zipios::ZipInputStreamTest::testZipContents() {
  ZipInputStream zis("test.zip");
  int count = 1;
  while (zis.getNextEntry()->isValid())
    count++;
  CPPUNIT_ASSERT_EQUAL(4, count);
}

void zipios::ZipInputStreamTest::testZipContentNames() {
  vector<string> entries;
  entries.push_back("file1.txt"); // got these from unzip -l test.zip
  entries.push_back("file2.txt");
  entries.push_back("file3.txt");
  entries.push_back("testfile.bin");
  ZipInputStream zis("test.zip");
  vector<string>::const_iterator it = entries.begin();
  ConstEntryPointer poi = zis.getNextEntry();
  int count =1;
  while( poi->isValid() ) {
      CPPUNIT_ASSERT_EQUAL( entries[count],poi->getName());    
      poi = zis.getNextEntry();
      count++;
  }
}
void zipios::ZipInputStreamTest::testZipFileSizes() {
  vector<int> entries;
  entries.push_back(1327); // got these from unzip -l test.zip
  entries.push_back(17992);
  entries.push_back(8);
  entries.push_back(76468);
  ZipInputStream zis("test.zip");
  ConstEntryPointer poi = zis.getNextEntry();
  int count = 1;
  while( poi->isValid() ) {
      int file_size = poi->getSize();
      CPPUNIT_ASSERT_EQUAL( entries[count],file_size);     
      poi = zis.getNextEntry();
      count++;
  }
}
