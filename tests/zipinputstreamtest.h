
#include <string>
#include <vector>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

#include "commontest.h"
#include "zipios++/zipinputstream.h"

namespace zipios {

  using std::string;
  using std::vector;
  using std::istream;


  class ZipInputStreamTest : public CppUnit::TestCase {
  public:
    CPPUNIT_TEST_SUITE(ZipInputStreamTest);
    CPPUNIT_TEST(testFirstMethod);
    CPPUNIT_TEST(testZipContents);
    CPPUNIT_TEST(testZipContentNames);
    CPPUNIT_TEST(testZipFileSizes);
    CPPUNIT_TEST(testDirectory);
    CPPUNIT_TEST_SUITE_END();

    void testFirstMethod();
    void testZipContents();
    void testZipContentNames();
    void testZipFileSizes();
    void testDirectory();
    
    static const TestFiles TEST_FILES;
  private:
     
    int dummy;    

  };

} // zipios
