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
    \anchor zipfiletest_anchor
    Source code to a small program that tests the functionality of Zipios++.
*/



#include "zipios++/zipoutputstream.h"

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>

namespace zipios_test
{


class ZipFileTest : public CppUnit::TestCase
{
public:
  CPPUNIT_TEST_SUITE(ZipFileTest);
    CPPUNIT_TEST(testUnzip);
    CPPUNIT_TEST(testZipUnzip);
    CPPUNIT_TEST(testComment);
    CPPUNIT_TEST(testClone);
  CPPUNIT_TEST_SUITE_END();

private:
  void testUnzip();
  void testZipUnzip();
  void testComment();
  void testClone();

  void writeZipFile(std::string const& zipFileName, std::vector<std::string> const& entryFileNames);
  void compareZipFile(std::string const& zipFileName, std::vector<std::string> const& entryFileNames);
  void writeFileToZipOutputStream(zipios::ZipOutputStream &zos, std::string const& filename);
  void compareStreams(std::string const& entryName, std::istream& is1, std::istream& is2);
};


} // zipios_test namespace

// vim: ts=2 sw=2 et
