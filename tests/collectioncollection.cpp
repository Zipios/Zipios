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
 * Zipios unit tests for the CollectionCollection class.
 */

#include "tests.hpp"

#include "zipios/collectioncollection.hpp"
#include "zipios/directorycollection.hpp"
#include "zipios/zipiosexceptions.hpp"

#include <fstream>

#include <string.h>




SCENARIO("CollectionCollection with various tests", "[DirectoryCollection] [FileCollection]")
{
    GIVEN("an empty collection collection")
    {
        zipios::CollectionCollection cc;

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // an empty CollectionCollection is valid
            REQUIRE(cc.isValid());
            REQUIRE(cc.entries().empty());
            REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(cc.getName() == "-");   // default name is "-"
            REQUIRE(cc.size() == 0);
            cc.mustBeValid();

            zipios::CollectionCollection copy_constructor(cc);
            REQUIRE(copy_constructor.isValid());
            REQUIRE(copy_constructor.entries().empty());
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_constructor.getName() == "-");   // copy name as is
            REQUIRE(copy_constructor.size() == 0);
            copy_constructor.mustBeValid();

            zipios::CollectionCollection copy_assignment;
            copy_assignment = cc;
            REQUIRE(copy_assignment.isValid());
            REQUIRE(copy_assignment.entries().empty());
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_assignment.getName() == "-");   // copy name as is
            REQUIRE(copy_assignment.size() == 0);
            copy_assignment.mustBeValid();

            zipios::FileCollection::pointer_t clone(cc.clone());
            REQUIRE(dynamic_cast<zipios::CollectionCollection *>(clone.get()));
            REQUIRE(clone->isValid());
            REQUIRE(clone->entries().empty());
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(clone->getName() == "-");   // copy name as is
            REQUIRE(clone->size() == 0);
            clone->mustBeValid();

            // this works!
            cc.addCollection(clone);

            // however, adding a null pointer fails dramatically
            zipios::FileCollection::pointer_t null_pointer;
            REQUIRE_THROWS_AS(cc.addCollection(null_pointer), zipios::InvalidException);
        }

        WHEN("we add an existing directory collection")
        {
            // adding a collection to itself fails
            REQUIRE_FALSE(cc.addCollection(cc));

            // create a directory tree starting in "tree"
            REQUIRE(system("rm -rf tree") != -1); // clean up, just in case
            size_t start_count(rand() % 10 + 10); // pretty small, no need to waste too much time here
            zipios_test::file_t tree(zipios_test::file_t::type_t::DIRECTORY, start_count, "tree");
            zipios::DirectoryCollection dc("tree", true);
            REQUIRE(cc.addCollection(dc));

            THEN("it is valid and we can find all the files")
            {
                REQUIRE(cc.isValid());
                REQUIRE_FALSE(cc.entries().empty());
                REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(cc.getName() == "-");   // default name is "-"
                REQUIRE(cc.size() == tree.size());
                cc.mustBeValid();

                SECTION("test the CollectionCollection::getInputStream() for each file in the tree (original)")
                {
                    // this DirectoryCollection is recursive so we get ALL
                    // the files in the collection
                    zipios_test::file_t::filenames_t all_files(tree.get_all_filenames());

                    for(auto it(all_files.begin()); it != all_files.end(); ++it)
                    {
                        std::string const name(*it);

                        if(!name.empty() && name.back() == '/')  // Directory?
                        {
                            // directories cannot be attached to an istream
                            zipios::DirectoryCollection::stream_pointer_t is1a(dc.getInputStream(name));
                            REQUIRE(!is1a);
                            zipios::CollectionCollection::stream_pointer_t is1b(cc.getInputStream(name));
                            REQUIRE(!is1b);

                            // also test without the ending '/', just in case
                            zipios::DirectoryCollection::stream_pointer_t is2a(dc.getInputStream(name.substr(0, name.length() - 1)));
                            REQUIRE(!is2a);
                            zipios::CollectionCollection::stream_pointer_t is2b(cc.getInputStream(name.substr(0, name.length() - 1)));
                            REQUIRE(!is2b);

                            // now also test the getEntry() which works with MATCH
                            // or IGNORE -- prove it!
                            //
                            zipios::FileEntry::pointer_t entry_match_a(dc.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match_a);
                            zipios::FileEntry::pointer_t entry_match_b(cc.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match_b);

                            std::string::size_type pos(name.rfind('/', name.length() - 2));
                            if(pos == std::string::npos)
                            {
                                pos = 0;
                            }
                            else
                            {
                                ++pos; // LCOV_EXCL_LINE
                            }
                            zipios::FileEntry::pointer_t entry_ignore_a(cc.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore_a);
                            zipios::FileEntry::pointer_t entry_ignore_b(cc.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore_b);
                        }
                        else
                        {
                            // files must all work and we can read them and
                            // compare with the "real thing" and it is equal
                            //
                            // Note: we only test with cc and not dc, the dc
                            // test is present in the DirectoryCollection test
                            //
                            zipios::DirectoryCollection::stream_pointer_t is(cc.getInputStream(name));
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
                            zipios::FileEntry::pointer_t entry_match_a(dc.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match_a);
                            zipios::FileEntry::pointer_t entry_match_b(cc.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match_b);

                            std::string::size_type pos(name.rfind('/'));
                            if(pos == std::string::npos)
                            {
                                pos = 0; // LCOV_EXCL_LINE
                            }
                            else
                            {
                                ++pos;
                            }
                            zipios::FileEntry::pointer_t entry_ignore_a(dc.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore_a);
                            zipios::FileEntry::pointer_t entry_ignore_b(cc.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore_b);
                        }
                    }
                }

                zipios::CollectionCollection copy_constructor(cc);
                REQUIRE(copy_constructor.isValid());
                REQUIRE_FALSE(copy_constructor.entries().empty());
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_constructor.getName() == "-");   // copy name as is
                REQUIRE(copy_constructor.size() == tree.size());
                copy_constructor.mustBeValid();

                SECTION("test the CollectionCollection::getInputStream() for each file in the tree (copy constructor)")
                {
                    // this DirectoryCollection is recursive so we get ALL
                    // the files in the collection
                    zipios_test::file_t::filenames_t all_files(tree.get_all_filenames());

                    for(auto it(all_files.begin()); it != all_files.end(); ++it)
                    {
                        std::string const name(*it);

                        if(!name.empty() && name.back() == '/')  // Directory?
                        {
                            // directories cannot be attached to an istream
                            zipios::CollectionCollection::stream_pointer_t is1(copy_constructor.getInputStream(name));
                            REQUIRE(!is1);

                            // also test without the ending '/', just in case
                            zipios::CollectionCollection::stream_pointer_t is2(copy_constructor.getInputStream(name.substr(0, name.length() - 1)));
                            REQUIRE(!is2);

                            // now also test the getEntry() which works with MATCH
                            // or IGNORE -- prove it!
                            //
                            zipios::FileEntry::pointer_t entry_match(copy_constructor.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
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
                            zipios::FileEntry::pointer_t entry_ignore(copy_constructor.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                        else
                        {
                            // files must all work and we can read them and
                            // compare with the "real thing" and it is equal
                            //
                            // Note: we only test with cc and not dc, the dc
                            // test is present in the DirectoryCollection test
                            //
                            zipios::DirectoryCollection::stream_pointer_t is(copy_constructor.getInputStream(name));
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
                            zipios::FileEntry::pointer_t entry_match(copy_constructor.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
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
                            zipios::FileEntry::pointer_t entry_ignore(copy_constructor.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                    }
                }

                zipios::CollectionCollection copy_assignment;
                copy_assignment = cc;
                REQUIRE(copy_assignment.isValid());
                REQUIRE_FALSE(copy_assignment.entries().empty());
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_assignment.getName() == "-");   // copy name as is
                REQUIRE(copy_assignment.size() == tree.size());
                copy_assignment.mustBeValid();

                SECTION("test the CollectionCollection::getInputStream() for each file in the tree (copy assignment)")
                {
                    // this DirectoryCollection is recursive so we get ALL
                    // the files in the collection
                    zipios_test::file_t::filenames_t all_files(tree.get_all_filenames());

                    for(auto it(all_files.begin()); it != all_files.end(); ++it)
                    {
                        std::string const name(*it);

                        if(!name.empty() && name.back() == '/')  // Directory?
                        {
                            // directories cannot be attached to an istream
                            zipios::CollectionCollection::stream_pointer_t is1(copy_assignment.getInputStream(name));
                            REQUIRE(!is1);

                            // also test without the ending '/', just in case
                            zipios::CollectionCollection::stream_pointer_t is2(copy_assignment.getInputStream(name.substr(0, name.length() - 1)));
                            REQUIRE(!is2);

                            // now also test the getEntry() which works with MATCH
                            // or IGNORE -- prove it!
                            //
                            zipios::FileEntry::pointer_t entry_match(copy_assignment.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match);

                            std::string::size_type pos(name.rfind('/', name.length() - 2));
                            if(pos == std::string::npos)
                            {
                                pos = 0;
                            }
                            else
                            {
                                ++pos; // LCOV_EXCL_LINE
                            }
                            zipios::FileEntry::pointer_t entry_ignore(copy_assignment.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                        else
                        {
                            // files must all work and we can read them and
                            // compare with the "real thing" and it is equal
                            //
                            zipios::DirectoryCollection::stream_pointer_t is(copy_assignment.getInputStream(name));
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
                            zipios::FileEntry::pointer_t entry_match(copy_assignment.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
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
                            zipios::FileEntry::pointer_t entry_ignore(copy_assignment.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                    }
                }

                zipios::FileCollection::pointer_t clone(cc.clone());
                REQUIRE(dynamic_cast<zipios::CollectionCollection *>(clone.get()));
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "-");   // copy name as is
                REQUIRE(clone->size() == tree.size());
                clone->mustBeValid();

                SECTION("test the CollectionCollection::getInputStream() for each file in the tree (clone)")
                {
                    // this DirectoryCollection is recursive so we get ALL
                    // the files in the collection
                    zipios_test::file_t::filenames_t all_files(tree.get_all_filenames());

                    for(auto it(all_files.begin()); it != all_files.end(); ++it)
                    {
                        std::string const name(*it);

                        if(!name.empty() && name.back() == '/')  // Directory?
                        {
                            // directories cannot be attached to an istream
                            zipios::CollectionCollection::stream_pointer_t is1(clone->getInputStream(name));
                            REQUIRE(!is1);

                            // also test without the ending '/', just in case
                            zipios::CollectionCollection::stream_pointer_t is2(clone->getInputStream(name.substr(0, name.length() - 1)));
                            REQUIRE(!is2);

                            // now also test the getEntry() which works with MATCH
                            // or IGNORE -- prove it!
                            //
                            zipios::FileEntry::pointer_t entry_match(clone->getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                            REQUIRE(entry_match);

                            std::string::size_type pos(name.rfind('/', name.length() - 2));
                            if(pos == std::string::npos)
                            {
                                pos = 0;
                            }
                            else
                            {
                                ++pos; // LCOV_EXCL_LINE
                            }
                            zipios::FileEntry::pointer_t entry_ignore(clone->getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                        else
                        {
                            // files must all work and we can read them and
                            // compare with the "real thing" and it is equal
                            //
                            zipios::DirectoryCollection::stream_pointer_t is(clone->getInputStream(name));
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
                            zipios::FileEntry::pointer_t entry_match(clone->getEntry(name, zipios::FileCollection::MatchPath::MATCH));
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
                            zipios::FileEntry::pointer_t entry_ignore(clone->getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                            REQUIRE(entry_ignore);
                        }
                    }
                }
            }
        }

        WHEN("we add existing directories and another sub-collection")
        {
            // Build a collection that looks like this to verify that we
            // do have access to all the files in all collections:
            //
            //                                +---------------------------+
            //                                | CollectionCollection (cc) |
            //                                +---------------------------+
            //                                             |
            //              +------------------------------+-------------------------------+
            //              |                              |                               |
            //              v                              v                               v
            // +---------------------------+  +---------------------------+  +---------------------------+
            // | DirectoryCollection (dc1) |  | CollectionCollection (cc) |  | DirectoryCollection (dc2) |
            // +---------------------------+  +---------------------------+  +---------------------------+
            //                                             |
            //              +------------------------------+-------------------------------+
            //              |                              |                               |
            //              v                              v                               v
            // +---------------------------+  +---------------------------+  +---------------------------+
            // | DirectoryCollection (dc3) |  | DirectoryCollection (cc4) |  | DirectoryCollection (dc5) |
            // +---------------------------+  +---------------------------+  +---------------------------+
            //

            // create directory trees starting in "tree1", "tree2", ..., "tree5"
            REQUIRE(system("rm -rf tree[1-5]")!=-1); // clean up, just in case

            zipios_test::file_t::vector_t tree;
            zipios::DirectoryCollection::vector_t dc;
            size_t total_size(0);
            for(int i(0); i < 5; ++i)
            {
                size_t start_count(rand() % 5 + 5); // very small, we create 5 of them already!
                std::string name("tree");
                name += std::to_string(i + 1);
                tree.push_back(zipios_test::file_t::pointer_t(new zipios_test::file_t(zipios_test::file_t::type_t::DIRECTORY, start_count, name)));
                total_size += tree[i]->size();
                dc.push_back(zipios::FileCollection::pointer_t(new zipios::DirectoryCollection(name, true)));
            }

            // build sub-collection first
            zipios::CollectionCollection sc;
            REQUIRE(sc.addCollection(dc[2]));
            REQUIRE(sc.size() == dc[2]->size());
            REQUIRE(sc.addCollection(dc[3]));
            REQUIRE(sc.size() == dc[2]->size() + dc[3]->size());
            REQUIRE(sc.addCollection(dc[4]));
            REQUIRE(sc.size() == dc[2]->size() + dc[3]->size() + dc[4]->size());

            // now add the collections to the main collection
            REQUIRE(cc.addCollection(sc));
            REQUIRE(cc.size() == dc[2]->size() + dc[3]->size() + dc[4]->size());
            REQUIRE(cc.addCollection(dc[0]));
            REQUIRE(cc.size() == dc[0]->size() + dc[2]->size() + dc[3]->size() + dc[4]->size());
            REQUIRE(cc.addCollection(dc[1]));
            REQUIRE(cc.size() == dc[0]->size() + dc[1]->size() + dc[2]->size() + dc[3]->size() + dc[4]->size());

            // now we have that tree as shown above, test it

            THEN("it is valid and we can find all the files in all the collections")
            {
                REQUIRE(cc.isValid());
                REQUIRE_FALSE(cc.entries().empty());
                REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(cc.getName() == "-");   // default name is "-"
                REQUIRE(cc.size() == total_size);
                cc.mustBeValid();

                SECTION("test the CollectionCollection::getInputStream() for each file in the collection of trees (original)")
                {
                    // this DirectoryCollection are all recursive so we get
                    // ALL the files in the collection
                    for(int i(0); i < 5; ++i)
                    {
                        zipios_test::file_t::filenames_t all_files(tree[i]->get_all_filenames());

                        for(auto it(all_files.begin()); it != all_files.end(); ++it)
                        {
                            std::string const name(*it);

                            if(!name.empty() && name.back() == '/')  // Directory?
                            {
                                // directories cannot be attached to an istream
                                zipios::CollectionCollection::stream_pointer_t is1(cc.getInputStream(name));
                                REQUIRE(!is1);

                                // also test without the ending '/', just in case
                                zipios::CollectionCollection::stream_pointer_t is2(cc.getInputStream(name.substr(0, name.length() - 1)));
                                REQUIRE(!is2);

                                // now also test the getEntry() which works with MATCH
                                // or IGNORE -- prove it!
                                //
                                zipios::FileEntry::pointer_t entry_match(cc.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
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
                                zipios::FileEntry::pointer_t entry_ignore(cc.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                                REQUIRE(entry_ignore);
                            }
                            else
                            {
                                // files must all work and we can read them and
                                // compare with the "real thing" and it is equal
                                //
                                // Note: we only test with cc and not dc, the dc
                                // test is present in the DirectoryCollection test
                                //
                                zipios::DirectoryCollection::stream_pointer_t is(cc.getInputStream(name));
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
                                zipios::FileEntry::pointer_t entry_match(cc.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
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
                                zipios::FileEntry::pointer_t entry_ignore(cc.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                                REQUIRE(entry_ignore);
                            }
                        }
                    }
                }

                // skipping on the copy constructor and assignment since
                // the clone use the same process (i.e. copy constructor)...

                // in this case we want to create a clone, then delete
                // it and make sure that our original is still fine
                {
                    zipios::FileCollection::pointer_t clone(cc.clone());
                    REQUIRE(dynamic_cast<zipios::CollectionCollection *>(clone.get()));
                    REQUIRE(clone->isValid());
                    REQUIRE_FALSE(clone->entries().empty());
                    REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                    REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                    REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                    REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                    REQUIRE(clone->getName() == "-");   // copy name as is
                    REQUIRE(clone->size() == total_size);
                    clone->mustBeValid();

                    SECTION("test the CollectionCollection::getInputStream() for each file in the tree (clone)")
                    {
                        for(int i(0); i < 5; ++i)
                        {
                            // this DirectoryCollection is recursive so we get ALL
                            // the files in the collection
                            zipios_test::file_t::filenames_t all_files(tree[i]->get_all_filenames());

                            for(auto it(all_files.begin()); it != all_files.end(); ++it)
                            {
                                std::string const name(*it);

                                if(!name.empty() && name.back() == '/')  // Directory?
                                {
                                    // directories cannot be attached to an istream
                                    zipios::CollectionCollection::stream_pointer_t is1(clone->getInputStream(name));
                                    REQUIRE(!is1);

                                    // also test without the ending '/', just in case
                                    zipios::CollectionCollection::stream_pointer_t is2(clone->getInputStream(name.substr(0, name.length() - 1)));
                                    REQUIRE(!is2);

                                    // now also test the getEntry() which works with MATCH
                                    // or IGNORE -- prove it!
                                    //
                                    zipios::FileEntry::pointer_t entry_match(clone->getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
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
                                    zipios::FileEntry::pointer_t entry_ignore(clone->getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                                    REQUIRE(entry_ignore);
                                }
                                else
                                {
                                    // files must all work and we can read them and
                                    // compare with the "real thing" and it is equal
                                    //
                                    zipios::DirectoryCollection::stream_pointer_t is(clone->getInputStream(name));
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
                                    zipios::FileEntry::pointer_t entry_match(clone->getEntry(name, zipios::FileCollection::MatchPath::MATCH));
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
                                    zipios::FileEntry::pointer_t entry_ignore(clone->getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                                    REQUIRE(entry_ignore);
                                }
                            }
                        }
                    }
                }
            }

            THEN("close that collectino of collection and it is now invalid")
            {
                cc.close();

                REQUIRE_FALSE(cc.isValid());
                REQUIRE_THROWS_AS(cc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getName(), zipios::InvalidStateException);   // default name is "-", but we should not return to test that
                REQUIRE_THROWS_AS(cc.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(cc), zipios::InvalidStateException);

                zipios::CollectionCollection copy_constructor(cc);
                REQUIRE_FALSE(copy_constructor.isValid());
                REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_constructor.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(copy_constructor), zipios::InvalidStateException);

                zipios::CollectionCollection copy_assignment;
                copy_assignment = cc;
                REQUIRE_FALSE(copy_assignment.isValid());
                REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_assignment.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(copy_assignment), zipios::InvalidStateException);

                zipios::FileCollection::pointer_t clone(cc.clone());
                REQUIRE(dynamic_cast<zipios::CollectionCollection *>(clone.get()));
                REQUIRE_FALSE(clone->isValid());
                REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(clone->size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dynamic_cast<zipios::CollectionCollection *>(clone.get())->addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(*clone), zipios::InvalidStateException);
            }
        }

        WHEN("closing the directory")
        {
            cc.close();

            // adding a collection to itself fails
            REQUIRE_THROWS_AS(cc.addCollection(cc), zipios::InvalidStateException);

            THEN("it is now invalid")
            {
                REQUIRE_FALSE(cc.isValid());
                REQUIRE_THROWS_AS(cc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getName(), zipios::InvalidStateException);   // default name is "-"
                REQUIRE_THROWS_AS(cc.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(cc), zipios::InvalidStateException);

                zipios::CollectionCollection copy_constructor(cc);
                REQUIRE_FALSE(copy_constructor.isValid());
                REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_constructor.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(copy_constructor), zipios::InvalidStateException);

                zipios::CollectionCollection copy_assignment;
                copy_assignment = cc;
                REQUIRE_FALSE(copy_assignment.isValid());
                REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_assignment.size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(copy_assignment), zipios::InvalidStateException);

                zipios::FileCollection::pointer_t clone(cc.clone());
                REQUIRE(dynamic_cast<zipios::CollectionCollection *>(clone.get()));
                REQUIRE_FALSE(clone->isValid());
                REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getName(), zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(clone->size(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dynamic_cast<zipios::CollectionCollection *>(clone.get())->addCollection(cc), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.addCollection(*clone), zipios::InvalidStateException);
            }
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
