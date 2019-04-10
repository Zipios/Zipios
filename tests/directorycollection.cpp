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
 * Zipios unit tests for the DirectoryCollection class.
 */

#include "tests.hpp"

#include "zipios/directorycollection.hpp"
#include "zipios/zipiosexceptions.hpp"
#include "zipios/dosdatetime.hpp"

#include <fstream>
#include <memory>
#include <vector>

#include <unistd.h>
#include <string.h>





SCENARIO("DirectoryCollection with invalid paths", "[DirectoryCollection] [FileCollection]")
{
    GIVEN("an empty directory collection")
    {
        zipios::DirectoryCollection dc;

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE(dc.isValid());
            REQUIRE_FALSE(dc.entries().empty());
            REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(dc.getName() == "-");   // default name is "-"
            REQUIRE(dc.size() == 1);
            dc.mustBeValid();

            zipios::DirectoryCollection copy_constructor(dc);
            REQUIRE(copy_constructor.isValid());
            REQUIRE_FALSE(copy_constructor.entries().empty());
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_constructor.getName() == "-");   // copy name as is
            REQUIRE(copy_constructor.size() == 1);
            copy_constructor.mustBeValid();

            zipios::DirectoryCollection copy_assignment;
            copy_assignment = dc;
            REQUIRE(copy_assignment.isValid());
            REQUIRE_FALSE(copy_assignment.entries().empty());
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_assignment.getName() == "-");   // copy name as is
            REQUIRE(copy_assignment.size() == 1);
            copy_assignment.mustBeValid();

            zipios::FileCollection::pointer_t clone(dc.clone());
            REQUIRE(dynamic_cast<zipios::DirectoryCollection *>(clone.get()));
            REQUIRE(clone->isValid());
            REQUIRE_FALSE(clone->entries().empty());
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(clone->getName() == "-");   // copy name as is
            REQUIRE(clone->size() == 1);
            clone->mustBeValid();
        }

        WHEN("closing the directory")
        {
            dc.close();

            THEN("it is still the same")
            {
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName(), zipios::InvalidStateException);   // default name is "-"
                REQUIRE_THROWS_AS(dc.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);

                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE_FALSE(copy_constructor.isValid());
                REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_constructor.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);

                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE_FALSE(copy_assignment.isValid());
                REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_assignment.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);

                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE(dynamic_cast<zipios::DirectoryCollection *>(clone.get()));
                REQUIRE_FALSE(clone->isValid());
                REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(clone->size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
            }
        }
    }
}


TEST_CASE("DirectoryCollection with a valid file, but not a directory", "[DirectoryCollection] [FileCollection]")
{
    // create a small random file
    int const file_size(rand() % 100 + 20);
    {
        // create a file
        std::ofstream f("directory-collection-test.txt", std::ios::out | std::ios::binary);
        for(int j(0); j < file_size; ++j)
        {
            char const c(rand());
            f << c;
        }
    }

    SECTION("verify that the object looks as expected in recursive mode")
    {
        // recursive reading
        zipios::DirectoryCollection dc("directory-collection-test.txt", true);

        // not valid because it is not a directory
        REQUIRE(dc.isValid());
        REQUIRE_FALSE(dc.entries().empty());
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE(dc.getName() == "directory-collection-test.txt");
        REQUIRE(dc.size() == 1);
        dc.mustBeValid();

        dc.close();

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
    }

    SECTION("verify that the object looks as expected in non-recursive mode")
    {
        // recursive reading
        zipios::DirectoryCollection dc("directory-collection-test.txt", false);

        // not valid because it is not a directory
        REQUIRE(dc.isValid());
        REQUIRE_FALSE(dc.entries().empty());
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE(dc.getName() == "directory-collection-test.txt");
        REQUIRE(dc.size() == 1);
        dc.mustBeValid();

        dc.close();

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
    }

    unlink("directory-collection-test.txt");
}


