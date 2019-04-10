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
 * Zipios unit tests used to verify the VirtualSeeker class.
 */

#include "tests.hpp"

#include "zipios/virtualseeker.hpp"
#include "zipios/zipiosexceptions.hpp"

#include <fstream>

#include <unistd.h>


namespace
{


size_t const FOUR(4);


} // no name namespace


TEST_CASE("VirtualSeeker tests", "[zipios_common]")
{
    // create a file of 256 bytes
    zipios_test::auto_unlink_t auto_unlink("file256.bin");
    {
        std::ofstream os("file256.bin", std::ios::out | std::ios::binary);
        for(int i(0); i < 256; ++i)
        {
            os << static_cast<char>(i);
        }
    }

    // reopen as read-only
    std::ifstream is("file256.bin", std::ios::out | std::ios::binary);
    char buf[256];

    for(int count(0); count < 256; ++count)
    {
        // make the start betwee 0 and 200 so that we have some wiggle room
        // for the end offset
        //
        zipios::offset_t const start_offset(rand() % 200);
        zipios::offset_t const end_offset(start_offset + rand() % (256 - start_offset));
        REQUIRE(start_offset <= end_offset); // this should always be true
        zipios::offset_t const end(256 - end_offset);
        size_t const max_read(end_offset - start_offset);
        // note that the "gap" may be zero

        // attempt to create the seeker with invalid offsets
        REQUIRE_THROWS_AS([&](){
                        zipios::VirtualSeeker vs(start_offset, -end);
                    }(), zipios::InvalidException);
        REQUIRE_THROWS_AS([&](){
                        zipios::VirtualSeeker vs(-start_offset, -end);
                    }(), zipios::InvalidException);
        if(start_offset != 0)
        {
            REQUIRE_THROWS_AS([&](){
                            zipios::VirtualSeeker vs(-start_offset, end);
                        }(), zipios::InvalidException);
        }

        // the end parameter to the VirtualSeeker is a "weird" position
        zipios::VirtualSeeker vs(start_offset, end);

        {
            REQUIRE(vs.startOffset() == start_offset);
            REQUIRE(vs.endOffset() == end);

            zipios::offset_t start_test;
            zipios::offset_t end_test;
            vs.getOffsets(start_test, end_test);
            REQUIRE(start_test == start_offset);
            REQUIRE(end_test == end);
        }

        {
            vs.vseekg(is, 0, std::ios::beg);
            REQUIRE(is.tellg() == start_offset);
            REQUIRE(vs.vtellg(is) == 0);

            size_t const sz(std::min(max_read, FOUR));
            is.read(buf, sz);
            REQUIRE(is.tellg() == static_cast<zipios::offset_t>(start_offset + sz));
            REQUIRE(is);
            if(sz > 0)
            {
                REQUIRE(buf[0] == static_cast<char>(start_offset));
            }
            if(sz > 1)
            {
                REQUIRE(buf[1] == static_cast<char>(start_offset + 1));
            }
            if(sz > 2)
            {
                REQUIRE(buf[2] == static_cast<char>(start_offset + 2));
            }
            if(sz > 3)
            {
                REQUIRE(buf[3] == static_cast<char>(start_offset + 3));
            }

            // try moving a little more (if max_read allows it)
            if(max_read > 9UL)
            {
                vs.vseekg(is, 4, std::ios::cur);
                REQUIRE(is.tellg() == start_offset + 8);
                REQUIRE(vs.vtellg(is) == 8);

                size_t const sz2(std::min(max_read - 8UL, 4UL));
                is.read(buf, sz2);
                REQUIRE(is);
                if(sz2 > 0)
                {
                    REQUIRE(buf[0] == static_cast<char>(start_offset + 8));
                }
                if(sz2 > 1)
                {
                    REQUIRE(buf[1] == static_cast<char>(start_offset + 8 + 1));
                }
                if(sz2 > 2)
                {
                    REQUIRE(buf[2] == static_cast<char>(start_offset + 8 + 2));
                }
                if(sz2 > 3)
                {
                    REQUIRE(buf[3] == static_cast<char>(start_offset + 8 + 3));
                }
            }
        }

        {
            ssize_t const sz(std::min(max_read, FOUR));

            vs.vseekg(is, -sz, std::ios::end);
            std::streampos const expected_absolute_pos(end_offset - sz);
            REQUIRE(is.tellg() == expected_absolute_pos);
            std::streampos const expected_virtual_pos(end_offset - sz - start_offset);
            REQUIRE(vs.vtellg(is) == expected_virtual_pos);

            is.read(buf, sz);
            REQUIRE(is.tellg() == end_offset);
            REQUIRE(is);
            if(sz > 0)
            {
                REQUIRE(buf[0] == static_cast<char>(end_offset - sz));
            }
            if(sz > 1)
            {
                REQUIRE(buf[1] == static_cast<char>(end_offset - sz + 1));
            }
            if(sz > 2)
            {
                REQUIRE(buf[2] == static_cast<char>(end_offset - sz + 2));
            }
            if(sz > 3)
            {
                REQUIRE(buf[3] == static_cast<char>(end_offset - sz + 3));
            }

            // try moving a little more (if max_read allows it)
            if(max_read >= 9UL && max_read - 8UL >= static_cast<size_t>(start_offset))
            {
                ssize_t const sz2(std::min(max_read - 8UL, 4UL));

                vs.vseekg(is, -sz2 - sz, std::ios::cur);
                std::streampos const expected_absolute_pos2(end_offset - sz2 - sz);
                REQUIRE(is.tellg() == expected_absolute_pos2);
                std::streampos const expected_virtual_pos2(end_offset - sz2 - sz - start_offset);
                REQUIRE(vs.vtellg(is) == expected_virtual_pos2);

                is.read(buf, sz2);
                REQUIRE(is);
                if(sz2 > 0)
                {
                    REQUIRE(buf[0] == static_cast<char>(end_offset - sz2 - sz));
                }
                if(sz2 > 1)
                {
                    REQUIRE(buf[1] == static_cast<char>(end_offset - sz2 - sz + 1));
                }
                if(sz2 > 2)
                {
                    REQUIRE(buf[2] == static_cast<char>(end_offset - sz2 - sz + 2));
                }
                if(sz2 > 3)
                {
                    REQUIRE(buf[3] == static_cast<char>(end_offset - sz2 - sz + 3));
                }
            }
        }

        // change the offset and try again
        zipios::offset_t const start_offset2(rand() % 200);
        zipios::offset_t const end_offset2(start_offset2 + rand() % (256 - start_offset2));
        REQUIRE(start_offset2 <= end_offset2); // this should not happen, period!
        zipios::offset_t const end2(256 - end_offset2);
        size_t max_read2(end_offset2 - start_offset2);
        // note that the "gap" may be zero

        // try setting the offsets with invalid values
        REQUIRE_THROWS_AS(vs.setOffsets(-start_offset2, -end2), zipios::InvalidException);
        REQUIRE_THROWS_AS(vs.setOffsets(start_offset2, -end2), zipios::InvalidException);
        if(start_offset2 != 0)
        {
            REQUIRE_THROWS_AS(vs.setOffsets(-start_offset2, -end2), zipios::InvalidException);
        }

        // then change it to a valid value
        vs.setOffsets(start_offset2, end2);

        {
            REQUIRE(vs.startOffset() == start_offset2);
            REQUIRE(vs.endOffset() == end2);

            zipios::offset_t start_test2;
            zipios::offset_t end_test2;
            vs.getOffsets(start_test2, end_test2);
            REQUIRE(start_test2 == start_offset2);
            REQUIRE(end_test2 == end2);
        }

        for(int invalid_seek_direction(-100); invalid_seek_direction <= 100; ++invalid_seek_direction)
        {
            switch(invalid_seek_direction)
            {
            case std::ios::cur:
            case std::ios::beg:
            case std::ios::end:
                break;

            default:
                REQUIRE_THROWS_AS(vs.vseekg(is, 0, static_cast<std::ios::seekdir>(invalid_seek_direction)), std::logic_error);
                break;

            }
        }

        {
            vs.vseekg(is, 0, std::ios::beg);
            REQUIRE(vs.vtellg(is) == 0);

            size_t const sz(std::min(max_read2, FOUR));
            is.read(buf, sz);
            REQUIRE(is);
            if(sz > 0)
            {
                REQUIRE(buf[0] == static_cast<char>(start_offset2));
            }
            if(sz > 1)
            {
                REQUIRE(buf[1] == static_cast<char>(start_offset2 + 1));
            }
            if(sz > 2)
            {
                REQUIRE(buf[2] == static_cast<char>(start_offset2 + 2));
            }
            if(sz > 3)
            {
                REQUIRE(buf[3] == static_cast<char>(start_offset2 + 3));
            }
        }

        {
            ssize_t const sz(std::min(max_read2, FOUR));

            vs.vseekg(is, -sz, std::ios::end);
            std::streampos const expected_absolute_pos(end_offset2 - sz);
            REQUIRE(is.tellg() == expected_absolute_pos);
            std::streampos const expected_virtual_pos(end_offset2 - sz - start_offset2);
            REQUIRE(vs.vtellg(is) == expected_virtual_pos);

            is.read(buf, sz);
            REQUIRE(is);
            if(sz > 0)
            {
                REQUIRE(buf[0] == static_cast<char>(end_offset2 - sz));
            }
            if(sz > 1)
            {
                REQUIRE(buf[1] == static_cast<char>(end_offset2 - sz + 1));
            }
            if(sz > 2)
            {
                REQUIRE(buf[2] == static_cast<char>(end_offset2 - sz + 2));
            }
            if(sz > 3)
            {
                REQUIRE(buf[3] == static_cast<char>(end_offset2 - sz + 3));
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
