#ifndef ZIPIOS_ZIPOUTPUTSTREAMTEST_H
#define ZIPIOS_ZIPOUTPUTSTREAMTEST_H

#include <string>
#include <vector>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

#include "commontest.h"
#include "zipios++/zipoutputstream.h"

namespace zipios {

  using std::string;
  using std::vector;
  using std::istream;
  
  class ZipOutputStreamTest : public CppUnit::TestCase {
  public:
    CPPUNIT_TEST_SUITE(ZipOutputStreamTest);
    CPPUNIT_TEST(testNativeUnzip);
    CPPUNIT_TEST_SUITE_END();

    void testNativeUnzip();
    
  private:
    static bool hasUnzip();
    static void entryToFile(const string &ent_name, istream &is, 
			    const string &outfile,
			    bool cerr_report);
    static void writeFileToZipOutputStream(zipios::ZipOutputStream& zos, 
					   const std::string& entryName);

    static void assertEntry(const std::string& zipFileName,
			    const std::string& entryName);

    static const std::string TEST_ZIPFILE_NAME;

    static const TestFiles TEST_FILES;
  };

} // zipios

#endif
