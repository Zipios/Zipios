#pragma once
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
    Source for a test program for testing ZipOutputStreambuf.
*/


#include "zipios++/zipinputstream.h"
#include "commontest.h"

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


namespace zipios_test
{

class ZipInputStreamTest : public CppUnit::TestCase
{
public:
  CPPUNIT_TEST_SUITE(ZipInputStreamTest);
    CPPUNIT_TEST(testFirstMethod);
    CPPUNIT_TEST(testZipContents);
    CPPUNIT_TEST(testZipContentNames);
    CPPUNIT_TEST(testZipFileSizes);
    CPPUNIT_TEST(testDirectory);
  CPPUNIT_TEST_SUITE_END();

private:
  void testFirstMethod();
  void testZipContents();
  void testZipContentNames();
  void testZipFileSizes();
  void testDirectory();

  int dummy;    
  static TestFiles const TEST_FILES;
};

} // zipios_test

// vim: ts=2 sw=2 et
