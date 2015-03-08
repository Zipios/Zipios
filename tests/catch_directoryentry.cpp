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
 *
 * Zipios++ unit tests for the DirectoryEntry class.
 */

#include "catch_tests.h"

#include "zipios++/directoryentry.hpp"

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>


SCENARIO("DirectoryEntry with invalid paths", "[DirectoryEntry] [FileEntry]")
{
    GIVEN("test a fantom file (path that \"cannot\" exists) and no comment")
    {
        zipios::DirectoryEntry de(zipios::FilePath("/this/file/really/should/not/exist/period.txt"), "");

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE(de.getComment().empty());
            REQUIRE(de.getCompressedSize() == 0);
            REQUIRE(de.getCrc() == 0);
            REQUIRE(de.getExtra().empty());
            REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
            REQUIRE(de.getFileName() == "period.txt");
            REQUIRE(de.getSize() == 0);
            REQUIRE(de.getTime() == 0);  // invalid date
            REQUIRE(de.getUnixTime() == 0);
            REQUIRE(!de.hasCrc());
            REQUIRE(!de.isDirectory());
            REQUIRE(!de.isValid());
            REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
        }

        //WHEN("and changing the path to something else as unexistant with assignment operator works too")
        //{
        //    fp = "/this/is/another/path/changed/with/assignment/operator";

        //    THEN("path was replaced")
        //    {
        //        // retrieve the path
        //        std::string const p(fp);
        //        REQUIRE(p == "/this/is/another/path/changed/with/assignment/operator");

        //        REQUIRE(fp == "/this/is/another/path/changed/with/assignment/operator");
        //        REQUIRE("/this/is/another/path/changed/with/assignment/operator" == fp);
        //        REQUIRE(fp == std::string("/this/is/another/path/changed/with/assignment/operator"));
        //        REQUIRE(std::string("/this/is/another/path/changed/with/assignment/operator") == fp);

        //        REQUIRE(!(fp == "this/is/another/path/changed/with/assignment/operator"));
        //        REQUIRE(!("/this/is/another/path/chnged/with/assignment/operator" == fp));
        //        REQUIRE(!(fp == std::string("/this/is/another/path/changed/with/asignment/operator")));
        //        REQUIRE(!(std::string("/this/is/another/path/changed/with/assignment/oprator") == fp));

        //        // check basename
        //        REQUIRE(static_cast<std::string>(fp.filename()) == "operator");

        //        REQUIRE(fp.length() == 54);
        //        REQUIRE(fp.size() == 54);

        //        // all flags must be false
        //        REQUIRE(!fp.exists());
        //        REQUIRE(!fp.isRegular());
        //        REQUIRE(!fp.isDirectory());
        //        REQUIRE(!fp.isCharSpecial());
        //        REQUIRE(!fp.isBlockSpecial());
        //        REQUIRE(!fp.isSocket());
        //        REQUIRE(!fp.isFifo());
        //    }
        //}
    }
}



// vim: ts=4 sw=4 et
