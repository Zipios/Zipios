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
 * Zipios++ unit tests for the ZipFile class.
 */

#include "catch_tests.hpp"

#include "zipios++/zipfile.hpp"
#include "zipios++/zipiosexceptions.hpp"

#include "src/dostime.h"

//#include <fstream>
//#include <memory>
//#include <vector>
//
//#include <unistd.h>
//#include <string.h>





TEST_CASE("An Empty ZipFile", "[ZipFile] [FileCollection]")
{
    zipios::ZipFile zf;

    REQUIRE_FALSE(zf.isValid());
    REQUIRE_THROWS_AS(zf.entries().empty(), zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.getName() == "-", zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.size() == 0, zipios::InvalidStateException);
    REQUIRE_THROWS_AS(zf.mustBeValid(), zipios::InvalidStateException); // not throwing
}


SCENARIO("ZipFile with a working", "[ZipFile] [FileCollection]")
{
    GIVEN("a tree directory")
    {
        system("rm -rf tree"); // clean up, just in case
        size_t const start_count(rand() % 40 + 80);
        zipios_test::file_t tree(zipios_test::file_t::type_t::DIRECTORY, start_count, "tree");
        zipios_test::auto_unlink_t remove_zip("tree.zip");
        system("zip -r tree.zip tree >/dev/null");


        // first, check that the object is setup as expected
        WHEN("we load the zip file")
        {
            zipios::ZipFile zf("tree.zip");

            THEN("it is valid and includes all the files in the tree")
            {
                REQUIRE(zf.isValid());
                REQUIRE_FALSE(zf.entries().empty());
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(zf.getName() == "tree.zip");
                REQUIRE(zf.size() == tree.size());
                zf.mustBeValid(); // not throwing

                zipios::FileEntry::vector_t v(zf.entries());
                for(auto it(v.begin()); it != v.end(); ++it)
                {
                    zipios::FileEntry::pointer_t entry(*it);

                    // verify that our tree knows about this file
                    zipios_test::file_t::type_t t(tree.find(entry->getName()));
                    REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                    struct stat file_stats;
                    REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                    REQUIRE((*it)->getComment().empty());
                    //REQUIRE((*it)->getCompressedSize() == (*it)->getSize()); -- not too sure how we could verify this size in this case
                    //REQUIRE((*it)->getCrc() == ...); -- not too sure how to compute that right now, but once we have it we'll test it
                    //REQUIRE((*it)->getEntryOffset() == ...); -- that's also difficult to test
                    //REQUIRE((*it)->getExtra().empty());
                    //REQUIRE((*it)->getHeaderSize() == 0); -- the header size varies
                    if((*it)->getMethod() == zipios::StorageMethod::STORED)
                    {
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                    }
                    else
                    {
                        //REQUIRE((*it)->getCompressedSize() < (*it)->getSize()); // this should be always be true with the zip tool
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                    size_t ut(dos2unixtime(unix2dostime(file_stats.st_mtime)));
                    REQUIRE((*it)->getUnixTime() == ut);
                    REQUIRE_FALSE((*it)->hasCrc());
                    if(t == zipios_test::file_t::type_t::DIRECTORY)
                    {
                        REQUIRE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == 0); // size is zero for directories
                    }
                    else
                    {
                        REQUIRE_FALSE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == file_stats.st_size);
                    }
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }
        }
    }
}


// vim: ts=4 sw=4 et
