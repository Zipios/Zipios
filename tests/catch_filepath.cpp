/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (c) 2015-2022  Made to Order Software Corp.  All Rights Reserved

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

#include "catch_main.hpp"

#include <zipios/filepath.hpp>

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>


CATCH_SCENARIO("FilePath that does not represent a file on disk", "[FilePath]")
{
    CATCH_GIVEN("test a fantom file (path that \"cannot\" exists)")
    {
        zipios::FilePath fp("/this/file/really/should/not/exist/period.txt");

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            CATCH_REQUIRE(p == "/this/file/really/should/not/exist/period.txt");

            CATCH_REQUIRE(fp == "/this/file/really/should/not/exist/period.txt");
            CATCH_REQUIRE("/this/file/really/should/not/exist/period.txt" == fp);
            CATCH_REQUIRE(fp == std::string("/this/file/really/should/not/exist/period.txt"));
            CATCH_REQUIRE(std::string("/this/file/really/should/not/exist/period.txt") == fp);

            CATCH_REQUIRE(!(fp == "/this/file/really/should/not/exist/period"));
            CATCH_REQUIRE(!("/file/really/should/not/exist/period.txt" == fp));
            CATCH_REQUIRE(!(fp == std::string("/this/file/really/should/exist/period.txt")));
            CATCH_REQUIRE(!(std::string("/this/file/should/not/exist/period.txt") == fp));

            // basename is "period.txt"
            CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "period.txt");

            CATCH_REQUIRE(fp.length() == 45);
            CATCH_REQUIRE(fp.size() == 45);

            // all flags must be false
            CATCH_REQUIRE(!fp.exists());
            CATCH_REQUIRE(!fp.isRegular());
            CATCH_REQUIRE(!fp.isDirectory());
            CATCH_REQUIRE(!fp.isCharSpecial());
            CATCH_REQUIRE(!fp.isBlockSpecial());
            CATCH_REQUIRE(!fp.isSocket());
            CATCH_REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            CATCH_REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt");
        }
        CATCH_END_SECTION()

        CATCH_WHEN("and changing the path to something else as unexistant with assignment operator works too")
        {
            fp = "/this/is/another/path/changed/with/assignment/operator";

            CATCH_THEN("path was replaced")
            {
                // retrieve the path
                std::string const p(fp);
                CATCH_REQUIRE(p == "/this/is/another/path/changed/with/assignment/operator");

                CATCH_REQUIRE(fp == "/this/is/another/path/changed/with/assignment/operator");
                CATCH_REQUIRE("/this/is/another/path/changed/with/assignment/operator" == fp);
                CATCH_REQUIRE(fp == std::string("/this/is/another/path/changed/with/assignment/operator"));
                CATCH_REQUIRE(std::string("/this/is/another/path/changed/with/assignment/operator") == fp);

                CATCH_REQUIRE(!(fp == "this/is/another/path/changed/with/assignment/operator"));
                CATCH_REQUIRE(!("/this/is/another/path/chnged/with/assignment/operator" == fp));
                CATCH_REQUIRE(!(fp == std::string("/this/is/another/path/changed/with/asignment/operator")));
                CATCH_REQUIRE(!(std::string("/this/is/another/path/changed/with/assignment/oprator") == fp));

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "operator");

                CATCH_REQUIRE(fp.length() == 54);
                CATCH_REQUIRE(fp.size() == 54);

                // all flags must be false
                CATCH_REQUIRE(!fp.exists());
                CATCH_REQUIRE(!fp.isRegular());
                CATCH_REQUIRE(!fp.isDirectory());
                CATCH_REQUIRE(!fp.isCharSpecial());
                CATCH_REQUIRE(!fp.isBlockSpecial());
                CATCH_REQUIRE(!fp.isSocket());
                CATCH_REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                CATCH_REQUIRE(ss.str() == "/this/is/another/path/changed/with/assignment/operator");
            }
        }

        CATCH_WHEN("still \"broken\" when appending another part (full path)")
        {
            zipios::FilePath path("/correct/path");

            zipios::FilePath appended(fp + path);

            CATCH_THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                CATCH_REQUIRE(p == "/this/file/really/should/not/exist/period.txt/correct/path");

                CATCH_REQUIRE(appended == "/this/file/really/should/not/exist/period.txt/correct/path");
                CATCH_REQUIRE("/this/file/really/should/not/exist/period.txt/correct/path" == appended);
                CATCH_REQUIRE(appended == std::string("/this/file/really/should/not/exist/period.txt/correct/path"));
                CATCH_REQUIRE(std::string("/this/file/really/should/not/exist/period.txt/correct/path") == appended);

                CATCH_REQUIRE(!(appended == "/this/file/really/not/exist/period.txt/correct/path"));
                CATCH_REQUIRE(!("/this/file/really/should/not/exist/period/correct/path" == appended));
                CATCH_REQUIRE(!(appended == std::string("/this/file/really/should/not/exist/period.txt/correct")));
                CATCH_REQUIRE(!(std::string("/this/file/should/not/exist/period.txt/correct/path") == appended));

                CATCH_REQUIRE(!(fp == path));
                CATCH_REQUIRE(!(path == fp));

                {
                    zipios::FilePath equal("/this/file/really/should/not/exist/period.txt");
                    CATCH_REQUIRE(fp == equal);
                    CATCH_REQUIRE(equal == fp);
                }

                // still the same basename
                CATCH_REQUIRE(static_cast<std::string>(appended.filename()) == "path");

                CATCH_REQUIRE(appended.length() == 58);
                CATCH_REQUIRE(appended.size() == 58);

                // still all flags are false
                CATCH_REQUIRE(!appended.exists());
                CATCH_REQUIRE(!appended.isRegular());
                CATCH_REQUIRE(!appended.isDirectory());
                CATCH_REQUIRE(!appended.isCharSpecial());
                CATCH_REQUIRE(!appended.isBlockSpecial());
                CATCH_REQUIRE(!appended.isSocket());
                CATCH_REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                CATCH_REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt/correct/path");
            }
        }

        CATCH_WHEN("still \"broken\" when appending another part (relative)")
        {
            zipios::FilePath path("relative/path.hpp");

            // append to the left...
            zipios::FilePath appended(fp + path);

            CATCH_THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                CATCH_REQUIRE(p == "/this/file/really/should/not/exist/period.txt/relative/path.hpp");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(appended.filename()) == "path.hpp");

                CATCH_REQUIRE(appended.length() == 63);
                CATCH_REQUIRE(appended.size() == 63);

                // still all flags are false
                CATCH_REQUIRE(!appended.exists());
                CATCH_REQUIRE(!appended.isRegular());
                CATCH_REQUIRE(!appended.isDirectory());
                CATCH_REQUIRE(!appended.isCharSpecial());
                CATCH_REQUIRE(!appended.isBlockSpecial());
                CATCH_REQUIRE(!appended.isSocket());
                CATCH_REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                CATCH_REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt/relative/path.hpp");
            }
        }

        CATCH_WHEN("still \"broken\" when appending another part (empty)")
        {
            zipios::FilePath path("");

            // append to the left...
            zipios::FilePath appended(fp + path);

            CATCH_THEN("path changed")
            {
                // retrieve the concatenated path
                std::string const p(appended);
                CATCH_REQUIRE(p == "/this/file/really/should/not/exist/period.txt");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(appended.filename()) == "period.txt");

                CATCH_REQUIRE(appended.length() == 45);
                CATCH_REQUIRE(appended.size() == 45);

                // still all flags are false
                CATCH_REQUIRE(!appended.exists());
                CATCH_REQUIRE(!appended.isRegular());
                CATCH_REQUIRE(!appended.isDirectory());
                CATCH_REQUIRE(!appended.isCharSpecial());
                CATCH_REQUIRE(!appended.isBlockSpecial());
                CATCH_REQUIRE(!appended.isSocket());
                CATCH_REQUIRE(!appended.isFifo());

                std::stringstream ss;
                ss << appended;
                CATCH_REQUIRE(ss.str() == "/this/file/really/should/not/exist/period.txt");
            }
        }
    }

    CATCH_GIVEN("an empty path")
    {
        zipios::FilePath fp;

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            CATCH_REQUIRE(p == "");

            // check basename
            CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "");

            CATCH_REQUIRE(fp.length() == 0);
            CATCH_REQUIRE(fp.size() == 0);

            // all flags must be false when empty
            // (because empty does not represent ".")
            CATCH_REQUIRE(!fp.exists());
            CATCH_REQUIRE(!fp.isRegular());
            CATCH_REQUIRE(!fp.isDirectory());
            CATCH_REQUIRE(!fp.isCharSpecial());
            CATCH_REQUIRE(!fp.isBlockSpecial());
            CATCH_REQUIRE(!fp.isSocket());
            CATCH_REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            CATCH_REQUIRE(ss.str().empty());
        }
        CATCH_END_SECTION()

        CATCH_WHEN("we can concatenate another empty path to it")
        {
            zipios::FilePath ep;

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("file name is still empty")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "");

                CATCH_REQUIRE(ee.length() == 0);
                CATCH_REQUIRE(ee.size() == 0);

                // all flags must be false when empty
                // (because empty does not represent ".")
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());
            }

            std::stringstream ss;
            ss << ee;
            CATCH_REQUIRE(ss.str().empty());
        }

        CATCH_WHEN("we can concatenate a full regular path to it")
        {
            zipios::FilePath ep("/this/is/a/regular/path");

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("new path is equal to the concatenated path")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "/this/is/a/regular/path");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "path");

                CATCH_REQUIRE(ee.length() == 23);
                CATCH_REQUIRE(ee.size() == 23);

                // all flags must be false
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "/this/is/a/regular/path");
            }
        }

        CATCH_WHEN("we can concatenate a relative path to it")
        {
            zipios::FilePath ep("this/is/a/relative/path.xml");

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("concatenated path is the added path")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "this/is/a/relative/path.xml");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "path.xml");

                CATCH_REQUIRE(ee.length() == 27);
                CATCH_REQUIRE(ee.size() == 27);

                // all flags must be false
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "this/is/a/relative/path.xml");
            }
        }
    }

    CATCH_GIVEN("a fantom relative path")
    {
        zipios::FilePath fp("this/is/a/relative/path/file1.txt");

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            CATCH_REQUIRE(p == "this/is/a/relative/path/file1.txt");

            // check basename
            CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "file1.txt");

            CATCH_REQUIRE(fp.length() == 33);
            CATCH_REQUIRE(fp.size() == 33);

            // all flags must be false when empty
            // (because empty does not represent ".")
            CATCH_REQUIRE(!fp.exists());
            CATCH_REQUIRE(!fp.isRegular());
            CATCH_REQUIRE(!fp.isDirectory());
            CATCH_REQUIRE(!fp.isCharSpecial());
            CATCH_REQUIRE(!fp.isBlockSpecial());
            CATCH_REQUIRE(!fp.isSocket());
            CATCH_REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            CATCH_REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt");
        }
        CATCH_END_SECTION()

        CATCH_WHEN("we can concatenate an empty path to it")
        {
            zipios::FilePath ep;

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("the result is the same as the left hand-side")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "this/is/a/relative/path/file1.txt");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "file1.txt");

                CATCH_REQUIRE(ee.length() == 33);
                CATCH_REQUIRE(ee.size() == 33);

                // all flags must be false when empty
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt");
            }
        }

        CATCH_WHEN("we can concatenate a full regular path to it")
        {
            zipios::FilePath ep("/this/is/a/regular/path");

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("path is the resulting concatenation with a slash at the end")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "this/is/a/relative/path/file1.txt/this/is/a/regular/path");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "path");

                CATCH_REQUIRE(ee.length() == 56);
                CATCH_REQUIRE(ee.size() == 56);

                // all flags must be false
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt/this/is/a/regular/path");
            }
        }

        CATCH_WHEN("we can concatenate a relative path to it")
        {
            zipios::FilePath ep("this/is/a/relative/path.xml");

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("the path changed")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "this/is/a/relative/path/file1.txt/this/is/a/relative/path.xml");

                // basename is "period.txt"
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "path.xml");

                CATCH_REQUIRE(ee.length() == 61);
                CATCH_REQUIRE(ee.size() == 61);

                // all flags must be false
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "this/is/a/relative/path/file1.txt/this/is/a/relative/path.xml");
            }
        }
    }

    CATCH_GIVEN("a fantom path that ends with /")
    {
        zipios::FilePath fp("this/is/a/relative/path/");

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            // retrieve the path
            std::string const p(fp);
            CATCH_REQUIRE(p == "this/is/a/relative/path");

            // check basename
            CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "path");

            CATCH_REQUIRE(fp.length() == 23);
            CATCH_REQUIRE(fp.size() == 23);

            // all flags must be false when empty
            // (because empty does not represent ".")
            CATCH_REQUIRE(!fp.exists());
            CATCH_REQUIRE(!fp.isRegular());
            CATCH_REQUIRE(!fp.isDirectory());
            CATCH_REQUIRE(!fp.isCharSpecial());
            CATCH_REQUIRE(!fp.isBlockSpecial());
            CATCH_REQUIRE(!fp.isSocket());
            CATCH_REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            CATCH_REQUIRE(ss.str() == "this/is/a/relative/path");
        }
        CATCH_END_SECTION()

        CATCH_WHEN("we can concatenate another path, it also prune the /")
        {
            zipios::FilePath ep("add/this/with/a/slash/");

            zipios::FilePath ee(fp + ep);

            CATCH_THEN("the result is as expected without the slash")
            {
                // retrieve the path
                std::string const p(ee);
                CATCH_REQUIRE(p == "this/is/a/relative/path/add/this/with/a/slash");

                // check basename
                CATCH_REQUIRE(static_cast<std::string>(ee.filename()) == "slash");

                CATCH_REQUIRE(ee.length() == 45);
                CATCH_REQUIRE(ee.size() == 45);

                // all flags must be false when empty
                CATCH_REQUIRE(!ee.exists());
                CATCH_REQUIRE(!ee.isRegular());
                CATCH_REQUIRE(!ee.isDirectory());
                CATCH_REQUIRE(!ee.isCharSpecial());
                CATCH_REQUIRE(!ee.isBlockSpecial());
                CATCH_REQUIRE(!ee.isSocket());
                CATCH_REQUIRE(!ee.isFifo());

                std::stringstream ss;
                ss << ee;
                CATCH_REQUIRE(ss.str() == "this/is/a/relative/path/add/this/with/a/slash");
            }
        }
    }
}