TEST_CASE("DirectoryCollection with valid trees of files", "[DirectoryCollection] [FileCollection]")
{
    for(int i(0); i < 6; ++i)
    {
        // create a directory tree starting in "tree"
        REQUIRE(system("rm -rf tree") == 0); // clean up, just in case
        size_t const start_count(rand() % 40 + 80);
        zipios_test::file_t tree(zipios_test::file_t::type_t::DIRECTORY, start_count, "tree");

        {
            zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", true));

            {
                REQUIRE(dc.isValid());
                REQUIRE_FALSE(dc.entries().empty());
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(dc.getName() == "tree");
                REQUIRE(dc.size() == tree.size());
                dc.mustBeValid(); // not throwing

                {
                    std::ostringstream expected_output;
                    expected_output << "collection 'tree' {";
                    zipios::FileEntry::vector_t v(dc.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        if(it != v.begin())
                        {
                            expected_output << ", ";
                        }
                        expected_output << entry->getName();

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                    expected_output << "}";

                    std::ostringstream output;
                    output << dc;
                    REQUIRE(expected_output.str() == output.str());
                }
            }

            {
                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE(copy_constructor.isValid());
                REQUIRE_FALSE(copy_constructor.entries().empty());
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_constructor.getName() == "tree");
                REQUIRE(copy_constructor.size() == tree.size());
                copy_constructor.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_constructor.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            {
                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE(copy_assignment.isValid());
                REQUIRE_FALSE(copy_assignment.entries().empty());
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_assignment.getName() == "tree");   // copy name as is
                REQUIRE(copy_assignment.size() == tree.size());
                copy_assignment.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_assignment.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            {
                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE(dynamic_cast<zipios::DirectoryCollection *>(clone.get()));
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "tree");
                REQUIRE(clone->size() == tree.size());
                clone->mustBeValid();

                {
                    zipios::FileEntry::vector_t v(clone->entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            {
                // this one is recursive so we get ALL the files in
                // the collection
                zipios_test::file_t::filenames_t all_files(tree.get_all_filenames());

                for(auto it(all_files.begin()); it != all_files.end(); ++it)
                {
                    std::string const name(*it);

                    if(!name.empty() && name.back() == '/')  // Directory?
                    {
                        // directories cannot be attached to an istream
                        zipios::DirectoryCollection::stream_pointer_t is1(dc.getInputStream(name));
                        REQUIRE(!is1);

                        // also test without the ending '/', just in case
                        zipios::DirectoryCollection::stream_pointer_t is2(dc.getInputStream(name.substr(0, name.length() - 1)));
                        REQUIRE(!is2);

                        // now also test the getEntry() which works with MATCH
                        // or IGNORE -- prove it!
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE(entry_match);

                        std::string::size_type pos(name.rfind('/', name.length() - 2));
                        if(pos == std::string::npos)
                        {
                            pos = 0;
                        }
                        else
                        {
                            ++pos;
                        }
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(entry_ignore);
                    }
                    else
                    {
                        // files must all work and we can read them and
                        // compare with the "real thing" and it is equal
                        zipios::FileCollection::stream_pointer_t is(dc.getInputStream(name));
                        REQUIRE(is);

                        std::ifstream in(name, std::ios::in | std::ios::binary);

                        while(in && *is)
                        {
                            char buf1[BUFSIZ], buf2[BUFSIZ];

                            in.read(buf1, sizeof(buf1));
                            std::streamsize sz1(in.gcount());

                            is->read(buf2, sizeof(buf2));
                            std::streamsize sz2(is->gcount());

                            REQUIRE(sz1 == sz2);
                            REQUIRE(memcmp(buf1, buf2, sz1) == 0);
                        }

                        REQUIRE(!in);
                        REQUIRE(!*is);

                        // now also test the getEntry() which works with MATCH
                        // or IGNORE -- prove it!
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE(entry_match);

                        std::string::size_type pos(name.rfind('/'));
                        if(pos == std::string::npos)
                        {
                            pos = 0; // LCOV_EXCL_LINE
                        }
                        else
                        {
                            ++pos;
                        }
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(entry_ignore);
                    }
                }
            }

            {
                dc.close();

                // not valid because it was closed
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
            }
        }

        {
            zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", false));

            {
                REQUIRE(dc.isValid());
                REQUIRE_FALSE(dc.entries().empty());
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(dc.getName() == "tree");
                REQUIRE(dc.size() == tree.children().size() + 1);
                dc.mustBeValid(); // not throwing

                {
                    zipios::FileEntry::vector_t v(dc.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE(copy_constructor.isValid());
                REQUIRE_FALSE(copy_constructor.entries().empty());
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_constructor.getName() == "tree");
                REQUIRE(copy_constructor.size() == tree.children().size() + 1);
                copy_constructor.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_constructor.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE(copy_assignment.isValid());
                REQUIRE_FALSE(copy_assignment.entries().empty());
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_assignment.getName() == "tree");   // copy name as is
                REQUIRE(copy_assignment.size() == tree.children().size() + 1);
                copy_assignment.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_assignment.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE(dynamic_cast<zipios::DirectoryCollection *>(clone.get()));
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "tree");
                REQUIRE(clone->size() == tree.children().size() + 1);
                clone->mustBeValid();

                {
                    zipios::FileEntry::vector_t v(clone->entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        zipios_test::file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != zipios_test::file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        zipios::DOSDateTime dt;
                        dt.setUnixTimestamp(file_stats.st_mtime);
                        REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
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

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            {
                // in this case the DirectoryCollection is not recursive
                // so only the top children are available
                zipios_test::file_t::vector_t all_files(tree.children());

                for(auto it(all_files.begin()); it != all_files.end(); ++it)
                {
                    zipios_test::file_t::pointer_t f(*it);

                    if(f->type() == zipios_test::file_t::type_t::DIRECTORY)  // Directory?
                    {
                        // directories cannot be attached to an istream
                        zipios::DirectoryCollection::stream_pointer_t is1(dc.getInputStream(f->filename()));
                        REQUIRE(!is1);
                    }
                    else
                    {
                        // WARNING: in this case we get a "bare" filename
                        //          from the file_t object
                        //
                        // files must all work and we can read them and
                        // compare with the "real thing" and it is equal

                        // "tree/" missing...
                        zipios::DirectoryCollection::stream_pointer_t bad_is(dc.getInputStream(f->filename(), zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE_FALSE(bad_is);

                        // in this case we ignore to see that we can indeed
                        // get the file... unfortunately it could be another
                        // file with the same name (although unlikely, it
                        // is very much possible)
                        zipios::DirectoryCollection::stream_pointer_t is(dc.getInputStream(f->filename(), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(is);

                        // So here we verify that it find the correct
                        // entry, if so then we can compare the files
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry("tree/" + f->filename(), zipios::FileCollection::MatchPath::MATCH));
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(f->filename(), zipios::FileCollection::MatchPath::IGNORE));
                        if(entry_match == entry_ignore)
                        {
                            std::ifstream in("tree/" + f->filename(), std::ios::in | std::ios::binary);

                            while(in && *is)
                            {
                                char buf1[BUFSIZ], buf2[BUFSIZ];

                                in.read(buf1, sizeof(buf1));
                                std::streamsize sz1(in.gcount());

                                is->read(buf2, sizeof(buf2));
                                std::streamsize sz2(is->gcount());

                                REQUIRE(sz1 == sz2);
                                REQUIRE(memcmp(buf1, buf2, sz1) == 0);
                            }

                            REQUIRE(!in);
                            REQUIRE(!*is);
                        }
                    }
                }
            }

            {
                dc.close();

                // not valid because it is not a directory
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
            }
        }
    }
}


TEST_CASE("DirectoryCollection with an existing directory that gets deleted", "[DirectoryCollection] [FileCollection]")
{
    // create a directory
    REQUIRE(system("rm -rf tree") == 0); // clean up, just in case
    REQUIRE(mkdir("tree", 0777) == 0);

    // the initialization works as expected!
    zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", false));
    REQUIRE(dc.isValid());

    // now we delete that directory!
    REQUIRE(rmdir("tree") == 0);

    // attempt the get the size, it throws
    REQUIRE_THROWS_AS(dc.size(), zipios::IOException);
    REQUIRE_FALSE(dc.isValid());
}


TEST_CASE("DirectoryCollection with an empty directory", "[DirectoryCollection] [FileCollection]")
{
    // create a directory
    REQUIRE(system("rm -rf tree") == 0); // clean up, just in case
    REQUIRE(mkdir("tree", 0777) == 0);

    SECTION("verify that the object looks as expected")
    {
        zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", true));

        REQUIRE(dc.isValid());
        REQUIRE_FALSE(dc.entries().empty());
        REQUIRE(dc.entries().size() == 1);
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
        REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
        REQUIRE(dc.getName() == "tree");
        REQUIRE(dc.size() == 1);
        dc.mustBeValid(); // not throwing

        {
            zipios::FileEntry::vector_t v(dc.entries());
            for(auto it(v.begin()); it != v.end(); ++it)
            {
                zipios::FileEntry::pointer_t entry(*it);

                // verify that our tree knows about this file
                REQUIRE(entry->getName() == "tree");

                struct stat file_stats;
                REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                REQUIRE((*it)->getComment().empty());
                REQUIRE((*it)->getCompressedSize() == 0);
                REQUIRE((*it)->getCrc() == 0);
                REQUIRE((*it)->getEntryOffset() == 0);
                REQUIRE((*it)->getExtra().empty());
                REQUIRE((*it)->getHeaderSize() == 0);
                REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE((*it)->getName() == "tree");
                REQUIRE((*it)->getFileName() == "tree");
                zipios::DOSDateTime t;
                t.setUnixTimestamp(file_stats.st_mtime);
                REQUIRE((*it)->getTime() == t.getDOSDateTime());
                REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE((*it)->hasCrc());
                REQUIRE((*it)->isDirectory());
                REQUIRE((*it)->getSize() == 0); // size is zero for directories
                REQUIRE((*it)->isValid());
                REQUIRE((*it)->toString() == "tree (directory)");

                // using the << operator to a stream we get the toString() value
                std::ostringstream output;
                output << **it;
                REQUIRE(output.str() == "tree (directory)");

                REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
            }
        }
    }

    rmdir("tree");
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
