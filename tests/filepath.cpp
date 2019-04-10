/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2019  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 *
 * Zipios unit tests used to verify the FilePath class.
 */

#include "tests.hpp"

#include "zipios/filepath.hpp"

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

            REQUIRE(fp == "/this/file/really/should/not/exist/period.txt");
            REQUIRE("/this/file/really/should/not/exist/period.txt" == fp);
            REQUIRE(fp == std::string("/this/file/really/should/not/exist/period.txt"));
            REQUIRE(std::string("/this/file/really/should/not/exist/period.txt") == fp);

            REQUIRE(!(fp == "/this/file/really/should/not/exist/period"));
            REQUIRE(!("/file/really/should/not/exist/period.txt" == fp));
            REQUIRE(!(fp == std::string("/this/file/really/should/exist/period.txt")));
            REQUIRE(!(std::string("/this/file/should/not/exist/period.txt") == fp));

            // basename is "period.txt"
            REQUIRE(static_cast<std::string>(fp.filename()) == "period.txt");

            REQUIRE(fp.length() == 45);
            REQUIRE(fp.size() == 45);

            // all flags must be false
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt");
        }

        WHEN("and changing the path to something else as unexistant with assignment operator works too")
        {
            fp = "/this/is/another/path/changed/with/assignment/operator";

            THEN("path was replaced")
            {
                // retrieve the path
                std::string const p(fp);
                REQUIRE(p == "/this/is/another/path/changed/with/assignment/operator");

                REQUIRE(fp == "/this/is/another/path/changed/with/assignment/operator");
                REQUIRE("/this/is/another/path/changed/with/assignment/operator" == fp);
                REQUIRE(fp == std::string("/this/is/another/path/changed/with/assignment/operator"));
                REQUIRE(std::string("/this/is/another/path/changed/with/assignment/operator") == fp);

                REQUIRE(!(fp == "this/is/another/path/changed/with/assignment/operator"));
                REQUIRE(!("/this/is/another/path/chnged/with/assignment/operator" == fp));
                REQUIRE(!(fp == std::string("/this/is/another/path/changed/with/asignment/operator")));
                REQUIRE(!(std::string("/this/is/another/path/changed/with/assignment/oprator") == fp));

                // check basename
                REQUIRE(static_cast<std::string>(fp.filename()) == "operator");

                REQUIRE(fp.length() == 54);
                REQUIRE(fp.size() == 54);

                // all flags must be false
                REQUIRE(!fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                REQUIRE(ss.str() == "/this/is/another/path/changed/with/assignment/operator");
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

                REQUIRE(appended == "/this/file/really/should/not/exist/period.txt/correct/path");
                REQUIRE("/this/file/really/should/not/exist/period.txt/correct/path" == appended);
                REQUIRE(appended == std::string("/this/file/really/should/not/exist/period.txt/correct/path"));
                REQUIRE(std::string("/this/file/really/should/not/exist/period.txt/correct/path") == appended);

                REQUIRE(!(appended == "/this/file/really/not/exist/period.txt/correct/path"));
                REQUIRE(!("/this/file/really/should/not/exist/period/correct/path" == appended));
                REQUIRE(!(appended == std::string("/this/file/really/should/not/exist/period.txt/correct")));
                REQUIRE(!(std::string("/this/file/should/not/exist/period.txt/correct/path") == appended));

                REQUIRE(!(fp == path));
                REQUIRE(!(path == fp));

                {
                    zipios::FilePath equal("/this/file/really/should/not/exist/period.txt");
                    REQUIRE(fp == equal);
                    REQUIRE(equal == fp);
                }

                // still the same basename
                REQUIRE(static_cast<std::string>(appended.filename()) == "path");

                REQUIRE(appended.length() == 58);
                REQUIRE(appended.size() == 58);

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt/correct/path");
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

                REQUIRE(appended.length() == 63);
                REQUIRE(appended.size() == 63);

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt/relative/path.hpp");
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

                REQUIRE(appended.length() == 45);
                REQUIRE(appended.size() == 45);

                // still all flags are false
                REQUIRE(!appended.exists());
                REQUIRE(!appended.isRegular());
                REQUIRE(!appended.isDirectory());
                REQUIRE(!appended.isCharSpecial());
                REQUIRE(!appended.isBlockSpecial());
                REQUIRE(!appended.isSocket());
                REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt");
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

            REQUIRE(fp.length() == 0);
            REQUIRE(fp.size() == 0);

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            REQUIRE(ss.str().empty());
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

                REQUIRE(ee.length() == 0);
                REQUIRE(ee.size() == 0);

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

            std::stringstream ss;
            ss << ee;
            REQUIRE(ss.str().empty());
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

                REQUIRE(ee.length() == 23);
                REQUIRE(ee.size() == 23);

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "/this/is/a/regular/path");
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

                REQUIRE(ee.length() == 27);
                REQUIRE(ee.size() == 27);

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "this/is/a/relative/path.xml");
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

            REQUIRE(fp.length() == 33);
            REQUIRE(fp.size() == 33);

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt");
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

                REQUIRE(ee.length() == 33);
                REQUIRE(ee.size() == 33);

                // all flags must be false when empty
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt");
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

                REQUIRE(ee.length() == 56);
                REQUIRE(ee.size() == 56);

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt/this/is/a/regular/path");
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

                REQUIRE(ee.length() == 61);
                REQUIRE(ee.size() == 61);

                // all flags must be false
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt/this/is/a/relative/path.xml");
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

            REQUIRE(fp.length() == 23);
            REQUIRE(fp.size() == 23);

            // all flags must be false when empty
            // (because empty does not represent ".")
            REQUIRE(!fp.exists());
            REQUIRE(!fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            REQUIRE(ss.str() == "this/is/a/relative/path");
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

                REQUIRE(ee.length() == 45);
                REQUIRE(ee.size() == 45);

                // all flags must be false when empty
                REQUIRE(!ee.exists());
                REQUIRE(!ee.isRegular());
                REQUIRE(!ee.isDirectory());
                REQUIRE(!ee.isCharSpecial());
                REQUIRE(!ee.isBlockSpecial());
                REQUIRE(!ee.isSocket());
                REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                REQUIRE(ss.str() == "this/is/a/relative/path/add/this/with/a/slash");
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
            f << "This is a simple test file." << std::endl;
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

                REQUIRE(fp.length() == 17);
                REQUIRE(fp.size() == 17);
                REQUIRE(fp.fileSize() == 28);

                struct stat file_stats;
                REQUIRE(stat("filepath-test.txt", &file_stats) == 0);
                REQUIRE(fp.lastModificationTime() == file_stats.st_mtime);

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(fp.isRegular());
                REQUIRE(!fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                REQUIRE(ss.str() == "filepath-test.txt");
            }
        }

        unlink("filepath-test.txt");
    }

    GIVEN("an existing directory")
    {
        // make sure the directory is gone before re-creating it
        REQUIRE(system("rm -rf filepath-test") == 0);
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

                REQUIRE(fp.length() == 13);
                REQUIRE(fp.size() == 13);

                // all flags must be false
                REQUIRE(fp.exists());
                REQUIRE(!fp.isRegular());
                REQUIRE(fp.isDirectory());
                REQUIRE(!fp.isCharSpecial());
                REQUIRE(!fp.isBlockSpecial());
                REQUIRE(!fp.isSocket());
                REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                REQUIRE(ss.str() == "filepath-test");
            }
        }

        rmdir("filepath-test");
    }

    // todo: add tests for other file types (not extremely useful
    //       for a small zip library though...)
}


