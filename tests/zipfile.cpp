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
 * Zipios unit tests for the ZipFile class.
 */

#include "tests.hpp"

#include "zipios/zipfile.hpp"
#include "zipios/directorycollection.hpp"
#include "zipios/zipiosexceptions.hpp"
#include "zipios/dosdatetime.hpp"

#include <algorithm>
#include <fstream>

#include <unistd.h>
#include <string.h>
#include <zlib.h>



namespace
{


zipios::StorageMethod const g_supported_storage_methods[]
{
    zipios::StorageMethod::STORED,
    zipios::StorageMethod::DEFLATED
};


} // no name namespace




TEST_CASE("An Empty ZipFile", "[ZipFile] [FileCollection]")
{
    zipios::ZipFile zf;

    REQUIRE(zf.isValid());
    REQUIRE(zf.entries().empty());
    REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
    REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
    REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
    REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
    REQUIRE(zf.getName() == "-");
    REQUIRE(zf.size() == 0);
    zf.mustBeValid();
}


TEST_CASE("A ZipFile with an invalid name", "[ZipFile] [FileCollection]")
{
    REQUIRE_THROWS_AS([&](){
                    zipios::ZipFile zf("this/file/does/not/exists/so/the/constructor/throws");
                }(), zipios::IOException);
}


TEST_CASE("A ZipFile with an invalid file", "[ZipFile] [FileCollection]")
{
    // create a totally random file which means there is still a very slight
    // chance that it represents a valid ZipFile, but frankly... no.
    zipios_test::auto_unlink_t auto_unlink("invalid.zip");
    {
        std::ofstream os("invalid.zip", std::ios::out | std::ios::binary);
        size_t const max_size(rand() % 1024 + 1024);
        for(size_t i(0); i < max_size; ++i)
        {
            os << static_cast<char>(rand());
        }
    }
    REQUIRE_THROWS_AS([&](){
                    zipios::ZipFile zf("invalid.zip");
                }(), zipios::FileCollectionException);
}


TEST_CASE("An empty ZipFile", "[ZipFile] [FileCollection]")
{
    // this is a special case where the file is composed of one
    // End of Central Directory with 0 entries
    zipios_test::auto_unlink_t auto_unlink("empty.zip");
    {
        std::ofstream os("empty.zip", std::ios::out | std::ios::binary);
        os << static_cast<char>(0x50);
        os << static_cast<char>(0x4B);
        os << static_cast<char>(0x05);
        os << static_cast<char>(0x06);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
        os << static_cast<char>(0x00);
    }
    zipios::ZipFile zf("empty.zip");

    REQUIRE(zf.isValid());
    REQUIRE(zf.entries().empty());
    REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
    REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
    REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
    REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
    REQUIRE(zf.getName() == "empty.zip");
    REQUIRE(zf.size() == 0);
    zf.mustBeValid(); // not throwing
}


SCENARIO("ZipFile with a valid zip archive", "[ZipFile] [FileCollection]")
{
    GIVEN("a tree directory")
    {
        REQUIRE(system("rm -rf tree") == 0); // clean up, just in case
        size_t const start_count(rand() % 40 + 80);
        zipios_test::file_t tree(zipios_test::file_t::type_t::DIRECTORY, start_count, "tree");
        zipios_test::auto_unlink_t remove_zip("tree.zip");
        REQUIRE(system("zip -r tree.zip tree >/dev/null") == 0);

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
                         // you would think that the compressed size would
                         // either be equal to the size or smaller, but never
                         // larger, that's not the case with zip under Linux...
                         //
                         // they probably use a streaming mechanism and thus
                         // cannot fix the problem later if the compressed
                         // version ends up being larger than the
                         // non-compressed version...
                         //
                         //REQUIRE((*it)->getCompressedSize() < (*it)->getSize());
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    zipios::DOSDateTime dt;
                    dt.setUnixTimestamp(file_stats.st_mtime);
                    REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                    std::time_t ut(dt.getUnixTimestamp());
                    REQUIRE((*it)->getUnixTime() == ut);
                    REQUIRE_FALSE((*it)->hasCrc());
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    if(t == zipios_test::file_t::type_t::DIRECTORY)
                    {
                        REQUIRE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == 0); // size is zero for directories
                    }
                    else
                    {
                        REQUIRE_FALSE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == file_stats.st_size);

                        // now read both files (if not a directory) and make sure
                        // they are equal
                        zipios::FileCollection::stream_pointer_t is(zf.getInputStream(entry->getName()));
                        REQUIRE(is);
                        std::ifstream in(entry->getName(), std::ios::in | std::ios::binary);

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

                    // I don't think we will test those directly...
                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }

            THEN("we can create a totally valid clone")
            {
                // we do not have a specific copy constructor and
                // assignment operator so we only try to clone() function
                zipios::ZipFile::pointer_t clone(zf.clone());

                // zf is unaffected
                REQUIRE(zf.isValid());
                REQUIRE_FALSE(zf.entries().empty());
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(zf.getName() == "tree.zip");
                REQUIRE(zf.size() == tree.size());
                zf.mustBeValid(); // not throwing

                // clone is valid
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "tree.zip");
                REQUIRE(clone->size() == tree.size());
                clone->mustBeValid(); // not throwing

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
                         // you would think that the compressed size would
                         // either be equal to the size or smaller, but never
                         // larger, that's not the case with zip under Linux...
                         //
                         // they probably use a streaming mechanism and thus
                         // cannot fix the problem later if the compressed
                         // version ends up being larger than the
                         // non-compressed version...
                         //
                         //REQUIRE((*it)->getCompressedSize() < (*it)->getSize());
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    zipios::DOSDateTime dt;
                    dt.setUnixTimestamp(file_stats.st_mtime);
                    REQUIRE((*it)->getTime() == dt.getDOSDateTime());  // invalid date
                    std::time_t ut(dt.getUnixTimestamp());
                    REQUIRE((*it)->getUnixTime() == ut);
                    REQUIRE_FALSE((*it)->hasCrc());
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    if(t == zipios_test::file_t::type_t::DIRECTORY)
                    {
                        REQUIRE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == 0); // size is zero for directories
                    }
                    else
                    {
                        REQUIRE_FALSE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == file_stats.st_size);

                        // now read both files (if not a directory) and make sure
                        // they are equal
                        zipios::FileCollection::stream_pointer_t is(clone->getInputStream(entry->getName()));
                        REQUIRE(is);
                        std::ifstream in(entry->getName(), std::ios::in | std::ios::binary);

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

                    // I don't think we will test those directly...
                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }

            THEN("we can compare incompatible entries against each others")
            {
                // we read the tree as a directory so we have
                // incompatible entries
                zipios::DirectoryCollection dc("tree");

                zipios::FileEntry::vector_t e(dc.entries());
                zipios::FileEntry::vector_t v(zf.entries());
                REQUIRE(e.size() == v.size()); // same tree so same size
                //size_t const max_entries(std::min(e.size(), v.size());
                for(size_t idx(0); idx < e.size(); ++idx)
                {
                    REQUIRE_FALSE(e[idx]->isEqual(*v[idx]));
                    REQUIRE_FALSE(v[idx]->isEqual(*e[idx]));
                }
            }
        }
    }
}


