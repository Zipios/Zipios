#ifndef ZIPIOS_ZIPFILETEST_H
#define ZIPIOS_ZIPFILETEST_H

#include <string>
#include <vector>

#include "zipios++/zipoutputstream.h"

#include "testcase.h"

namespace zipios {

using std::string;
using std::vector;
using std::istream;

class ZipFileTest : public TestCase {
public:
  void runTests();
private:
  static void testUnzip();
  static void testZipUnzip();
  static void writeZipFile(const string &zipFileName, vector<string> entryFileNames);
  static void compareZipFile(const string &zipFileName, vector<string> entryFileNames);
  static void writeFileToZipOutputStream( ZipOutputStream &zos, const string &filename );
  static void compareStreams(istream &is1, istream &is2);
};

} // zipios

#endif
