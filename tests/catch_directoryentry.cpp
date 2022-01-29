/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2021  Made to Order Software Corporation

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
 * Zipios unit tests for the DirectoryEntry class.
 */

#include "catch_main.hpp"

#include <zipios/directoryentry.hpp>
#include <zipios/zipiosexceptions.hpp>
#include <zipios/dosdatetime.hpp>

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>


/** \brief Local definitions used globally in the DirectoryEntry tests.
 *
 */
namespace
{


/** \brief Expected level defined as a variable.
 *
 * This definition is used because otherwise catch fails seeing the
 * definition as a global and the linkage fails.
 */
zipios::FileEntry::CompressionLevel const g_expected_level(zipios::FileEntry::COMPRESSION_LEVEL_DEFAULT);

/** \brief Directories make use of level 'none'.
 *
 * This value is to verify that the compression level of a directory
 * is properly defined.
 */
zipios::FileEntry::CompressionLevel const g_directory_level(zipios::FileEntry::COMPRESSION_LEVEL_NONE);


} // no name namespace


CATCH_SCENARIO("DirectoryEntry with invalid paths", "[DirectoryEntry] [FileEntry]")
{
    CATCH_GIVEN("test a fantom file (path that \"cannot\" exists) and no comment")
    {
        zipios::DirectoryEntry de(zipios::FilePath("/this/file/really/should/not/exist/period.txt"), "");

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            CATCH_REQUIRE(de.isEqual(de));
            CATCH_REQUIRE(de.getComment().empty());
            CATCH_REQUIRE(de.getCompressedSize() == 0);
            CATCH_REQUIRE(de.getCrc() == 0);
            CATCH_REQUIRE(de.getEntryOffset() == 0);
            CATCH_REQUIRE(de.getExtra().empty());
            CATCH_REQUIRE(de.getHeaderSize() == 0);
            CATCH_REQUIRE(de.getLevel() == g_expected_level);
            CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
            CATCH_REQUIRE(de.getFileName() == "period.txt");
            CATCH_REQUIRE(de.getSize() == 0);
            CATCH_REQUIRE(de.getTime() == 0);  // invalid date
            CATCH_REQUIRE(de.getUnixTime() == 0);
            CATCH_REQUIRE_FALSE(de.hasCrc());
            CATCH_REQUIRE_FALSE(de.isDirectory());
            CATCH_REQUIRE_FALSE(de.isValid());
            CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

            CATCH_REQUIRE_THROWS_AS(de.read(std::cin), zipios::IOException &);
            CATCH_REQUIRE_THROWS_AS(de.write(std::cout), zipios::IOException &);

            zipios::FileEntry::pointer_t null_entry;
//            CATCH_REQUIRE_FALSE(de.isEqual(*null_entry));  // here we are passing a NULL reference which most people think is something impossible to do...
            //CATCH_REQUIRE_FALSE(null_entry->isEqual(de)); -- that would obviously crash!

            zipios::DirectoryEntry empty(zipios::FilePath(""), "");
            CATCH_REQUIRE_FALSE(de.isEqual(empty));
            CATCH_REQUIRE_FALSE(empty.isEqual(de));

            // attempt a clone now, should have the same content
            zipios::DirectoryEntry::pointer_t clone(de.clone());

            CATCH_REQUIRE(clone->getComment().empty());
            CATCH_REQUIRE(clone->getCompressedSize() == 0);
            CATCH_REQUIRE(clone->getCrc() == 0);
            CATCH_REQUIRE(clone->getEntryOffset() == 0);
            CATCH_REQUIRE(clone->getExtra().empty());
            CATCH_REQUIRE(clone->getHeaderSize() == 0);
            CATCH_REQUIRE(clone->getLevel() == g_expected_level);
            CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
            CATCH_REQUIRE(clone->getFileName() == "period.txt");
            CATCH_REQUIRE(clone->getSize() == 0);
            CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
            CATCH_REQUIRE(clone->getUnixTime() == 0);
            CATCH_REQUIRE_FALSE(clone->hasCrc());
            CATCH_REQUIRE_FALSE(clone->isDirectory());
            CATCH_REQUIRE_FALSE(clone->isValid());
            CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            CATCH_REQUIRE(clone->isEqual(de));
            CATCH_REQUIRE(de.isEqual(*clone));
        }

        CATCH_WHEN("setting the comment")
        {
            de.setComment("new comment");

            CATCH_THEN("we can read it and nothing else changed")
            {
                CATCH_REQUIRE(de.getComment() == "new comment");
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("another/path"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment() == "new comment");
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_WHEN("setting the compressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setCompressedSize(r);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry same(zipios::FilePath("/this/file/really/should/not/exist/period.txt"), "");
                CATCH_REQUIRE(de.isEqual(same));
                CATCH_REQUIRE(same.isEqual(de));

                zipios::DirectoryEntry other(zipios::FilePath("this/file/really/should/not/exist/period.txt"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_WHEN("setting the CRC")
        {
            // zero would not really prove anything so skip such
            uint32_t r;
            do
            {
                r = rand();
            }
            while(r == 0);
            de.setCrc(rand());

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("/this/file/really/should/not/exist/period"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_WHEN("setting an extra buffer")
        {
            // zero would not really prove anything so skip such
            zipios::FileEntry::buffer_t b;
            uint32_t size(rand() % 100 + 20);
            for(uint32_t i(0); i < size; ++i)
            {
                b.push_back(rand());
            }
            de.setExtra(b);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(de.getExtra() == b);

                zipios::DirectoryEntry other(zipios::FilePath("period.txt"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->getExtra() == b);
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_START_SECTION("setting an invalid level")
        {
            for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
            {
                if(level >= -3 && level <= 100)
                {
                    // this is considered valid
                    de.setLevel(level);

                    CATCH_REQUIRE(de.getComment().empty());
                    CATCH_REQUIRE(de.getCompressedSize() == 0);
                    CATCH_REQUIRE(de.getCrc() == 0);
                    CATCH_REQUIRE(de.getEntryOffset() == 0);
                    CATCH_REQUIRE(de.getExtra().empty());
                    CATCH_REQUIRE(de.getHeaderSize() == 0);
                    CATCH_REQUIRE(de.getLevel() == level);
                    CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                    CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                    CATCH_REQUIRE(de.getFileName() == "period.txt");
                    CATCH_REQUIRE(de.getSize() == 0);
                    CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                    CATCH_REQUIRE(de.getUnixTime() == 0);
                    CATCH_REQUIRE_FALSE(de.hasCrc());
                    CATCH_REQUIRE_FALSE(de.isDirectory());
                    CATCH_REQUIRE_FALSE(de.isValid());
                    CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                    zipios::DirectoryEntry other(zipios::FilePath("/file/really/should/not/exist/period.txt"), "");
                    CATCH_REQUIRE_FALSE(de.isEqual(other));
                    CATCH_REQUIRE_FALSE(other.isEqual(de));

                    // attempt a clone now, should have the same content
                    zipios::DirectoryEntry::pointer_t clone(de.clone());

                    CATCH_REQUIRE(clone->getComment().empty());
                    CATCH_REQUIRE(clone->getCompressedSize() == 0);
                    CATCH_REQUIRE(clone->getCrc() == 0);
                    CATCH_REQUIRE(clone->getEntryOffset() == 0);
                    CATCH_REQUIRE(clone->getExtra().empty());
                    CATCH_REQUIRE(clone->getHeaderSize() == 0);
                    CATCH_REQUIRE(clone->getLevel() == level);
                    CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                    CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                    CATCH_REQUIRE(clone->getFileName() == "period.txt");
                    CATCH_REQUIRE(clone->getSize() == 0);
                    CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                    CATCH_REQUIRE(clone->getUnixTime() == 0);
                    CATCH_REQUIRE_FALSE(clone->hasCrc());
                    CATCH_REQUIRE_FALSE(clone->isDirectory());
                    CATCH_REQUIRE_FALSE(clone->isValid());
                    CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                    CATCH_REQUIRE(clone->isEqual(de));
                    CATCH_REQUIRE(de.isEqual(*clone));
                }
                else
                {
                    CATCH_REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException &);
                }
            }
        }

        CATCH_START_SECTION("setting an invalid method")
        {
            // WARNING: the StorageMethod is a uint8_t so testing
            //          with negative and such would wrap the number...
            //          (i.e. no need to do more than the following)
            for(int i(0); i < 256; ++i)
            {
                switch(i)
                {
                case 0: // Stored
                case 8: // Deflated
                    break;

                default:
                    CATCH_REQUIRE_THROWS_AS(de.setMethod(static_cast<zipios::StorageMethod>(i)), zipios::InvalidStateException &);
                    break;

                }
            }
        }

        CATCH_WHEN("setting the method")
        {
            // set a method other than STORED, which is 0
            // in the newer version, though, the set only accepts
            // with STORED and DEFLATED
            //
            zipios::StorageMethod storage_method(zipios::StorageMethod::DEFLATED);
            de.setMethod(storage_method);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == storage_method);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("/file/really/should/not/exist/period.txt"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == storage_method);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_WHEN("setting the uncompressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setSize(r);

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == r);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == r);
                CATCH_REQUIRE(de.getTime() == 0);  // invalid date
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (" + std::to_string(r) + " bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("really/.should"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == r);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == r);
                CATCH_REQUIRE(clone->getTime() == 0);  // invalid date
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (" + std::to_string(r) + " bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

#if INTPTR_MAX != INT32_MAX
// at this time only check on 64 bit computers because the DOS date can
// go out of range in a Unix date when we're on a 32 bit computer
        CATCH_WHEN("setting the DOS time")
        {
            // DOS time numbers are not linear so we test until we get one
            // that works...
            //
            std::time_t t((static_cast<std::time_t>(zipios_test::rand_size_t())) % (4354848000LL - 315561600LL) + 315561600);
            zipios::DOSDateTime r;
            r.setUnixTimestamp(t);
            de.setTime(r.getDOSDateTime());

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r.getUnixTimestamp());
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("other-name.txt"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }

        CATCH_WHEN("setting the Unix time")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            std::time_t r((static_cast<std::time_t>(zipios_test::rand_size_t())) % (4354848000LL - 315561600LL) + 315561600);
            de.setUnixTime(r);

            zipios::DOSDateTime dt;
            dt.setUnixTimestamp(r);

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("path/incorrect"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }
#endif

        CATCH_WHEN("setting the entry offset")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            std::streampos r(zipios_test::rand_size_t());
            de.setEntryOffset(r);

            CATCH_THEN("we retrieve the same value")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == r);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(de.getFileName() == "period.txt");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == 0);
                CATCH_REQUIRE(de.getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE_FALSE(de.isValid());
                CATCH_REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("path/incorrect"), "");
                CATCH_REQUIRE_FALSE(de.isEqual(other));
                CATCH_REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == r);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                CATCH_REQUIRE(clone->getFileName() == "period.txt");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == 0);
                CATCH_REQUIRE(clone->getUnixTime() == 0);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE_FALSE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                CATCH_REQUIRE(clone->isEqual(de));
                CATCH_REQUIRE(de.isEqual(*clone));
            }
        }
    }
}


CATCH_TEST_CASE("DirectoryEntry with one valid file", "[DirectoryEntry] [FileEntry]")
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
            CATCH_REQUIRE(stat("filepath-test.txt", &file_stats) == 0);
            zipios::DOSDateTime dt;
            dt.setUnixTimestamp(file_stats.st_mtime);

            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            {
                de.setComment("new comment");

                CATCH_REQUIRE(de.getComment() == "new comment");
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment() == "new comment");
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                de.setComment("");
            }

            {
                // zero would not really prove anything so skip such
                // (although it may be extremely rare...)
                size_t r;
                do
                {
                    r = zipios_test::rand_size_t();
                }
                while(r == 0);
                de.setCompressedSize(r);

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            {
                // zero would not really prove anything so skip such
                uint32_t r;
                do
                {
                    r = rand();
                }
                while(r == 0);
                de.setCrc(rand());

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
            }

            {
                // zero would not really prove anything so skip such
                zipios::FileEntry::buffer_t b;
                uint32_t size(rand() % 100 + 20);
                for(uint32_t j(0); j < size; ++j)
                {
                    b.push_back(rand());
                }
                de.setExtra(b);

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
                CATCH_REQUIRE(de.getExtra() == b);

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
                CATCH_REQUIRE(clone->getExtra() == b);

                // reset the buffer
                de.setExtra(zipios::FileEntry::buffer_t());
            }

            CATCH_START_SECTION("setting all levels, including many invalid ones")
            {
                for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
                {
                    if(level >= -3 && level <= 100)
                    {
                        // this is considered valid for a standard file
                        de.setLevel(level);

                        CATCH_REQUIRE(de.getComment().empty());
                        CATCH_REQUIRE(de.getCompressedSize() == file_size);
                        CATCH_REQUIRE(de.getCrc() == 0);
                        CATCH_REQUIRE(de.getEntryOffset() == 0);
                        CATCH_REQUIRE(de.getExtra().empty());
                        CATCH_REQUIRE(de.getHeaderSize() == 0);
                        CATCH_REQUIRE(de.getLevel() == level);
                        CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                        CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                        CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                        CATCH_REQUIRE(de.getSize() == file_size);
                        CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                        CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                        CATCH_REQUIRE_FALSE(de.hasCrc());
                        CATCH_REQUIRE_FALSE(de.isDirectory());
                        CATCH_REQUIRE(de.isValid());
                        CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
                    }
                    else
                    {
                        CATCH_REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException &);
                    }
                }

                // restore before continuing test
                de.setLevel(g_expected_level);
            }

            {
                // set a method other than STORED, which is 1, so just us % 8 instead of % 9 and do a +1
                de.setMethod(zipios::StorageMethod::DEFLATED);

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            {
                // zero would not really prove anything so skip such
                // (although it may be extremely rare...)
                size_t r;
                {
                    r = zipios_test::rand_size_t();
                }
                while(r == 0);
                de.setSize(r);

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == r);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == r);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(r) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == r);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == r);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(r) + " bytes)");

                de.setSize(file_size);
            }