SCENARIO("use Zipios to create a zip archive", "[ZipFile] [FileCollection]")
{
    GIVEN("a tree directory")
    {
        REQUIRE(system("rm -rf tree tree.zip") == 0); // clean up, just in case
        size_t const start_count(rand() % 40 + 80);
        zipios_test::file_t tree(zipios_test::file_t::type_t::DIRECTORY, start_count, "tree");
        zipios_test::auto_unlink_t remove_zip("tree.zip");
        zipios::DirectoryCollection dc("tree");

        // first, check that the object is setup as expected
        WHEN("we save the directory tree in a .zip file")
        {
            {
                dc.setMethod(1024, zipios::StorageMethod::STORED, zipios::StorageMethod::DEFLATED);
                dc.setLevel(1024, zipios::FileEntry::COMPRESSION_LEVEL_NONE, zipios::FileEntry::COMPRESSION_LEVEL_MAXIMUM);
                std::ofstream out("tree.zip", std::ios::out | std::ios::binary);
                zipios::ZipFile::saveCollectionToArchive(out, dc);
            }

            THEN("it is valid and includes all the files in the tree as expected")
            {
                zipios::ZipFile zf("tree.zip");

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
                         // you would think that the compressed size would
                         // either be equal to the size or smaller, but never
                         // larger, that's not the case with zip under Linux...
                         //
                         // they probably use a streaming mechanism and thus
                         // cannot fix the problem later if the compressed
                         // version ends up being larger than the
                         // non-compressed version...
                         //
                         //REQUIRE((*it)->getCompressedSize() < (*it)->getSize());
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    zipios::DOSDateTime dt;
                    dt.setUnixTimestamp(file_stats.st_mtime);
                    REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                    REQUIRE((*it)->getUnixTime() == dt.getUnixTimestamp());
                    REQUIRE_FALSE((*it)->hasCrc());
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    if(t == zipios_test::file_t::type_t::DIRECTORY)
                    {
                        REQUIRE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == 0); // size is zero for directories
                    }
                    else
                    {
                        REQUIRE_FALSE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == file_stats.st_size);

                        // now read both files (if not a directory) and make sure
                        // they are equal
                        zipios::FileCollection::stream_pointer_t is(zf.getInputStream(entry->getName()));
                        REQUIRE(is);
                        std::ifstream in(entry->getName(), std::ios::in | std::ios::binary);

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

                    // I don't think we will test those directly...
                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }
        }

        // test with all the possible levels
        SECTION("test creating zip with all available levels")
        {
            for(zipios::FileEntry::CompressionLevel level(-3); level <= 100; ++level)
            {
                // Note that a level of COMPRESSION_LEVEL_NONE and method of
                // DEFLATED is valid, the system will ignore the DEFLATED
                // when saving the file and just use STORED instead.
                dc.setMethod(1024, zipios::StorageMethod::STORED, zipios::StorageMethod::DEFLATED);
                dc.setLevel(1024, zipios::FileEntry::COMPRESSION_LEVEL_NONE, level);
                {
                    std::ofstream out("tree.zip", std::ios::out | std::ios::binary | std::ios::trunc);
                    zipios::ZipFile::saveCollectionToArchive(out, dc);
                }

                zipios::ZipFile zf("tree.zip");

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
                         // you would think that the compressed size would
                         // either be equal to the size or smaller, but never
                         // larger, that's not the case with zip under Linux...
                         //
                         // they probably use a streaming mechanism and thus
                         // cannot fix the problem later if the compressed
                         // version ends up being larger than the
                         // non-compressed version...
                         //
                         //REQUIRE((*it)->getCompressedSize() < (*it)->getSize());
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    zipios::DOSDateTime dt;
                    dt.setUnixTimestamp(file_stats.st_mtime);
                    REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                    REQUIRE((*it)->getUnixTime() == dt.getUnixTimestamp());
                    REQUIRE_FALSE((*it)->hasCrc());
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    if(t == zipios_test::file_t::type_t::DIRECTORY)
                    {
                        REQUIRE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == 0); // size is zero for directories
                    }
                    else
                    {
                        REQUIRE_FALSE((*it)->isDirectory());
                        REQUIRE((*it)->getSize() == file_stats.st_size);

                        // now read both files (if not a directory) and make sure
                        // they are equal
                        zipios::FileCollection::stream_pointer_t is(zf.getInputStream(entry->getName()));
                        REQUIRE(is);
                        std::ifstream in(entry->getName(), std::ios::in | std::ios::binary);

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

                        REQUIRE_FALSE(in);
                        REQUIRE_FALSE(*is);
                    }

                    // I don't think we will test those directly...
                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }
        }
    }
}


