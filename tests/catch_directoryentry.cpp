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

#include "src/dostime.h"

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

            // attempt a clone now, should have the same content
            zipios::DirectoryEntry::pointer_t clone(de.clone());

            REQUIRE(clone->getComment().empty());
            REQUIRE(clone->getCompressedSize() == 0);
            REQUIRE(clone->getCrc() == 0);
            REQUIRE(clone->getExtra().empty());
            REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
            REQUIRE(clone->getFileName() == "period.txt");
            REQUIRE(clone->getSize() == 0);
            REQUIRE(clone->getTime() == 0);  // invalid date
            REQUIRE(clone->getUnixTime() == 0);
            REQUIRE(!clone->hasCrc());
            REQUIRE(!clone->isDirectory());
            REQUIRE(!clone->isValid());
            REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
        }

        WHEN("setting the comment")
        {
            de.setComment("new comment");

            THEN("we can read it and nothing else changed")
            {
                REQUIRE(de.getComment() == "new comment");
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

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment() == "new comment");
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting the compressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
            }
            while(r == 0);
            de.setCompressedSize(r);

            THEN("we ignore it")
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

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting the CRC")
        {
            // zero would not really prove anything so skip such
            uint32_t r;
            do
            {
                r = rand();
            }
            while(r == 0);
            de.setCrc(rand());

            THEN("we ignore it")
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

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting an extra buffer")
        {
            // zero would not really prove anything so skip such
            zipios::FileEntry::buffer_t b;
            uint32_t size(rand() % 100 + 20);
            for(uint32_t i(0); i < size; ++i)
            {
                b.push_back(rand());
            }
            de.setExtra(b);

            THEN("we ignore it")
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

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting the method")
        {
            // set a method other than STORED, which is 1, so just us % 8 instead of % 9 and do a +1
            de.setMethod(static_cast<zipios::StorageMethod>(rand() % 8 + 1));

            THEN("we ignore it")
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

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting the uncompressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
            }
            while(r == 0);
            de.setSize(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == r);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(!de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (" + std::to_string(r) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == r);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == r);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (" + std::to_string(r) + " bytes)");
            }
        }

        WHEN("setting the DOS time")
        {
            // DOS time numbers are not linear so we test until we get one
            // that works...
            dostime_t r;
            do
            {
                r = static_cast<dostime_t>(rand());
            }
            while(dos2unixtime(r) == -1);
            de.setTime(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r);
                REQUIRE(de.getUnixTime() == dos2unixtime(r));
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(!de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r);
                REQUIRE(clone->getUnixTime() == dos2unixtime(r));
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }

        WHEN("setting the Unix time")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            dostime_t r;
            do
            {
                r = static_cast<dostime_t>(rand());
            }
            while(dos2unixtime(r) == -1);
            de.setUnixTime(dos2unixtime(r));

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r);
                REQUIRE(de.getUnixTime() == dos2unixtime(r));
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(!de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r);
                REQUIRE(clone->getUnixTime() == dos2unixtime(r));
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            }
        }
    }
}


TEST_CASE("DirectoryEntry with valid files", "[DirectoryEntry] [FileEntry]")
{
    for(int i(0); i < 10; ++i)
    {
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
            zipios::DirectoryEntry de(zipios::FilePath("filepath-test.txt"), "");

            struct stat file_stats;
            REQUIRE(stat("filepath-test.txt", &file_stats) == 0);

            SECTION("verify that the object looks as expected")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            SECTION("try setting the comment")
            {
                de.setComment("new comment");

                REQUIRE(de.getComment() == "new comment");
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment() == "new comment");
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            SECTION("setting the compressed size")
            {
                // zero would not really prove anything so skip such
                // (although it may be extremely rare...)
                size_t r;
                do
                {
                    r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
                }
                while(r == 0);
                de.setCompressedSize(r);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            SECTION("setting the CRC")
            {
                // zero would not really prove anything so skip such
                uint32_t r;
                do
                {
                    r = rand();
                }
                while(r == 0);
                de.setCrc(rand());

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            SECTION("setting an extra buffer")
            {
                // zero would not really prove anything so skip such
                zipios::FileEntry::buffer_t b;
                uint32_t size(rand() % 100 + 20);
                for(uint32_t j(0); j < size; ++j)
                {
                    b.push_back(rand());
                }
                de.setExtra(b);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            SECTION("setting the method")
            {
                // set a method other than STORED, which is 1, so just us % 8 instead of % 9 and do a +1
                de.setMethod(static_cast<zipios::StorageMethod>(rand() % 8 + 1));

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            SECTION("setting the uncompressed size")
            {
                // zero would not really prove anything so skip such
                // (although it may be extremely rare...)
                size_t r;
                {
                    r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
                }
                while(r == 0);
                de.setSize(r);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == r);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(r) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == r);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == r);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(r) + " bytes)");
            }

            SECTION("setting the DOS time")
            {
                // DOS time numbers are not linear so we use a Unix date and
                // convert to DOS time (since we know our convertor works)
                // 
                // Jan 1, 1980 at 00:00:00  is  315561600   (min)
                // Dec 31, 2107 at 23:59:59  is 4354847999  (max)
                time_t t((static_cast<time_t>(rand()) | (static_cast<time_t>(rand()) << 32)) % (4354848000LL - 315561600LL) + 315561600);
                dostime_t r(unix2dostime(t));
                de.setTime(r);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == r);
                REQUIRE(de.getUnixTime() == dos2unixtime(r)); // WARNING: r is not always equal to t because setTime() may use the next even second
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == r);
                REQUIRE(clone->getUnixTime() == dos2unixtime(r));
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            SECTION("setting the Unix time")
            {
                // DOS time are limited to a smaller range and on every other
                // second so we get a valid DOS time and convert it to a Unix time
                time_t r((static_cast<time_t>(rand()) | (static_cast<time_t>(rand()) << 32)) % (4354848000LL - 315561600LL) + 315561600);
                de.setUnixTime(r);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == unix2dostime(r));
                REQUIRE(de.getUnixTime() == r);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == unix2dostime(r));
                REQUIRE(clone->getUnixTime() == r);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }
        }

        unlink("filepath-test.txt");
    }
}