#if INTPTR_MAX != INT32_MAX
// at this time only check on 64 bit computers because the DOS date can
// go out of range in a Unix date when we're on a 32 bit computer
            {
                // DOS time numbers are not linear so we use a Unix date and
                // convert to DOS time (since we know our converter works)
                //
                // Jan 1, 1980 at 00:00:00  is  315561600   (min)
                // Dec 31, 2107 at 23:59:59  is 4354847999  (max)
                std::time_t t(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
                zipios::DOSDateTime r;
                r.setUnixTimestamp(t);
                de.setTime(r.getDOSDateTime());

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r.getUnixTimestamp()); // WARNING: this is not always equal to t because setTime() may use the next even second
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            {
                // DOS time are limited to a smaller range and on every other
                // second so we get a valid DOS time and convert it to a Unix time
                std::time_t r(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
                zipios::DOSDateTime dr;
                dr.setUnixTimestamp(r);
                de.setUnixTime(r);

                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == file_size);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_expected_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(de.getName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(de.getSize() == file_size);
                CATCH_REQUIRE(de.getTime() == dr.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE_FALSE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == file_size);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_expected_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                CATCH_REQUIRE(clone->getName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test.txt");
                CATCH_REQUIRE(clone->getSize() == file_size);
                CATCH_REQUIRE(clone->getTime() == dr.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE_FALSE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }
#endif
        }

        unlink("filepath-test.txt");
    }
}


CATCH_SCENARIO("DirectoryEntry for a valid directory", "[DirectoryEntry] [FileEntry]")
{
    CATCH_GIVEN("test an existing directory and no comment")
    {
        // make sure the directory is gone before re-creating it
        CATCH_REQUIRE(system("rm -rf filepath-test") == 0);

        // create a directory
        CATCH_REQUIRE(mkdir("filepath-test", 0777) == 0);

        zipios::DirectoryEntry de(zipios::FilePath("filepath-test"), "");

        struct stat file_stats;
        CATCH_REQUIRE(stat("filepath-test", &file_stats) == 0);

        zipios::DOSDateTime dt;
        dt.setUnixTimestamp(file_stats.st_mtime);

        // first, check that the object is setup as expected
        CATCH_START_SECTION("verify that the object looks as expected")
        {
            CATCH_REQUIRE(de.getComment().empty());
            CATCH_REQUIRE(de.getCompressedSize() == 0);
            CATCH_REQUIRE(de.getCrc() == 0);
            CATCH_REQUIRE(de.getEntryOffset() == 0);
            CATCH_REQUIRE(de.getExtra().empty());
            CATCH_REQUIRE(de.getHeaderSize() == 0);
            CATCH_REQUIRE(de.getLevel() == g_directory_level);
            CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            CATCH_REQUIRE(de.getName() == "filepath-test");
            CATCH_REQUIRE(de.getFileName() == "filepath-test");
            CATCH_REQUIRE(de.getSize() == 0);
            CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
            CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
            CATCH_REQUIRE_FALSE(de.hasCrc());
            CATCH_REQUIRE(de.isDirectory());
            CATCH_REQUIRE(de.isValid());
            CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

            // attempt a clone now, should have the same content
            zipios::DirectoryEntry::pointer_t clone(de.clone());

            CATCH_REQUIRE(clone->getComment().empty());
            CATCH_REQUIRE(clone->getCompressedSize() == 0);
            CATCH_REQUIRE(clone->getCrc() == 0);
            CATCH_REQUIRE(clone->getEntryOffset() == 0);
            CATCH_REQUIRE(clone->getExtra().empty());
            CATCH_REQUIRE(clone->getHeaderSize() == 0);
            CATCH_REQUIRE(clone->getLevel() == g_directory_level);
            CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            CATCH_REQUIRE(clone->getName() == "filepath-test");
            CATCH_REQUIRE(clone->getFileName() == "filepath-test");
            CATCH_REQUIRE(clone->getSize() == 0);
            CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
            CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
            CATCH_REQUIRE_FALSE(clone->hasCrc());
            CATCH_REQUIRE(clone->isDirectory());
            CATCH_REQUIRE(clone->isValid());
            CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
        }

        CATCH_WHEN("setting the comment")
        {
            de.setComment("new comment");

            CATCH_THEN("we can read it and nothing else changed")
            {
                CATCH_REQUIRE(de.getComment() == "new comment");
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment() == "new comment");
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        CATCH_WHEN("setting the compressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setCompressedSize(r);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        CATCH_WHEN("setting the CRC")
        {
            // zero would not really prove anything so skip such
            uint32_t r;
            do
            {
                r = rand();
            }
            while(r == 0);
            de.setCrc(rand());

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        CATCH_WHEN("setting an extra buffer")
        {
            // zero would not really prove anything so skip such
            zipios::FileEntry::buffer_t b;
            uint32_t size(rand() % 100 + 20);
            for(uint32_t i(0); i < size; ++i)
            {
                b.push_back(rand());
            }
            de.setExtra(b);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");
                CATCH_REQUIRE(de.getExtra() == b);

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE_FALSE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
                CATCH_REQUIRE(clone->getExtra() == b);
            }
        }

        CATCH_START_SECTION("setting all levels, including many invalid ones")
        {
            for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
            {
                // directories do not accept values from 1 to 100
                if(level >= -3 && level <= 0)
                {
                    // this is considered valid for a standard file
                    de.setLevel(level);

                    CATCH_REQUIRE(de.getComment().empty());
                    CATCH_REQUIRE(de.getCompressedSize() == 0);
                    CATCH_REQUIRE(de.getCrc() == 0);
                    CATCH_REQUIRE(de.getEntryOffset() == 0);
                    CATCH_REQUIRE(de.getExtra().empty());
                    CATCH_REQUIRE(de.getHeaderSize() == 0);
                    CATCH_REQUIRE(de.getLevel() == g_directory_level);
                    CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                    CATCH_REQUIRE(de.getName() == "filepath-test");
                    CATCH_REQUIRE(de.getFileName() == "filepath-test");
                    CATCH_REQUIRE(de.getSize() == 0);
                    CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                    CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                    CATCH_REQUIRE_FALSE(de.hasCrc());
                    CATCH_REQUIRE(de.isDirectory());
                    CATCH_REQUIRE(de.isValid());
                    CATCH_REQUIRE(de.toString() == "filepath-test (directory)");
                }
                else
                {
                    CATCH_REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException &);
                }
            }

            // restore before continuing test
            de.setLevel(g_expected_level);
        }

        CATCH_WHEN("setting the method")
        {
            // set to DEFLATED which has no effect because the de is a
            // directory and directories accept DEFLATED but ignore it
            de.setMethod(zipios::StorageMethod::DEFLATED);

            CATCH_THEN("we ignore it")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        CATCH_WHEN("setting the uncompressed size")
        {
            // zero would not really prove anything so skip such
            // (although it may be extremely rare...)
            size_t r;
            do
            {
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setSize(r);

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == r);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == r);
                CATCH_REQUIRE(de.getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == r);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == r);
                CATCH_REQUIRE(clone->getTime() == dt.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

#if INTPTR_MAX != INT32_MAX
// at this time only check on 64 bit computers because the DOS date can
// go out of range in a Unix date when we're on a 32 bit computer
        CATCH_WHEN("setting the DOS time")
        {
            // DOS time numbers are not linear so we test until we get one
            // that works...
            std::time_t t(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
            zipios::DOSDateTime r;
            r.setUnixTimestamp(t);
            de.setTime(r.getDOSDateTime());

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r.getUnixTimestamp());
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == r.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

        CATCH_WHEN("setting the Unix time")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            std::time_t r(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
            de.setUnixTime(r);

            zipios::DOSDateTime dr;
            dr.setUnixTimestamp(r);

            CATCH_THEN("we take it as is")
            {
                CATCH_REQUIRE(de.getComment().empty());
                CATCH_REQUIRE(de.getCompressedSize() == 0);
                CATCH_REQUIRE(de.getCrc() == 0);
                CATCH_REQUIRE(de.getEntryOffset() == 0);
                CATCH_REQUIRE(de.getExtra().empty());
                CATCH_REQUIRE(de.getHeaderSize() == 0);
                CATCH_REQUIRE(de.getLevel() == g_directory_level);
                CATCH_REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(de.getName() == "filepath-test");
                CATCH_REQUIRE(de.getFileName() == "filepath-test");
                CATCH_REQUIRE(de.getSize() == 0);
                CATCH_REQUIRE(de.getTime() == dr.getDOSDateTime());
                CATCH_REQUIRE(de.getUnixTime() == r);
                CATCH_REQUIRE_FALSE(de.hasCrc());
                CATCH_REQUIRE(de.isDirectory());
                CATCH_REQUIRE(de.isValid());
                CATCH_REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                CATCH_REQUIRE(clone->getComment().empty());
                CATCH_REQUIRE(clone->getCompressedSize() == 0);
                CATCH_REQUIRE(clone->getCrc() == 0);
                CATCH_REQUIRE(clone->getEntryOffset() == 0);
                CATCH_REQUIRE(clone->getExtra().empty());
                CATCH_REQUIRE(clone->getHeaderSize() == 0);
                CATCH_REQUIRE(clone->getLevel() == g_directory_level);
                CATCH_REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                CATCH_REQUIRE(clone->getName() == "filepath-test");
                CATCH_REQUIRE(clone->getFileName() == "filepath-test");
                CATCH_REQUIRE(clone->getSize() == 0);
                CATCH_REQUIRE(clone->getTime() == dr.getDOSDateTime());
                CATCH_REQUIRE(clone->getUnixTime() == r);
                CATCH_REQUIRE_FALSE(clone->hasCrc());
                CATCH_REQUIRE(clone->isDirectory());
                CATCH_REQUIRE(clone->isValid());
                CATCH_REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }
#endif

        rmdir("filepath-test");
    }
}




// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