CATCH_SCENARIO("FilePath against existing files on disk", "[FilePath]")
{
    zipios_test::safe_chdir cwd(SNAP_CATCH2_NAMESPACE::g_tmp_dir());

    CATCH_GIVEN("an existing file")
    {
        zipios_test::auto_unlink_t auto_unlink("filepath-test.txt", true);

        {
            // create a file
            std::ofstream f("filepath-test.txt", std::ios::out | std::ios::binary);
            f << "This is a simple test file." << std::endl;
        }

        CATCH_WHEN("creating a FilePath object")
        {
            zipios::FilePath fp("filepath-test.txt");

            CATCH_THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                CATCH_REQUIRE(p == "filepath-test.txt");

                // basename is "period.txt"
                CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

                CATCH_REQUIRE(fp.length() == 17);
                CATCH_REQUIRE(fp.size() == 17);
                CATCH_REQUIRE(fp.fileSize() == 28);

                struct stat file_stats;
                CATCH_REQUIRE(stat("filepath-test.txt", &file_stats) == 0);
                CATCH_REQUIRE(fp.lastModificationTime() == file_stats.st_mtime);

                // all flags must be false
                CATCH_REQUIRE(fp.exists());
                CATCH_REQUIRE(fp.isRegular());
                CATCH_REQUIRE(!fp.isDirectory());
                CATCH_REQUIRE(!fp.isCharSpecial());
                CATCH_REQUIRE(!fp.isBlockSpecial());
                CATCH_REQUIRE(!fp.isSocket());
                CATCH_REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                CATCH_REQUIRE(ss.str() == "filepath-test.txt");
            }
        }
    }

    CATCH_GIVEN("an existing directory")
    {
        zipios_test::auto_unlink_t auto_unlink("filepath-test", true);

        // create a directory
        CATCH_REQUIRE(mkdir("filepath-test", 0777) == 0);

        CATCH_WHEN("creating a FilePath object")
        {
            zipios::FilePath fp("filepath-test");

            CATCH_THEN("is found")
            {
                // retrieve the path
                std::string const p(fp);
                CATCH_REQUIRE(p == "filepath-test");

                // basename is "period.txt"
                CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test");

                CATCH_REQUIRE(fp.length() == 13);
                CATCH_REQUIRE(fp.size() == 13);

                // all flags must be false
                CATCH_REQUIRE(fp.exists());
                CATCH_REQUIRE(!fp.isRegular());
                CATCH_REQUIRE(fp.isDirectory());
                CATCH_REQUIRE(!fp.isCharSpecial());
                CATCH_REQUIRE(!fp.isBlockSpecial());
                CATCH_REQUIRE(!fp.isSocket());
                CATCH_REQUIRE(!fp.isFifo());

                std::stringstream ss;
                ss << fp;
                CATCH_REQUIRE(ss.str() == "filepath-test");
            }
        }
    }

    // todo: add tests for other file types (not extremely useful
    //       for a small zip library though...)
}


