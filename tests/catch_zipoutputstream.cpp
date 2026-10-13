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
 * Zipios unit tests for the ZipOutputStream class.
 */

#include "catch_main.hpp"

#include <zipios/directoryentry.hpp>
#include <zipios/zipfile.hpp>

#include <src/zipoutputstream.hpp>

#include <fstream>
#include <iterator>


namespace
{


zipios::FileEntry::pointer_t create_entry(std::string const & name, zipios::StorageMethod method)
{
    zipios::FileEntry::pointer_t entry(std::make_shared<zipios::DirectoryEntry>(zipios::FilePath(name)));
    entry->setMethod(method);
    entry->setLevel(method == zipios::StorageMethod::STORED
                        ? zipios::FileEntry::COMPRESSION_LEVEL_NONE
                        : zipios::FileEntry::COMPRESSION_LEVEL_DEFAULT);
    entry->setUnixTime(1700000000);
    return entry;
}


// write a deflated and a stored entry, with or without flushing the stream
void write_archive(std::ostream & out, bool flush)
{
    zipios::ZipOutputStream zip(out);

    zip.putNextEntry(create_entry("text.txt", zipios::StorageMethod::DEFLATED));
    for(int i(0); i < 1000; ++i)
    {
        zip << "line " << i;
        if(flush)
        {
            zip << std::endl;
        }
        else
        {
            zip << '\n';
        }
    }
    CATCH_REQUIRE(zip.good());

    zip.putNextEntry(create_entry("stored.txt", zipios::StorageMethod::STORED));
    zip << "stored data";
    if(flush)
    {
        zip.flush();
    }
    CATCH_REQUIRE(zip.good());

    zip.close();
}


std::string expected_text()
{
    std::string text;
    for(int i(0); i < 1000; ++i)
    {
        text += "line " + std::to_string(i) + "\n";
    }
    return text;
}


std::string read_all(std::istream & in)
{
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}


} // no name namespace


CATCH_SCENARIO("flush a ZipOutputStream", "[ZipOutputStream]")
{
    zipios_test::safe_chdir cwd(SNAP_CATCH2_NAMESPACE::g_tmp_dir());

    CATCH_GIVEN("an archive written with std::endl and flush()")
    {
        zipios_test::auto_unlink_t remove_zip("flushed.zip", true);
        {
            std::ofstream out("flushed.zip", std::ios::out | std::ios::binary);
            write_archive(out, true);
        }

        CATCH_THEN("no data is lost")
        {
            zipios::ZipFile zf("flushed.zip");
            CATCH_REQUIRE(zf.size() == 2);
            CATCH_REQUIRE(read_all(*zf.getInputStream("text.txt")) == expected_text());
            CATCH_REQUIRE(read_all(*zf.getInputStream("stored.txt")) == "stored data");
        }
    }

    CATCH_GIVEN("the same archive written with and without flushing")
    {
        std::ostringstream flushed(std::ios::out | std::ios::binary);
        write_archive(flushed, true);
        std::ostringstream not_flushed(std::ios::out | std::ios::binary);
        write_archive(not_flushed, false);

        CATCH_THEN("the flushes do not change the archive")
        {
            CATCH_REQUIRE(flushed.str() == not_flushed.str());
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