SCENARIO("use Zipios to create zip archives with 1 or 3 files each", "[ZipFile] [FileCollection]")
{
    GIVEN("a one file zip file")
    {
        REQUIRE(system("rm -f file.bin") == 0); // clean up, just in case
        {
            std::ofstream file_bin("file.bin", std::ios::out | std::ios::binary);
            file_bin << "this zip file contents.\n";
        }
        zipios_test::auto_unlink_t remove_bin("file.bin");

        // first, check that the object is setup as expected
        WHEN("we save the file in a .zip")
        {
            zipios::DirectoryCollection dc("file.bin");
            zipios_test::auto_unlink_t remove_zip("file.zip");
            {
                std::ofstream out("file.zip", std::ios::out | std::ios::binary);
                zipios::ZipFile::saveCollectionToArchive(out, dc);
            }

            THEN("it is valid and includes the file as expected")
            {
                zipios::ZipFile zf("file.zip");

                REQUIRE(zf.isValid());
                REQUIRE_FALSE(zf.entries().empty());
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(zf.getName() == "file.zip");
                REQUIRE(zf.size() == 1);
                zf.mustBeValid(); // not throwing

                zipios::FileEntry::vector_t v(zf.entries());
                REQUIRE(v.size() == 1);
                for(auto it(v.begin()); it != v.end(); ++it)
                {
                    zipios::FileEntry::pointer_t entry(*it);

                    struct stat file_stats;
                    REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                    REQUIRE((*it)->getComment().empty());
                    REQUIRE((*it)->getCompressedSize() == (*it)->getSize()); // we keep STORED as the method
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
                         // you would think that the compressed size would
                         // either be equal to the size or smaller, but never
                         // larger, that is not the case with zip under Linux...
                         //
                         // they probably use a streaming mechanism and thus
                         // cannot fix the problem later if the compressed
                         // version ends up being larger than the
                         // non-compressed version...
                         //
                         //REQUIRE((*it)->getCompressedSize() < (*it)->getSize());
                    }
                    //REQUIRE((*it)->getName() == ...);
                    //REQUIRE((*it)->getFileName() == ...);
                    zipios::DOSDateTime dt;
                    dt.setUnixTimestamp(file_stats.st_mtime);
                    REQUIRE((*it)->getTime() == dt.getDOSDateTime());
                    REQUIRE((*it)->getUnixTime() == dt.getUnixTimestamp());
                    REQUIRE_FALSE((*it)->hasCrc());
                    REQUIRE((*it)->isValid());
                    //REQUIRE((*it)->toString() == "... (0 bytes)");

                    REQUIRE_FALSE((*it)->isDirectory());
                    REQUIRE((*it)->getSize() == file_stats.st_size);

                    // now read both files (if not a directory) and make sure
                    // they are equal
                    zipios::FileCollection::stream_pointer_t is(zf.getInputStream(entry->getName()));
                    REQUIRE(is);
                    std::ifstream in(entry->getName(), std::ios::in | std::ios::binary);

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

                    REQUIRE_FALSE(in);
                    REQUIRE_FALSE(*is);

                    // I don't think we will test those directly...
                    //REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                    //REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                }
            }
        }

/** \todo
 * Once clang is fixed, remove those #if/#endif boundaries. clang does not
 * clear the std::unchecked_exception() flag when we have a re-throw in a
 * catch.
 */
#ifndef __clang__
        // test with a comment that's too large
        WHEN("we make sure that saving the file fails if the comment is too large")
        {
            zipios::DirectoryCollection dc("file.bin");
            zipios::FileEntry::vector_t v(dc.entries());
            REQUIRE(v.size() == 1);
            auto it(v.begin());
            // generate a random comment of 65Kb
            std::string comment;
            for(int i(0); i < 65 * 1024; ++i)
            {
                comment += rand() % 26 + 'A';
            }
            (*it)->setComment(comment);

            THEN("it is invalid and fails as expected")
            {
                zipios_test::auto_unlink_t remove_zip("file.zip");
                {
                    std::ofstream out("file.zip", std::ios::out | std::ios::binary);
                    REQUIRE_THROWS_AS(zipios::ZipFile::saveCollectionToArchive(out, dc), zipios::InvalidStateException);
                    REQUIRE_FALSE(out);
                }
            }
        }
#endif

#ifndef __clang__
        // check that extra buffers that are too large make the save fail
        WHEN("we make sure that saving the file fails if the extra buffer is too large")
        {
            zipios::DirectoryCollection dc("file.bin");
            zipios::FileEntry::vector_t v(dc.entries());
            REQUIRE(v.size() == 1);
            auto it(v.begin());
            // generate a random extra buffer of 65Kb
            zipios::FileEntry::buffer_t buffer;
            for(int i(0); i < 65 * 1024; ++i)
            {
                buffer.push_back(static_cast<unsigned char>(rand()));
            }
            (*it)->setExtra(buffer);

            THEN("it is invalid and fails as expected")
            {
                zipios_test::auto_unlink_t remove_zip("file.zip");
                {
                    std::ofstream out("file.zip", std::ios::out | std::ios::binary);
                    REQUIRE_THROWS_AS(zipios::ZipFile::saveCollectionToArchive(out, dc), zipios::InvalidStateException);
                    REQUIRE_FALSE(out);
                }
            }
        }
#endif

#ifndef __clang__
        // check with a global comment which is too large
        WHEN("we make sure that saving the file fails if the Zip (gloabl) comment is too large")
        {
            zipios::DirectoryCollection dc("file.bin");
            // generate a random comment of 65Kb
            std::string comment;
            for(int i(0); i < 65 * 1024; ++i)
            {
                comment += rand() % 26 + 'A';
            }

            THEN("it is invalid and fails as expected")
            {
                zipios_test::auto_unlink_t remove_zip("file.zip");
                {
                    std::ofstream out("file.zip", std::ios::out | std::ios::binary);
                    REQUIRE_THROWS_AS(zipios::ZipFile::saveCollectionToArchive(out, dc, comment), zipios::InvalidStateException);
                    REQUIRE_FALSE(out);
                }
            }
        }
#endif
    }

#ifndef __clang__
    GIVEN("a very small file")
    {
        REQUIRE(system("rm -f file.bin") == 0); // clean up, just in case
        {
            // one byte file
            std::ofstream file_bin("file.bin", std::ios::out | std::ios::binary);
            file_bin << "1";
        }
        zipios_test::auto_unlink_t remove_bin("file.bin");

        // first, check that the object is setup as expected
        WHEN("we add it more than 64Kb times to the directory")
        {
            zipios::DirectoryCollection dc("file.bin");

            // add another 64Kb file entries! (all the same name, ouch!)
            for(int i(0); i < 64 * 1024 + rand() % 100; ++i)
            {
                zipios::DirectoryEntry other_entry(zipios::FilePath("file.bin"));
                dc.addEntry(other_entry);
            }

            THEN("the creating of the zip archive fails: too many file entries")
            {
                zipios_test::auto_unlink_t remove_zip("file.zip");
                {
                    std::ofstream out("file.zip", std::ios::out | std::ios::binary);
                    REQUIRE_THROWS_AS(zipios::ZipFile::saveCollectionToArchive(out, dc), zipios::InvalidStateException);
                }
            }
        }
    }
#endif
}


