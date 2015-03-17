/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
    \anchor all_tests_anchor
    Zipios++ unit test suite.
*/


#include "zipinputstreamtest.h"
#include "zipoutputstreamtest.h"
#include "zipfiletest.h"

#include <cppunit/TestSuite.h>
#include <cppunit/TextTestResult.h>
#include <cppunit/TestCaller.h>

//#include <iostream>


int main(int /*argc*/, char* /*argv*/[])
{
  CppUnit::TextTestResult result;
  {
    CppUnit::TestSuite suite;
    suite.addTest(zipios_test::ZipOutputStreamTest::suite());
    suite.addTest(zipios_test::ZipInputStreamTest::suite());
    suite.addTest(zipios_test::ZipFileTest::suite());

    suite.run(&result);
    std::cout << result << std::endl;
  }
  return result.failures().size();
}



// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
