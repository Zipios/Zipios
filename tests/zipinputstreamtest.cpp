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
 for ( ; it != entries.end() ; ++it ) {
   //std::cout<<(*it).c_str()<<endl;
  }
}
