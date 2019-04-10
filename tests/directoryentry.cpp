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
 * Zipios unit tests for the DirectoryEntry class.
 */

#include "tests.hpp"

#include "zipios/directoryentry.hpp"
#include "zipios/zipiosexceptions.hpp"
#include "zipios/dosdatetime.hpp"

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


SCENARIO("DirectoryEntry with invalid paths", "[DirectoryEntry] [FileEntry]")
{
    GIVEN("test a fantom file (path that \"cannot\" exists) and no comment")
    {
        zipios::DirectoryEntry de(zipios::FilePath("/this/file/really/should/not/exist/period.txt"), "");

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE(de.isEqual(de));
            REQUIRE(de.getComment().empty());
            REQUIRE(de.getCompressedSize() == 0);
            REQUIRE(de.getCrc() == 0);
            REQUIRE(de.getEntryOffset() == 0);
            REQUIRE(de.getExtra().empty());
            REQUIRE(de.getHeaderSize() == 0);
            REQUIRE(de.getLevel() == g_expected_level);
            REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
            REQUIRE(de.getFileName() == "period.txt");
            REQUIRE(de.getSize() == 0);
            REQUIRE(de.getTime() == 0);  // invalid date
            REQUIRE(de.getUnixTime() == 0);
            REQUIRE_FALSE(de.hasCrc());
            REQUIRE_FALSE(de.isDirectory());
            REQUIRE_FALSE(de.isValid());
            REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

            REQUIRE_THROWS_AS(de.read(std::cin), zipios::IOException);
            REQUIRE_THROWS_AS(de.write(std::cout), zipios::IOException);

            zipios::FileEntry::pointer_t null_entry;
//            REQUIRE_FALSE(de.isEqual(*null_entry));  // here we are passing a NULL reference which most people think is something impossible to do...
            //REQUIRE_FALSE(null_entry->isEqual(de)); -- that would obviously crash!

            zipios::DirectoryEntry empty(zipios::FilePath(""), "");
            REQUIRE_FALSE(de.isEqual(empty));
            REQUIRE_FALSE(empty.isEqual(de));

            // attempt a clone now, should have the same content
            zipios::DirectoryEntry::pointer_t clone(de.clone());

            REQUIRE(clone->getComment().empty());
            REQUIRE(clone->getCompressedSize() == 0);
            REQUIRE(clone->getCrc() == 0);
            REQUIRE(clone->getEntryOffset() == 0);
            REQUIRE(clone->getExtra().empty());
            REQUIRE(clone->getHeaderSize() == 0);
            REQUIRE(clone->getLevel() == g_expected_level);
            REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
            REQUIRE(clone->getFileName() == "period.txt");
            REQUIRE(clone->getSize() == 0);
            REQUIRE(clone->getTime() == 0);  // invalid date
            REQUIRE(clone->getUnixTime() == 0);
            REQUIRE_FALSE(clone->hasCrc());
            REQUIRE_FALSE(clone->isDirectory());
            REQUIRE_FALSE(clone->isValid());
            REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
            REQUIRE(clone->isEqual(de));
            REQUIRE(de.isEqual(*clone));
        }

        WHEN("setting the comment")
        {
            de.setComment("new comment");

            THEN("we can read it and nothing else changed")
            {
                REQUIRE(de.getComment() == "new comment");
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("another/path"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment() == "new comment");
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        WHEN("setting the compressed size")
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

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry same(zipios::FilePath("/this/file/really/should/not/exist/period.txt"), "");
                REQUIRE(de.isEqual(same));
                REQUIRE(same.isEqual(de));

                zipios::DirectoryEntry other(zipios::FilePath("this/file/really/should/not/exist/period.txt"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
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
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("/this/file/really/should/not/exist/period"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
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
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE_FALSE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(de.getExtra() == b);

                zipios::DirectoryEntry other(zipios::FilePath("period.txt"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE_FALSE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->getExtra() == b);
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        SECTION("setting an invalid level")
        {
            for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
            {
                if(level >= -3 && level <= 100)
                {
                    // this is considered valid
                    de.setLevel(level);

                    REQUIRE(de.getComment().empty());
                    REQUIRE(de.getCompressedSize() == 0);
                    REQUIRE(de.getCrc() == 0);
                    REQUIRE(de.getEntryOffset() == 0);
                    REQUIRE(de.getExtra().empty());
                    REQUIRE(de.getHeaderSize() == 0);
                    REQUIRE(de.getLevel() == level);
                    REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                    REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                    REQUIRE(de.getFileName() == "period.txt");
                    REQUIRE(de.getSize() == 0);
                    REQUIRE(de.getTime() == 0);  // invalid date
                    REQUIRE(de.getUnixTime() == 0);
                    REQUIRE_FALSE(de.hasCrc());
                    REQUIRE_FALSE(de.isDirectory());
                    REQUIRE_FALSE(de.isValid());
                    REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                    zipios::DirectoryEntry other(zipios::FilePath("/file/really/should/not/exist/period.txt"), "");
                    REQUIRE_FALSE(de.isEqual(other));
                    REQUIRE_FALSE(other.isEqual(de));

                    // attempt a clone now, should have the same content
                    zipios::DirectoryEntry::pointer_t clone(de.clone());

                    REQUIRE(clone->getComment().empty());
                    REQUIRE(clone->getCompressedSize() == 0);
                    REQUIRE(clone->getCrc() == 0);
                    REQUIRE(clone->getEntryOffset() == 0);
                    REQUIRE(clone->getExtra().empty());
                    REQUIRE(clone->getHeaderSize() == 0);
                    REQUIRE(clone->getLevel() == level);
                    REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                    REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                    REQUIRE(clone->getFileName() == "period.txt");
                    REQUIRE(clone->getSize() == 0);
                    REQUIRE(clone->getTime() == 0);  // invalid date
                    REQUIRE(clone->getUnixTime() == 0);
                    REQUIRE_FALSE(clone->hasCrc());
                    REQUIRE_FALSE(clone->isDirectory());
                    REQUIRE_FALSE(clone->isValid());
                    REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                    REQUIRE(clone->isEqual(de));
                    REQUIRE(de.isEqual(*clone));
                }
                else
                {
                    REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException);
                }
            }
        }

        SECTION("setting an invalid method")
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
                    REQUIRE_THROWS_AS(de.setMethod(static_cast<zipios::StorageMethod>(i)), zipios::InvalidStateException);
                    break;

                }
            }
        }

        WHEN("setting the method")
        {
            // set a method other than STORED, which is 0
            // in the newer version, though, the set only accepts
            // with STORED and DEFLATED
            //
            zipios::StorageMethod storage_method(zipios::StorageMethod::DEFLATED);
            de.setMethod(storage_method);

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == storage_method);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);  // invalid date
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("/file/really/should/not/exist/period.txt"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == storage_method);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);  // invalid date
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        WHEN("setting the uncompressed size")
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

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
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

                zipios::DirectoryEntry other(zipios::FilePath("really/.should"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == r);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
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
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        WHEN("setting the DOS time")
        {
            // DOS time numbers are not linear so we test until we get one
            // that works...
            //
            std::time_t t((static_cast<std::time_t>(zipios_test::rand_size_t())) % (4354848000LL - 315561600LL) + 315561600);
            zipios::DOSDateTime r;
            r.setUnixTimestamp(t);
            de.setTime(r.getDOSDateTime());

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r.getDOSDateTime());
                REQUIRE(de.getUnixTime() == r.getUnixTimestamp());
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(!de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("other-name.txt"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        WHEN("setting the Unix time")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            std::time_t r((static_cast<std::time_t>(zipios_test::rand_size_t())) % (4354848000LL - 315561600LL) + 315561600);
            de.setUnixTime(r);

            zipios::DOSDateTime dt;
            dt.setUnixTimestamp(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == r);
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(!de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("path/incorrect"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == r);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(!clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }

        WHEN("setting the entry offset")
        {
            // DOS time are limited to a smaller range and on every other
            // second so we get a valid DOS time and convert it to a Unix time
            std::streampos r(zipios_test::rand_size_t());
            de.setEntryOffset(r);

            THEN("we retrieve the same value")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == r);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(de.getFileName() == "period.txt");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == 0);
                REQUIRE(de.getUnixTime() == 0);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE_FALSE(de.isValid());
                REQUIRE(de.toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");

                zipios::DirectoryEntry other(zipios::FilePath("path/incorrect"), "");
                REQUIRE_FALSE(de.isEqual(other));
                REQUIRE_FALSE(other.isEqual(de));

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == r);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "/this/file/really/should/not/exist/period.txt");
                REQUIRE(clone->getFileName() == "period.txt");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == 0);
                REQUIRE(clone->getUnixTime() == 0);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE(clone->toString() == "/this/file/really/should/not/exist/period.txt (0 bytes)");
                REQUIRE(clone->isEqual(de));
                REQUIRE(de.isEqual(*clone));
            }
        }
    }
}