SCENARIO("DirectoryEntry for a valid directory", "[DirectoryEntry] [FileEntry]")
{
    GIVEN("test an existing directory and no comment")
    {
        // make sure the directory is gone before re-creating it
        static_cast<void>(system("rm -rf filepath-test"));

        // create a directory
        REQUIRE(mkdir("filepath-test", 0777) == 0);

        zipios::DirectoryEntry de(zipios::FilePath("filepath-test"), "");

        struct stat file_stats;
        REQUIRE(stat("filepath-test", &file_stats) == 0);

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE(de.getComment().empty());
            REQUIRE(de.getCompressedSize() == 0);
            REQUIRE(de.getCrc() == 0);
            REQUIRE(de.getExtra().empty());
            REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(de.getName() == "filepath-test");
            REQUIRE(de.getFileName() == "");
            REQUIRE(de.getSize() == 0);
            REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
            REQUIRE(de.getUnixTime() == file_stats.st_mtime);
            REQUIRE(!de.hasCrc());
            REQUIRE(de.isDirectory());
            REQUIRE(de.isValid());
            REQUIRE(de.toString() == "filepath-test (directory)");

            // attempt a clone now, should have the same content
            zipios::DirectoryEntry::pointer_t clone(de.clone());

            REQUIRE(clone->getComment().empty());
            REQUIRE(clone->getCompressedSize() == 0);
            REQUIRE(clone->getCrc() == 0);
            REQUIRE(clone->getExtra().empty());
            REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(clone->getName() == "filepath-test");
            REQUIRE(clone->getFileName() == "");
            REQUIRE(clone->getSize() == 0);
            REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
            REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
            REQUIRE(!clone->hasCrc());
            REQUIRE(clone->isDirectory());
            REQUIRE(clone->isValid());
            REQUIRE(clone->toString() == "filepath-test (directory)");
        }

        WHEN("setting the comment")
        {
            de.setComment("new comment");

            THEN("we can read it and nothing else changed")
            {
                REQUIRE(de.getComment() == "new comment");
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment() == "new comment");
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the compressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
            }
            while(r == 0);
            de.setCompressedSize(r);

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the CRC")
        {
            // zero would not really prove anything so skip such
            uint32_t r;
            do
            {
                r = rand();
            }
            while(r == 0);
            de.setCrc(rand());

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting an extra buffer")
        {
            // zero would not really prove anything so skip such
            zipios::FileEntry::buffer_t b;
            uint32_t size(rand() % 100 + 20);
            for(uint32_t i(0); i < size; ++i)
            {
                b.push_back(rand());
            }
            de.setExtra(b);

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the method")
        {
            // set a method other than STORED, which is 1, so just us % 8 instead of % 9 and do a +1
            de.setMethod(static_cast<zipios::StorageMethod>(rand() % 8 + 1));

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the uncompressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = static_cast<size_t>(rand()) | (static_cast<size_t>(rand()) << 32);
            }
            while(r == 0);
            de.setSize(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == r);
                REQUIRE(de.getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == r);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == r);
                REQUIRE(clone->getTime() == unix2dostime(file_stats.st_mtime));
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the DOS time")
        {
            // DOS time numbers are not linear so we test until we get one
            // that works...
            dostime_t r;
            do
            {
                r = static_cast<dostime_t>(rand());
            }
            while(dos2unixtime(r) == -1);
            de.setTime(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r);
                REQUIRE(de.getUnixTime() == dos2unixtime(r));
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r);
                REQUIRE(clone->getUnixTime() == dos2unixtime(r));
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        WHEN("setting the Unix time")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            dostime_t r;
            do
            {
                r = static_cast<dostime_t>(rand());
            }
            while(dos2unixtime(r) == -1);
            de.setUnixTime(dos2unixtime(r));

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r);
                REQUIRE(de.getUnixTime() == dos2unixtime(r));
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r);
                REQUIRE(clone->getUnixTime() == dos2unixtime(r));
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        rmdir("filepath-test");
    }
}




// vim: ts=4 sw=4 et