TEST_CASE("Test with regular files of various sizes", "[FilePath]")
{
    for(int i(0); i < 10; ++i)
    {
        // create a random file
        int const file_size(rand() % 100 + 20);
        {
            // create a file
            std::fstream f("filepath-test.txt", std::ios::out | std::ios::binary);
            for(int j(0); j < file_size; ++j)
            {
                char const c(rand());
                f << c;
            }
        }

        {
            zipios::FilePath fp("filepath-test.txt");

            // retrieve the path
            std::string const p(fp);
            REQUIRE(p == "filepath-test.txt");

            // basename is "period.txt"
            REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

            REQUIRE(fp.length() == 17);
            REQUIRE(fp.size() == 17);
            REQUIRE(fp.fileSize() == file_size);

            struct stat file_stats;
            REQUIRE(stat("filepath-test.txt", &file_stats) == 0);
            REQUIRE(fp.lastModificationTime() == file_stats.st_mtime);

            // all flags must be false
            REQUIRE(fp.exists());
            REQUIRE(fp.isRegular());
            REQUIRE(!fp.isDirectory());
            REQUIRE(!fp.isCharSpecial());
            REQUIRE(!fp.isBlockSpecial());
            REQUIRE(!fp.isSocket());
            REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            REQUIRE(ss.str() == "filepath-test.txt");
        }

        unlink("filepath-test.txt");
    }
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