TEST_CASE("DirectoryEntry with one valid file", "[DirectoryEntry] [FileEntry]")
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
            zipios::DOSDateTime dt;
            dt.setUnixTimestamp(file_stats.st_mtime);

            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            {
                de.setComment("new comment");

                REQUIRE(de.getComment() == "new comment");
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");

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

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
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

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
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

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE_FALSE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
                REQUIRE(de.getExtra() == b);

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE_FALSE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_stats.st_size) + " bytes)");
                REQUIRE(clone->getExtra() == b);

                // reset the buffer
                de.setExtra(zipios::FileEntry::buffer_t());
            }

            SECTION("setting all levels, including many invalid ones")
            {
                for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
                {
                    if(level >= -3 && level <= 100)
                    {
                        // this is considered valid for a standard file
                        de.setLevel(level);

                        REQUIRE(de.getComment().empty());
                        REQUIRE(de.getCompressedSize() == file_size);
                        REQUIRE(de.getCrc() == 0);
                        REQUIRE(de.getEntryOffset() == 0);
                        REQUIRE(de.getExtra().empty());
                        REQUIRE(de.getHeaderSize() == 0);
                        REQUIRE(de.getLevel() == level);
                        REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                        REQUIRE(de.getName() == "filepath-test.txt");
                        REQUIRE(de.getFileName() == "filepath-test.txt");
                        REQUIRE(de.getSize() == file_size);
                        REQUIRE(de.getTime() == dt.getDOSDateTime());
                        REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE(de.hasCrc());
                        REQUIRE_FALSE(de.isDirectory());
                        REQUIRE(de.isValid());
                        REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
                    }
                    else
                    {
                        REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException);
                    }
                }

                // restore before continuing test
                de.setLevel(g_expected_level);
            }

            {
                // set a method other than STORED, which is 1, so just us % 8 instead of % 9 and do a +1
                de.setMethod(zipios::StorageMethod::DEFLATED);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE_FALSE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(clone->hasCrc());
                REQUIRE_FALSE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
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

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == r);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == r);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(r) + " bytes)");

                de.setSize(file_size);
            }

            {
                // DOS time numbers are not linear so we use a Unix date and
                // convert to DOS time (since we know our convertor works)
                //
                // Jan 1, 1980 at 00:00:00  is  315561600   (min)
                // Dec 31, 2107 at 23:59:59  is 4354847999  (max)
                std::time_t t(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
                zipios::DOSDateTime r;
                r.setUnixTimestamp(t);
                de.setTime(r.getDOSDateTime());

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == r.getDOSDateTime());
                REQUIRE(de.getUnixTime() == r.getUnixTimestamp()); // WARNING: this is not always equal to t because setTime() may use the next even second
                REQUIRE(!de.hasCrc());
                REQUIRE(!de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == file_size);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == r.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
                REQUIRE(!clone->hasCrc());
                REQUIRE(!clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test.txt (" + std::to_string(file_size) + " bytes)");
            }

            {
                // DOS time are limited to a smaller range and on every other
                // second so we get a valid DOS time and convert it to a Unix time
                std::time_t r(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
                zipios::DOSDateTime dr;
                dr.setUnixTimestamp(r);
                de.setUnixTime(r);

                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == file_size);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_expected_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(de.getName() == "filepath-test.txt");
                REQUIRE(de.getFileName() == "filepath-test.txt");
                REQUIRE(de.getSize() == file_size);
                REQUIRE(de.getTime() == dr.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_expected_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::DEFLATED);
                REQUIRE(clone->getName() == "filepath-test.txt");
                REQUIRE(clone->getFileName() == "filepath-test.txt");
                REQUIRE(clone->getSize() == file_size);
                REQUIRE(clone->getTime() == dr.getDOSDateTime());
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
        REQUIRE(system("rm -rf filepath-test") == 0);

        // create a directory
        REQUIRE(mkdir("filepath-test", 0777) == 0);

        zipios::DirectoryEntry de(zipios::FilePath("filepath-test"), "");

        struct stat file_stats;
        REQUIRE(stat("filepath-test", &file_stats) == 0);

        zipios::DOSDateTime dt;
        dt.setUnixTimestamp(file_stats.st_mtime);

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE(de.getComment().empty());
            REQUIRE(de.getCompressedSize() == 0);
            REQUIRE(de.getCrc() == 0);
            REQUIRE(de.getEntryOffset() == 0);
            REQUIRE(de.getExtra().empty());
            REQUIRE(de.getHeaderSize() == 0);
            REQUIRE(de.getLevel() == g_directory_level);
            REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(de.getName() == "filepath-test");
            REQUIRE(de.getFileName() == "filepath-test");
            REQUIRE(de.getSize() == 0);
            REQUIRE(de.getTime() == dt.getDOSDateTime());
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
            REQUIRE(clone->getEntryOffset() == 0);
            REQUIRE(clone->getExtra().empty());
            REQUIRE(clone->getHeaderSize() == 0);
            REQUIRE(clone->getLevel() == g_directory_level);
            REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
            REQUIRE(clone->getName() == "filepath-test");
            REQUIRE(clone->getFileName() == "filepath-test");
            REQUIRE(clone->getSize() == 0);
            REQUIRE(clone->getTime() == dt.getDOSDateTime());
            REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
            REQUIRE_FALSE(clone->hasCrc());
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
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment() == "new comment");
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(clone->hasCrc());
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
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setCompressedSize(r);

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(clone->hasCrc());
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
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
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
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE_FALSE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");
                REQUIRE(de.getExtra() == b);

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE_FALSE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
                REQUIRE(clone->getExtra() == b);
            }
        }

        SECTION("setting all levels, including many invalid ones")
        {
            for(zipios::FileEntry::CompressionLevel level(-1000); level <= 1000; ++level)
            {
                // directories do not accept values from 1 to 100
                if(level >= -3 && level <= 0)
                {
                    // this is considered valid for a standard file
                    de.setLevel(level);

                    REQUIRE(de.getComment().empty());
                    REQUIRE(de.getCompressedSize() == 0);
                    REQUIRE(de.getCrc() == 0);
                    REQUIRE(de.getEntryOffset() == 0);
                    REQUIRE(de.getExtra().empty());
                    REQUIRE(de.getHeaderSize() == 0);
                    REQUIRE(de.getLevel() == g_directory_level);
                    REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                    REQUIRE(de.getName() == "filepath-test");
                    REQUIRE(de.getFileName() == "filepath-test");
                    REQUIRE(de.getSize() == 0);
                    REQUIRE(de.getTime() == dt.getDOSDateTime());
                    REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                    REQUIRE_FALSE(de.hasCrc());
                    REQUIRE(de.isDirectory());
                    REQUIRE(de.isValid());
                    REQUIRE(de.toString() == "filepath-test (directory)");
                }
                else
                {
                    REQUIRE_THROWS_AS(de.setLevel(level), zipios::InvalidStateException);
                }
            }

            // restore before continuing test
            de.setLevel(g_expected_level);
        }

        WHEN("setting the method")
        {
            // set to DEFLATED which has no effect because the de is a
            // directory and directories accept DEFLATED but ignore it
            de.setMethod(zipios::StorageMethod::DEFLATED);

            THEN("we ignore it")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
                REQUIRE(de.getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == file_stats.st_mtime);
                REQUIRE_FALSE(clone->hasCrc());
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
                r = zipios_test::rand_size_t();
            }
            while(r == 0);
            de.setSize(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == r);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == r);
                REQUIRE(de.getTime() == dt.getDOSDateTime());
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
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == r);
                REQUIRE(clone->getTime() == dt.getDOSDateTime());
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
            std::time_t t(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
            zipios::DOSDateTime r;
            r.setUnixTimestamp(t);
            de.setTime(r.getDOSDateTime());

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == r.getDOSDateTime());
                REQUIRE(de.getUnixTime() == r.getUnixTimestamp());
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == r.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == r.getUnixTimestamp());
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
            std::time_t r(static_cast<std::time_t>(zipios_test::rand_size_t()) % (4354848000LL - 315561600LL) + 315561600);
            de.setUnixTime(r);

            zipios::DOSDateTime dr;
            dr.setUnixTimestamp(r);

            THEN("we take it as is")
            {
                REQUIRE(de.getComment().empty());
                REQUIRE(de.getCompressedSize() == 0);
                REQUIRE(de.getCrc() == 0);
                REQUIRE(de.getEntryOffset() == 0);
                REQUIRE(de.getExtra().empty());
                REQUIRE(de.getHeaderSize() == 0);
                REQUIRE(de.getLevel() == g_directory_level);
                REQUIRE(de.getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(de.getName() == "filepath-test");
                REQUIRE(de.getFileName() == "filepath-test");
                REQUIRE(de.getSize() == 0);
                REQUIRE(de.getTime() == dr.getDOSDateTime());
                REQUIRE(de.getUnixTime() == dr.getUnixTimestamp());
                REQUIRE(!de.hasCrc());
                REQUIRE(de.isDirectory());
                REQUIRE(de.isValid());
                REQUIRE(de.toString() == "filepath-test (directory)");

                // attempt a clone now, should have the same content
                zipios::DirectoryEntry::pointer_t clone(de.clone());

                REQUIRE(clone->getComment().empty());
                REQUIRE(clone->getCompressedSize() == 0);
                REQUIRE(clone->getCrc() == 0);
                REQUIRE(clone->getEntryOffset() == 0);
                REQUIRE(clone->getExtra().empty());
                REQUIRE(clone->getHeaderSize() == 0);
                REQUIRE(clone->getLevel() == g_directory_level);
                REQUIRE(clone->getMethod() == zipios::StorageMethod::STORED);
                REQUIRE(clone->getName() == "filepath-test");
                REQUIRE(clone->getFileName() == "filepath-test");
                REQUIRE(clone->getSize() == 0);
                REQUIRE(clone->getTime() == dr.getDOSDateTime());
                REQUIRE(clone->getUnixTime() == dr.getUnixTimestamp());
                REQUIRE(!clone->hasCrc());
                REQUIRE(clone->isDirectory());
                REQUIRE(clone->isValid());
                REQUIRE(clone->toString() == "filepath-test (directory)");
            }
        }

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
