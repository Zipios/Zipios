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

