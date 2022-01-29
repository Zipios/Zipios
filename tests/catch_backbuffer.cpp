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
  Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 *
 * Zipios unit tests for the BackBuffer class.
 */

#include "catch_main.hpp"

#include <src/backbuffer.hpp>
#include <zipios/zipiosexceptions.hpp>

#include <fstream>


CATCH_SCENARIO("BackBuffer read a file", "[BackBuffer]")
{
    CATCH_GIVEN("a binary file of 256 bytes")
    {
        // create a file of 256 bytes
        zipios_test::auto_unlink_t auto_unlink("file256.bin", true);
        {
            std::ofstream os("file256.bin", std::ios::out | std::ios::binary);
            for(int i(0); i < 256; ++i)
            {
                os << static_cast<char>(i);
            }
        }

        CATCH_WHEN("setting up a backbuffer")
        {
            std::ifstream is("file256.bin", std::ios::in | std::ios::binary);
            zipios::BackBuffer back_buffer(is, zipios::VirtualSeeker(), 16);

            CATCH_THEN("we can read the file going backward")
            {
                ssize_t read_pointer(0);
                for(int i(0), j(256 - 16); i < 256; i += 16, j -= 16)
                {
                    back_buffer.readChunk(read_pointer);
                    CATCH_REQUIRE(read_pointer == i + 16);
                    for(int k(0); k < 16; ++k)
                    {
                        CATCH_REQUIRE(back_buffer[k] == j + k);
                    }
                }
            }

            CATCH_THEN("try with an invalid chunk size")
            {
                for(int i(-16); i <= 0; ++i)
                {
                    CATCH_REQUIRE_THROWS_AS([&](){
                                zipios::BackBuffer bb(is, zipios::VirtualSeeker(), i);
                            }(), zipios::InvalidException);
                }
            }

            CATCH_THEN("we close the file and we get an exception")
            {
                is.close();
                // first time the seek fails
                CATCH_REQUIRE_THROWS_AS([&](){
                                zipios::BackBuffer bb(is);
                            }(), zipios::IOException);
                // second time the file is marked as invalid
                CATCH_REQUIRE_THROWS_AS([&](){
                                zipios::BackBuffer bb(is);
                            }(), zipios::InvalidException);
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
