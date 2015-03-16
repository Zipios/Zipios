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

#include <fstream>
//#include <memory>
//#include <vector>

#include <unistd.h>
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


TEST_CASE("A ZipFile with an invalid name", "[ZipFile] [FileCollection]")
{
    REQUIRE_THROWS_AS(zipios::ZipFile zf("this/file/does/not/exists/so/the/constructor/throws"), zipios::IOException);
}


TEST_CASE("A ZipFile with an invalid file", "[ZipFile] [FileCollection]")
{
    // create a totally random file which means there is still a very slight
    // chance that it represents a valid ZipFile, but frankly... no.
    {
        std::ofstream os("invalid.zip", std::ios::out | std::ios::binary);
        size_t const max_size(rand() % 1024 + 1024);
        for(size_t i(0); i < max_size; ++i)
        {
            os << static_cast<char>(rand());
        }
    }
    REQUIRE_THROWS_AS(zipios::ZipFile zf("invalid.zip"), zipios::FileCollectionException);
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


// For this one we have a set of structures and we "manually"
// create Zip archive files that we can thus tweak with totally
// invalid parameters
struct local_header_t
{
    typedef std::vector<unsigned char>      buffer_t;

    uint32_t            m_signature;            // "PK 3.4"
    uint16_t            m_version;              // 10 or 20
    uint16_t            m_flags;                // generally zero
    uint16_t            m_compression_method;   // zipios++ only supports STORED and DEFLATE
    uint32_t            m_time_and_date;        // MS-DOS date and time
    uint32_t            m_crc32;                // CRC-32 of the file data
    uint32_t            m_compressed_size;      // size of data once compressed
    uint32_t            m_uncompressed_size;    // size of data uncompressed
    //uint16_t            m_filename_length;     // length name of this file
    //uint16_t            m_extra_field_length;   // length of extra buffer, zipios++ ignore those
    //uint8_t             m_filename[m_filename_length];
    std::string         m_filename;
    //uint8_t             m_extra_field[m_extra_field_length];
    buffer_t            m_extra_field;

    local_header_t()
        : m_signature(0x04034B50)
        , m_version(10)
        , m_flags(0)
        , m_compression_method(0)   // 0 == STORED
        , m_time_and_date(unix2dostime(time(nullptr)))
        , m_crc32(0)
        , m_compressed_size(0)      // undefined is compression method is 0
        , m_uncompressed_size(0)
        //, m_filename("") -- auto-init
        //, m_extra_field() -- auto-init
    {
    }

    void write(std::ostream& os)
    {
        if(m_filename.empty())
        {
            std::cerr << "bug: local_header_t::write() called without a filename." << std::endl;
            exit(1);
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

            REQUIRE_THROWS_AS(zipios::ZipFile zf("file.zip"), zipios::FileCollectionException);
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

            REQUIRE_THROWS_AS(zipios::ZipFile zf("file.zip"), zipios::IOException);
        }
    }
}


// vim: ts=4 sw=4 et
