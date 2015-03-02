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


#include <string>
#include <vector>

namespace zipios_test
{

class TestFiles : public std::vector<std::string>
{
public:
  TestFiles()
  {
    push_back("all_tests");
    push_back("file1.txt");
    push_back("file2.txt");
    push_back("file3.txt");
  }

  static std::string const TEST_ZIPFILE_NAME;
};


} // zipios_test namespace

// vim: ts=2 sw=2 et