CATCH_TEST_CASE("Test with regular files of various sizes", "[FilePath]")
{
    zipios_test::safe_chdir cwd(SNAP_CATCH2_NAMESPACE::g_tmp_dir());

    for(int i(0); i < 10; ++i)
    {
        zipios_test::auto_unlink_t auto_unlink("filepath-test.txt", true);

        // create a random file
        int const file_size(rand() % 100 + 20);
        {
            // create a file
            std::ofstream f("filepath-test.txt", std::ios::out | std::ios::binary);
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
            CATCH_REQUIRE(p == "filepath-test.txt");

            // basename is "period.txt"
            CATCH_REQUIRE(static_cast<std::string>(fp.filename()) == "filepath-test.txt");

            CATCH_REQUIRE(fp.length() == 17);
            CATCH_REQUIRE(fp.size() == 17);
            CATCH_REQUIRE(fp.fileSize() == static_cast<std::size_t>(file_size));

            struct stat file_stats;
            CATCH_REQUIRE(stat("filepath-test.txt", &file_stats) == 0);
            CATCH_REQUIRE(fp.lastModificationTime() == file_stats.st_mtime);

            // all flags must be false
            CATCH_REQUIRE(fp.exists());
            CATCH_REQUIRE(fp.isRegular());
            CATCH_REQUIRE(!fp.isDirectory());
            CATCH_REQUIRE(!fp.isCharSpecial());
            CATCH_REQUIRE(!fp.isBlockSpecial());
            CATCH_REQUIRE(!fp.isSocket());
            CATCH_REQUIRE(!fp.isFifo());

            std::stringstream ss;
            ss << fp;
            CATCH_REQUIRE(ss.str() == "filepath-test.txt");
        }
    }
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
