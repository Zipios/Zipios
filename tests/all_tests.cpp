

#include <iostream>

//#include "simplesmartptrtest.h"
#include "zipfiletest.h"

using std::cout;
using std::endl;
using namespace zipios;

int main() {
  try {
    //  SimpleSmartPointerTest sspt;
    //  sspt.runTests();
    
    ZipFileTest zipFileTest;
    zipFileTest.runTests();
  } catch (exception &ex) {
    cout << "Exception: " << ex.what() << endl;
    return 1;
  }
  return 0;
}