TEST_CASE("Simple Valid and Invalid ZipFile Archives", "[ZipFile] [FileCollection]")
{
    SECTION("try one uncompressed file of many sizes")
    {
        REQUIRE(system("rm -f file.bin") == 0); // clean up, just in case
        for(int sz(0); sz <= 128 * 1024; sz += sz < 10 ? 1 : rand() % (1024 * 4))
        {
            zipios_test::auto_unlink_t remove_bin("file.bin");
            zipios_test::auto_unlink_t remove_zip("file.zip");

            // create a file of various sizes (increasingly big though)
            {
                std::ofstream file_bin("file.bin", std::ios::out | std::ios::binary);
                for(int pos(0); pos < sz; ++pos)
                {
                    file_bin << static_cast<char>(rand());
                }
            }

            zipios::DirectoryCollection dc("file.bin");
            zipios::FileEntry::vector_t v(dc.entries());
            (*v.begin())->setLevel(zipios::FileEntry::COMPRESSION_LEVEL_NONE);
            (*v.begin())->setMethod(zipios::StorageMethod::DEFLATED);
            {
                std::ofstream out("file.zip", std::ios::out | std::ios::binary | std::ios::trunc);
                zipios::ZipFile::saveCollectionToArchive(out, dc);
            }

            zipios::ZipFile zf("file.zip");

            REQUIRE(zf.isValid());
            REQUIRE_FALSE(zf.entries().empty());
            REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(zf.getName() == "file.zip");
            REQUIRE(zf.size() == 1);
            zf.mustBeValid(); // not throwing
        }
    }

    SECTION("try three uncompressed files of many sizes")
    {
        REQUIRE(system("rm -f file.zip file?.bin") == 0); // clean up, just in case
        for(int sz(0); sz <= 128 * 1024; sz += sz < 10 ? 1 : rand() % (1024 * 4))
        {
            zipios_test::auto_unlink_t remove_bin1("file1.bin");
            zipios_test::auto_unlink_t remove_bin2("file2.bin");
            zipios_test::auto_unlink_t remove_bin3("file3.bin");
            zipios_test::auto_unlink_t remove_zip("file.zip");

            // create a file of various sizes (increasingly big though)
            {
                std::ofstream file_bin("file1.bin", std::ios::out | std::ios::binary);
                for(int pos(0); pos < sz; ++pos)
                {
                    file_bin << static_cast<char>(rand());
                }
            }
            {
                std::ofstream file_bin("file2.bin", std::ios::out | std::ios::binary);
                for(int pos(0); pos < sz; ++pos)
                {
                    file_bin << static_cast<char>(rand());
                }
            }
            {
                std::ofstream file_bin("file3.bin", std::ios::out | std::ios::binary);
                for(int pos(0); pos < sz; ++pos)
                {
                    file_bin << static_cast<char>(rand());
                }
            }

            zipios::DirectoryCollection dc("file1.bin");
            {
                zipios::DirectoryEntry other_entry2(zipios::FilePath("file2.bin"));
                dc.addEntry(other_entry2);
                zipios::DirectoryEntry other_entry3(zipios::FilePath("file3.bin"));
                dc.addEntry(other_entry3);
            }
            zipios::FileEntry::vector_t v(dc.entries());
            (*v.begin())->setLevel(zipios::FileEntry::COMPRESSION_LEVEL_NONE);
            (*v.begin())->setMethod(zipios::StorageMethod::DEFLATED);
            {
                std::ofstream out("file.zip", std::ios::out | std::ios::binary | std::ios::trunc);
                zipios::ZipFile::saveCollectionToArchive(out, dc);
            }

            zipios::ZipFile zf("file.zip");

            REQUIRE(zf.isValid());
            REQUIRE_FALSE(zf.entries().empty());
            REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(zf.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(zf.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(zf.getName() == "file.zip");
            REQUIRE(zf.size() == 3);
            zf.mustBeValid(); // not throwing
        }
    }
}


// For this one we have a set of structures and we "manually"
// create Zip archive files that we can thus tweak with totally
// invalid parameters
struct local_header_t
{
    typedef std::vector<unsigned char>      buffer_t;

    uint32_t            m_signature;            // "PK 3.4"
    uint16_t            m_version;              // 10 or 20
    uint16_t            m_flags;                // generally zero ("general purpose field")
    uint16_t            m_compression_method;   // Zipios only supports STORED and DEFLATE
    uint32_t            m_time_and_date;        // MS-DOS date and time
    uint32_t            m_crc32;                // CRC-32 of the file data
    uint32_t            m_compressed_size;      // size of data once compressed
    uint32_t            m_uncompressed_size;    // size of data uncompressed
    //uint16_t            m_filename_length;     // length name of this file
    //uint16_t            m_extra_field_length;   // length of extra buffer, Zipios ignore those
    //uint8_t             m_filename[m_filename_length];
    std::string         m_filename;
    //uint8_t             m_extra_field[m_extra_field_length];
    buffer_t            m_extra_field;

    local_header_t()
        : m_signature(0x04034B50)
        , m_version(10)
        , m_flags(0)
        , m_compression_method(0)   // 0 == STORED
        , m_crc32(0)
        , m_compressed_size(0)      // undefined is compression method is 0
        , m_uncompressed_size(0)
        //, m_filename("") -- auto-init
        //, m_extra_field() -- auto-init
    {
        zipios::DOSDateTime dt;
        dt.setUnixTimestamp(time(nullptr));
        m_time_and_date = dt.getDOSDateTime();
    }

    void write(std::ostream& os)
    {
        if(m_filename.empty())
        {
            std::cerr << "bug: local_header_t::write() called without a filename." << std::endl; // LCOV_EXCL_LINE
            exit(1); // LCOV_EXCL_LINE
        }

        // IMPORTANT NOTE:
        // We do not verify any field other than the filename because
        // we want to be able to use this class to create anything
        // (i.e. including invalid headers.)

        os << static_cast<unsigned char>(m_signature >>  0);
        os << static_cast<unsigned char>(m_signature >>  8);
        os << static_cast<unsigned char>(m_signature >> 16);
        os << static_cast<unsigned char>(m_signature >> 24);
        os << static_cast<unsigned char>(m_version >> 0);
        os << static_cast<unsigned char>(m_version >> 8);
        os << static_cast<unsigned char>(m_flags >> 0);
        os << static_cast<unsigned char>(m_flags >> 8);
        os << static_cast<unsigned char>(m_compression_method >> 0);
        os << static_cast<unsigned char>(m_compression_method >> 8);
        os << static_cast<unsigned char>(m_time_and_date >>  0);
        os << static_cast<unsigned char>(m_time_and_date >>  8);
        os << static_cast<unsigned char>(m_time_and_date >> 16);
        os << static_cast<unsigned char>(m_time_and_date >> 24);
        os << static_cast<unsigned char>(m_crc32 >>  0);
        os << static_cast<unsigned char>(m_crc32 >>  8);
        os << static_cast<unsigned char>(m_crc32 >> 16);
        os << static_cast<unsigned char>(m_crc32 >> 24);
        os << static_cast<unsigned char>(m_compressed_size >>  0);
        os << static_cast<unsigned char>(m_compressed_size >>  8);
        os << static_cast<unsigned char>(m_compressed_size >> 16);
        os << static_cast<unsigned char>(m_compressed_size >> 24);
        os << static_cast<unsigned char>(m_uncompressed_size >>  0);
        os << static_cast<unsigned char>(m_uncompressed_size >>  8);
        os << static_cast<unsigned char>(m_uncompressed_size >> 16);
        os << static_cast<unsigned char>(m_uncompressed_size >> 24);
        uint16_t filename_length(m_filename.length());
        os << static_cast<unsigned char>(filename_length >> 0);
        os << static_cast<unsigned char>(filename_length >> 8);
        uint16_t extra_field_length(m_extra_field.size());
        os << static_cast<unsigned char>(extra_field_length >> 0);
        os << static_cast<unsigned char>(extra_field_length >> 8);
        os << m_filename;
        os.write(reinterpret_cast<char const *>(&m_extra_field[0]), m_extra_field.size());
    }
};


struct central_directory_header_t
{
    typedef std::vector<unsigned char>      buffer_t;

    uint32_t            m_signature;            // 00 -- "PK 2.1"
    uint16_t            m_version;              // 04 -- 10 or 20
    uint16_t            m_extract_version;      // 06 -- generally zero
    uint16_t            m_flags;                // 08 -- various flags
    uint16_t            m_compression_method;   // 0A -- method used to compress the data
    uint32_t            m_time_and_date;        // 0C -- MS-DOS date and time
    uint32_t            m_crc32;                // 10 -- CRC-32 of the file data
    uint32_t            m_compressed_size;      // 14 -- size of data once compressed
    uint32_t            m_uncompressed_size;    // 18 -- size of data uncompressed
    //uint16_t            m_filename_length;      // 1C -- length name of this file
    //uint16_t            m_extra_field_length;   // 1E -- length of extra buffer, Zipios ignore those
    //uint16_t            m_file_comment_length;  // 20 -- length of comment
    uint16_t            m_disk_number_start;                // 22 -- disk number of split archives
    uint16_t            m_internal_file_attributes;         // 24 -- file attributes
    uint32_t            m_external_file_attributes;         // 26 -- file attributes
    uint32_t            m_relative_offset_to_local_header;  // 2A -- offset to actual file
    //uint8_t             m_filename[m_filename_length];    // 2E -- filename (variable size
    std::string         m_filename;
    //uint8_t             m_extra_field[m_extra_field_length];      // .. -- extra field(s)
    buffer_t            m_extra_field;
    //uint8_t             m_file_comment[m_file_comment_length];    // .. -- file comment
    std::string         m_file_comment;

    central_directory_header_t()
        : m_signature(0x02014B50)
        , m_version(10)
        , m_extract_version(10)
        , m_flags(0)
        , m_compression_method(0)   // 0 == STORED
        , m_crc32(0)
        , m_compressed_size(0)      // undefined is compression method is 0
        , m_uncompressed_size(0)
        , m_disk_number_start(0)
        , m_internal_file_attributes(0)
        , m_external_file_attributes(0)
        , m_relative_offset_to_local_header(0)
        //, m_filename("") -- auto-init
        //, m_extra_field() -- auto-init
        //, m_file_comment("") -- auto-init
    {
        zipios::DOSDateTime dt;
        dt.setUnixTimestamp(time(nullptr));
        m_time_and_date = dt.getDOSDateTime();
    }

    void write(std::ostream& os)
    {
        if(m_filename.empty())
        {
            std::cerr << "bug: central_directory_header_t::write() called without a filename." << std::endl; // LCOV_EXCL_LINE
            exit(1); // LCOV_EXCL_LINE
        }

        // IMPORTANT NOTE:
        // We do not verify any field other than the filename because
        // we want to be able to use this class to create anything
        // (i.e. including invalid headers.)

        os << static_cast<unsigned char>(m_signature >>  0);
        os << static_cast<unsigned char>(m_signature >>  8);
        os << static_cast<unsigned char>(m_signature >> 16);
        os << static_cast<unsigned char>(m_signature >> 24);
        os << static_cast<unsigned char>(m_version >> 0);
        os << static_cast<unsigned char>(m_version >> 8);
        os << static_cast<unsigned char>(m_extract_version >> 0);
        os << static_cast<unsigned char>(m_extract_version >> 8);
        os << static_cast<unsigned char>(m_flags >> 0);
        os << static_cast<unsigned char>(m_flags >> 8);
        os << static_cast<unsigned char>(m_compression_method >> 0);
        os << static_cast<unsigned char>(m_compression_method >> 8);
        os << static_cast<unsigned char>(m_time_and_date >>  0);
        os << static_cast<unsigned char>(m_time_and_date >>  8);
        os << static_cast<unsigned char>(m_time_and_date >> 16);
        os << static_cast<unsigned char>(m_time_and_date >> 24);
        os << static_cast<unsigned char>(m_crc32 >>  0);
        os << static_cast<unsigned char>(m_crc32 >>  8);
        os << static_cast<unsigned char>(m_crc32 >> 16);
        os << static_cast<unsigned char>(m_crc32 >> 24);
        os << static_cast<unsigned char>(m_compressed_size >>  0);
        os << static_cast<unsigned char>(m_compressed_size >>  8);
        os << static_cast<unsigned char>(m_compressed_size >> 16);
        os << static_cast<unsigned char>(m_compressed_size >> 24);
        os << static_cast<unsigned char>(m_uncompressed_size >>  0);
        os << static_cast<unsigned char>(m_uncompressed_size >>  8);
        os << static_cast<unsigned char>(m_uncompressed_size >> 16);
        os << static_cast<unsigned char>(m_uncompressed_size >> 24);
        uint16_t filename_length(m_filename.length());
        os << static_cast<unsigned char>(filename_length >> 0);
        os << static_cast<unsigned char>(filename_length >> 8);
        uint16_t extra_field_length(m_extra_field.size());
        os << static_cast<unsigned char>(extra_field_length >> 0);
        os << static_cast<unsigned char>(extra_field_length >> 8);
        uint16_t file_comment_length(m_file_comment.length());
        os << static_cast<unsigned char>(file_comment_length >> 0);
        os << static_cast<unsigned char>(file_comment_length >> 8);
        os << static_cast<unsigned char>(m_disk_number_start >> 0);
        os << static_cast<unsigned char>(m_disk_number_start >> 8);
        os << static_cast<unsigned char>(m_internal_file_attributes >> 0);
        os << static_cast<unsigned char>(m_internal_file_attributes >> 8);
        os << static_cast<unsigned char>(m_external_file_attributes >>  0);
        os << static_cast<unsigned char>(m_external_file_attributes >>  8);
        os << static_cast<unsigned char>(m_external_file_attributes >> 16);
        os << static_cast<unsigned char>(m_external_file_attributes >> 24);
        os << static_cast<unsigned char>(m_relative_offset_to_local_header >>  0);
        os << static_cast<unsigned char>(m_relative_offset_to_local_header >>  8);
        os << static_cast<unsigned char>(m_relative_offset_to_local_header >> 16);
        os << static_cast<unsigned char>(m_relative_offset_to_local_header >> 24);
        os << m_filename;
        os.write(reinterpret_cast<char const *>(&m_extra_field[0]), m_extra_field.size());
        os << m_file_comment;
    }
};


struct end_of_central_directory_t
{
    uint32_t            m_signature;        // "PK 5.6"
    uint16_t            m_disk_number;
    uint16_t            m_disk_start;
    uint16_t            m_file_count;       // number of files in this archive
    uint16_t            m_total_count;      // total number across all split files
    uint32_t            m_central_directory_size;
    uint32_t            m_central_directory_offset;
    //uint16_t            m_comment_length;
    //unsigned char       m_comment[m_comment_length];
    std::string         m_comment;

    end_of_central_directory_t()
        : m_signature(0x06054B50)
        , m_disk_number(0)
        , m_disk_start(0)
        , m_file_count(0)
        , m_total_count(0)
        , m_central_directory_size(0)
        , m_central_directory_offset(0)
        //, m_comment_length(0)
        //, m_comment("") -- auto-init
    {
    }

    void write(std::ostream& os)
    {
        // IMPORTANT NOTE:
        // We do not verify any of the values on purpose, we want to be
        // able to use this class to create anything (i.e. including invalid
        // headers.)

        os << static_cast<unsigned char>(m_signature >>  0);
        os << static_cast<unsigned char>(m_signature >>  8);
        os << static_cast<unsigned char>(m_signature >> 16);
        os << static_cast<unsigned char>(m_signature >> 24);
        os << static_cast<unsigned char>(m_disk_number >> 0);
        os << static_cast<unsigned char>(m_disk_number >> 8);
        os << static_cast<unsigned char>(m_disk_start >> 0);
        os << static_cast<unsigned char>(m_disk_start >> 8);
        os << static_cast<unsigned char>(m_file_count >> 0);
        os << static_cast<unsigned char>(m_file_count >> 8);
        os << static_cast<unsigned char>(m_total_count >>  0);
        os << static_cast<unsigned char>(m_total_count >>  8);
        os << static_cast<unsigned char>(m_central_directory_size >>  0);
        os << static_cast<unsigned char>(m_central_directory_size >>  8);
        os << static_cast<unsigned char>(m_central_directory_size >> 16);
        os << static_cast<unsigned char>(m_central_directory_size >> 24);
        os << static_cast<unsigned char>(m_central_directory_offset >>  0);
        os << static_cast<unsigned char>(m_central_directory_offset >>  8);
        os << static_cast<unsigned char>(m_central_directory_offset >> 16);
        os << static_cast<unsigned char>(m_central_directory_offset >> 24);
        uint16_t comment_length(m_comment.length());
        os << static_cast<unsigned char>(comment_length >>  0);
        os << static_cast<unsigned char>(comment_length >>  8);
        os << m_comment;
    }
};


TEST_CASE("Valid and Invalid ZipFile Archives", "[ZipFile] [FileCollection]")
{
    SECTION("create files with End of Central Directory that are tool small")
    {
        for(ssize_t i(22 - 1); i >= 0; --i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                end_of_central_directory_t eocd;
                eocd.write(os);
            }

            // truncate the file to 'i' size
            truncate("file.zip", i);

            REQUIRE_THROWS_AS([&](){
                            zipios::ZipFile zf("file.zip");
                        }(), zipios::FileCollectionException);
        }
    }

    SECTION("create files with End of Central Directory file except for the comment")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            size_t const comment_len(rand() % 20 + 5);
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                end_of_central_directory_t eocd;
                for(size_t j(0); j < comment_len; ++j)
                {
                    eocd.m_comment += static_cast<char>('A' + rand() % 26);
                }
                eocd.write(os);
            }

            // truncate the file to not include the whole comment
            // (truncate at least one character though)
            size_t const five(5);
            truncate("file.zip", (22 + comment_len) - (rand() % std::min(five, comment_len) + 1));

            REQUIRE_THROWS_AS([&](){
                            zipios::ZipFile zf("file.zip");
                        }(), zipios::IOException);
        }
    }

    SECTION("create files with End of Central Directory using counts that differ")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            size_t const size1(rand() & 0xFFFF);
            size_t size2;
            do
            {
                size2 = rand() & 0xFFFF;
            }
            while(size1 == size2);
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                end_of_central_directory_t eocd;
                eocd.m_file_count = size1;
                eocd.m_total_count = size2;
                eocd.write(os);
            }

            REQUIRE_THROWS_AS([&](){
                        zipios::ZipFile zf("file.zip");
                    }(), zipios::FileCollectionException);
        }
    }

    SECTION("create files with one Local Entry using an invalid signature")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // since the signature will be invalid, we can ignore the
                // rest too...
                central_directory_header_t cdh;
                cdh.m_signature = 0x01020304;       // an invalid signature
                cdh.m_filename = "invalid";
                cdh.write(os);

                end_of_central_directory_t eocd;
                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);
            }

            REQUIRE_THROWS_AS([&](){
                        zipios::ZipFile zf("file.zip");
                    }(), zipios::IOException);
        }
    }

    SECTION("create files with a valid central directory but no local entries")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // since the signature will be invalid, we can ignore the
                // rest too...
                central_directory_header_t cdh;
                cdh.m_filename = "invalid";
                cdh.write(os);

                end_of_central_directory_t eocd;
                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);
            }

            REQUIRE_THROWS_AS([&](){
                        zipios::ZipFile zf("file.zip");
                    }(), zipios::IOException);
        }
    }

    SECTION("create files with one an unsupported compression method")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // create a header (has to be equal to pass to the method check)
                local_header_t lh;
                central_directory_header_t cdh;
                end_of_central_directory_t eocd;

                for(;;)
                {
                    // this is saved as a 16 bit value so it probably should
                    // support 16 bits
                    lh.m_compression_method = rand() & 0xFFFF;

                    // make sure it is not a valid method
                    bool found(false);
                    for(size_t m(0); m < sizeof(g_supported_storage_methods) / sizeof(g_supported_storage_methods[0]); ++m)
                    {
                        if(static_cast<zipios::StorageMethod>(lh.m_compression_method) == g_supported_storage_methods[m])
                        {
                            // it is valid, we will try again
                            // (it is going to be really rare, so we exclude
                            // these lines from the coverage)
                            found = true; // LCOV_EXCL_LINE
                            break; // LCOV_EXCL_LINE
                        }
                    }
                    if(!found)
                    {
                        break;
                    }
                } // LCOV_EXCL_LINE
                lh.m_filename = "invalid";
                lh.write(os);

                eocd.m_central_directory_offset = os.tellp();

                cdh.m_compression_method = lh.m_compression_method;
                cdh.m_filename = "invalid";
                cdh.write(os);

                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);
            }

            zipios::ZipFile zf("file.zip");
            REQUIRE_THROWS_AS(zf.getInputStream("invalid"), zipios::FileCollectionException);
        }
    }

    SECTION("create files with a trailing data descriptor")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // create a header (has to be equal to pass to the method check)
                local_header_t lh;
                central_directory_header_t cdh;
                end_of_central_directory_t eocd;

                // use a valid compression method
                lh.m_flags |= 1 << 3;  // <-- testing that trailing data is not supported!
                lh.m_compression_method = static_cast<uint16_t>(g_supported_storage_methods[rand() % (sizeof(g_supported_storage_methods) / sizeof(g_supported_storage_methods[0]))]);
                lh.m_filename = "invalid";
                lh.write(os);

                eocd.m_central_directory_offset = os.tellp();

                cdh.m_compression_method = lh.m_compression_method;
                cdh.m_flags = lh.m_flags;
                cdh.m_filename = "invalid";
                cdh.write(os);

                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);
            }

            zipios::ZipFile zf("file.zip");
            REQUIRE_THROWS_AS(zf.getInputStream("invalid"), zipios::FileCollectionException);
        }
    }

    /** \todo
     * We need to write a similar test that verifies each and every field
     * that proves there is an error and all the fields that do not prove
     * anything.
     */
    SECTION("create files with a mismatched compression method")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // create a header (has to be equal to pass to the method check)
                local_header_t lh;
                central_directory_header_t cdh;
                end_of_central_directory_t eocd;

                // use a valid compression method
                lh.m_compression_method = static_cast<uint16_t>(zipios::StorageMethod::STORED);
                lh.m_filename = "invalid";
                lh.write(os);

                eocd.m_central_directory_offset = os.tellp();

                cdh.m_compression_method = static_cast<uint16_t>(zipios::StorageMethod::DEFLATED);
                cdh.m_flags = lh.m_flags;
                cdh.m_filename = "invalid";
                cdh.write(os);

                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);
            }

            REQUIRE_THROWS_AS([&](){
                        zipios::ZipFile zf("file.zip");
                    }(), zipios::FileCollectionException);
        }
    }

    SECTION("create files with a trailing data descriptor")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // create a header (has to be equal to pass to the method check)
                local_header_t lh;
                central_directory_header_t cdh;
                end_of_central_directory_t eocd;

                // use a valid compression method
                lh.m_compression_method = static_cast<uint16_t>(g_supported_storage_methods[rand() % (sizeof(g_supported_storage_methods) / sizeof(g_supported_storage_methods[0]))]);
                lh.m_filename = "invalid";
                lh.write(os);

                eocd.m_central_directory_offset = os.tellp();

                cdh.m_compression_method = lh.m_compression_method;
                cdh.m_flags = lh.m_flags;
                cdh.m_filename = "invalid";
                cdh.write(os);

                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                if(i & 1)
                {
                    eocd.m_central_directory_size = 46 + 7 + rand() % 10 + 1; // structure + filename + erroneous size
                }
                else
                {
                    eocd.m_central_directory_size = 46 + 7 - rand() % 10 - 1; // structure + filename - erroneous size
                }
                eocd.write(os);
            }

            REQUIRE_THROWS_AS([&](){
                        zipios::ZipFile zf("file.zip");
                    }(), zipios::FileCollectionException);
        }
    }

