#ifndef ZIPIOS_ZIPFILETEST_H
#define ZIPIOS_ZIPFILETEST_H

#include <string>
#include <vector>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

#include "zipios++/zipoutputstream.h"

namespace zipios {

  using std::string;
  using std::vector;
  using std::istream;
  
  class ZipFileTest : public CppUnit::TestCase {
  public:
    CPPUNIT_TEST_SUITE(ZipFileTest);
    CPPUNIT_TEST(testUnzip);
    CPPUNIT_TEST(testZipUnzip);
    CPPUNIT_TEST(testComment);
    CPPUNIT_TEST(testClone);
    CPPUNIT_TEST_SUITE_END();

    void testUnzip();
    void testZipUnzip();
    void testComment();
    void testClone();
    
  private:
    static void writeZipFile(const string &zipFileName, 
			     vector<string> entryFileNames);
    static void compareZipFile(const string &zipFileName, 
			       vector<string> entryFileNames);
    static void writeFileToZipOutputStream(ZipOutputStream &zos, 
					   const string &filename );
    static void compareStreams(const std::string& entryName,
			       istream &is1, istream &is2);
  };

} // zipios

#endif
