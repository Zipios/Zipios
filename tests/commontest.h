#ifndef ZIPIOS_COMMONTEST_H
#define ZIPIOS_COMMONTEST_H

#include <string>
#include <vector>

namespace zipios {

  class TestFiles : public std::vector<std::string> {
  public:
    TestFiles() {
      push_back("all_tests");
      push_back("file1.txt");
      push_back("file2.txt");
      push_back("file3.txt");
    }
    static const std::string TEST_ZIPFILE_NAME;
  };


} // namespace

#endif