/** \todo
 * Once clang is fixed, remove those tests. clang does not clear the
 * std::unchecked_exception() flag when we have a re-throw in a catch.
 * In this case we have a problem with the exception raised in
 * InflateInputStreambuf::underflow() when gzip finds an invalid
 * input stream.
 */
#ifndef __clang__
    SECTION("create files with a compressed file, only save only 50% of the data")
    {
        for(int i(0); i < 10; ++i)
        {
            // create an empty header in the file
            zipios_test::auto_unlink_t auto_unlink("file.zip");
            size_t uncompressed_size(0);
            {
                std::ofstream os("file.zip", std::ios::out | std::ios::binary);

                // create a header (has to be equal to pass to the method check)
                local_header_t lh;
                central_directory_header_t cdh;
                end_of_central_directory_t eocd;

                // create a file in a buffer then compress it
                // make sure the file is large enough to ensure that the
                // decompression fails "as expected" by this test
                typedef std::vector<Bytef> buffer_t;
                buffer_t file_buffer;
                size_t const file_size(rand() % (80 * 1024) + zipios::getBufferSize() * 3);
                for(size_t pos(0); pos < file_size; ++pos)
                {
                    file_buffer.push_back(static_cast<unsigned char>(rand()));
                }
                buffer_t compressed_buffer;
                uLongf compressed_size(file_size * 2);
                compressed_buffer.resize(compressed_size);
                compress2(&compressed_buffer[0], &compressed_size, &file_buffer[0], file_size, 9);
                compressed_buffer.resize(compressed_size); // the new size!
                std::fill(compressed_buffer.begin() + compressed_size / 2, compressed_buffer.end(), 0);

                // use a valid compression method
                lh.m_compression_method = static_cast<uint16_t>(zipios::StorageMethod::DEFLATED);
                lh.m_compressed_size = compressed_size - 2;
                lh.m_uncompressed_size = file_size;
                lh.m_crc32 = crc32(0L, &file_buffer[0], file_size);
                lh.m_filename = "invalid";
                lh.write(os);

                // write the first 50% of the compressed data then zeroes
                // make sure to skip the first 2 bytes which are the zlib
                // marker (0x78 0x9C)
                os.write(reinterpret_cast<char *>(&compressed_buffer[0]) + 2, (compressed_size - 2));

                eocd.m_central_directory_offset = os.tellp();

                cdh.m_compression_method = lh.m_compression_method;
                cdh.m_compressed_size = lh.m_compressed_size;
                cdh.m_uncompressed_size = lh.m_uncompressed_size;
                cdh.m_crc32 = lh.m_crc32;
                cdh.m_flags = lh.m_flags;
                cdh.m_filename = "invalid";
                cdh.write(os);

                eocd.m_file_count = 1;
                eocd.m_total_count = 1;
                eocd.m_central_directory_size = 46 + 7; // structure + filename
                eocd.write(os);

                // keep a copy of the uncompressed size to test after the
                // read stops
                uncompressed_size = file_size;
            }

            zipios::ZipFile zf("file.zip");
            // we cannot really know when exactly
            // we are going to get the throw
            zipios::ZipFile::stream_pointer_t in(zf.getInputStream("invalid"));
            size_t amount_read(0);
            do
            {
                char buf[BUFSIZ];
                in->read(buf, sizeof(buf));
                amount_read += in->gcount();
            }
            while(*in);
            REQUIRE(in->bad());
            REQUIRE(in->fail());
            REQUIRE(amount_read != uncompressed_size);
        }
    }
#endif
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
