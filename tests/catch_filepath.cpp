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
 * \anchor catch_tests_anchor
 *
 * Zipios++ unit test suite using catch.hpp, see for details:
 *
 *   https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
 */

#include "catch_tests.h"

#include "zipios++/filepath.h"

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>


SCENARIO("FilePath that does not represent a file on disk", "[FilePath]")
{
    GIVEN("test a fantom file (path that \"cannot\" exists)")
    {
        zipios::FilePath fp("/this/file/really/should/not/exist/period.txt");

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "/this/file/really/should/not/exist/period.txt");

            // basename is "period.txt"
            REQUIRE(static_cast<std::string>(fp.filename()) == "period.txt");

            // all flags must be false
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());
        }

        WHEN("and changing the path to something else as unexistant with assignment operator works too")
        {
            fp = "/this/is/another/path/changed/with/assignment/operator";

            THEN("path was replaced")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "/this/is/another/path/changed/with/assignment/operator");

                // check basename
                REQUIRE(static_cast<std::string>(fp.filename()) == "operator");

                // all flags must be false
                REQUIRE(!fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        WHEN("still \"broken\" when appending another part (full path)")
        {
            zipios::FilePath path("/correct/path");

            zipios::FilePath appended(fp + path);

            THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                REQUIRE(p == "/this/file/really/should/not/exist/period.txt/correct/path");

                // still the same basename
                REQUIRE(static_cast<std::string>(appended.filename()) == "path");

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());
            }
        }

        WHEN("still \"broken\" when appending another part (relative)")
        {
            zipios::FilePath path("relative/path.hpp");

            // append to the left...
            zipios::FilePath appended(fp + path);

            THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                REQUIRE(p == "/this/file/really/should/not/exist/period.txt/relative/path.hpp");

                // check basename
                REQUIRE(static_cast<std::string>(appended.filename()) == "path.hpp");

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());
            }
        }

        WHEN("still \"broken\" when appending another part (empty)")
        {
            zipios::FilePath path("");

            // append to the left...
            zipios::FilePath appended(fp + path);

            THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                REQUIRE(p == "/this/file/really/should/not/exist/period.txt");

                // check basename
                REQUIRE(static_cast<std::string>(appended.filename()) == "period.txt");

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());
            }
        }
    }

    GIVEN("an empty path")
    {
        zipios::FilePath fp;

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "");

            // check basename
            REQUIRE(static_cast<std::string>(fp.filename()) == "");

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());
        }

        WHEN("we can concatenate another empty path to it")
        {
            zipios::FilePath ep;

            zipios::FilePath ee(fp + ep);

            THEN("file name is still empty")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "");

                // all flags must be false when empty
                // (because empty does not represent ".")
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }

        WHEN("we can concatenate a full regular path to it")
        {
            zipios::FilePath ep("/this/is/a/regular/path");

            zipios::FilePath ee(fp + ep);

            THEN("new path is equal to the concatenated path")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "/this/is/a/regular/path");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "path");

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }

        WHEN("we can concatenate a relative path to it")
        {
            zipios::FilePath ep("this/is/a/relative/path.xml");

            zipios::FilePath ee(fp + ep);

            THEN("concatenated path is the added path")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path.xml");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "path.xml");

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }
    }

    GIVEN("a fantom relative path")
    {
        zipios::FilePath fp("this/is/a/relative/path/file1.txt");

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "this/is/a/relative/path/file1.txt");

            // check basename
            REQUIRE(static_cast<std::string>(fp.filename()) == "file1.txt");

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());
        }

        WHEN("we can concatenate an empty path to it")
        {
            zipios::FilePath ep;

            zipios::FilePath ee(fp + ep);

            THEN("the result is the same as the left hand-side")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path/file1.txt");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "file1.txt");

                // all flags must be false when empty
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }

        WHEN("we can concatenate a full regular path to it")
        {
            zipios::FilePath ep("/this/is/a/regular/path");

            zipios::FilePath ee(fp + ep);

            THEN("path is the resulting concatenation with a slash at the end")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path/file1.txt/this/is/a/regular/path");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "path");

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }

        WHEN("we can concatenate a relative path to it")
        {
            zipios::FilePath ep("this/is/a/relative/path.xml");

            zipios::FilePath ee(fp + ep);

            THEN("the path changed")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path/file1.txt/this/is/a/relative/path.xml");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(ee.filename()) == "path.xml");

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }
    }

    GIVEN("a fantom path that ends with /")
    {
        zipios::FilePath fp("this/is/a/relative/path/");

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "this/is/a/relative/path");

            // check basename
            REQUIRE(static_cast<std::string>(fp.filename()) == "path");

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());
        }

        WHEN("we can concatenate another path, it also prune the /")
        {
            zipios::FilePath ep("add/this/with/a/slash/");

            zipios::FilePath ee(fp + ep);

            THEN("the result is as expected without the slash")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path/add/this/with/a/slash");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "slash");

                // all flags must be false when empty
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }
    }

    GIVEN("repeat this last fantom path test that ends with / with 'check_immediately' set")
    {
        zipios::FilePath fp("this/is/a/relative/path/", true);

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "this/is/a/relative/path");

            // check basename
            REQUIRE(static_cast<std::string>(fp.filename()) == "path");

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());
        }

        WHEN("we can concatenate another path, it also prune the /")
        {
            zipios::FilePath ep("add/this/with/a/slash/", true);

            zipios::FilePath ee(fp + ep);

            THEN("the result is as expected without the slash")
            {
                // retrieve the path
                std::string const p(ee);
                REQUIRE(p == "this/is/a/relative/path/add/this/with/a/slash");

                // check basename
                REQUIRE(static_cast<std::string>(ee.filename()) == "slash");

                // all flags must be false when empty
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());
            }
        }
    }
}


SCENARIO("FilePath against existing files on disk", "[FilePath]")
{
    GIVEN("an existing file")
    {
        {
            // create a file
            std::fstream f("filepath-test.txt", std::ios::out | std::ios::binary);
            f << "This is an existing file." << std::endl;
        }

        WHEN("creating a FilePath object")
        {
            zipios::FilePath fp("filepath-test.txt");

            THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test.txt");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        WHEN("creating a FilePath object with immediate flag set")
        {
            zipios::FilePath fp("filepath-test.txt", false);

            THEN("the file is found too")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test.txt");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        WHEN("creating a FilePath object with immediate flag set")
        {
            zipios::FilePath fp("filepath-test.txt", true);

            THEN("the file is found too")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test.txt");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }
    }

    GIVEN("an existing directory")
    {
        // make sure the directory is gone before re-creating it
        system("rm -rf filepath-test");
        // create a directory
        REQUIRE(mkdir("filepath-test", 0777) == 0);

        WHEN("creating a FilePath object")
        {
            zipios::FilePath fp("filepath-test");

            THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        WHEN("creating a FilePath object with the immediate flag set")
        {
            zipios::FilePath fp("filepath-test", false);

            THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        WHEN("creating a FilePath object with the immediate flag set")
        {
            zipios::FilePath fp("filepath-test", true);

            THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "filepath-test");

                // basename is "period.txt"
                REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test");

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());
            }
        }

        rmdir("filepath-test");
    }

    // TODO: add tests for other files types (not extremely useful
    //       for a small zip library though...)
}

// vim: ts=4 sw=4 et
