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
 * Zipios unit tests for various stream and buffer classes.
 */

#include "tests.hpp"

#include "zipios/zipfile.hpp"
#include "zipios/zipiosexceptions.hpp"

#include "src/filterinputstreambuf.hpp"
#include "src/filteroutputstreambuf.hpp"

#include <fstream>

#include <unistd.h>
#include <string.h>





TEST_CASE("An input filter", "[Buffer]")
{
    SECTION("Valid input stream buffer")
    {
        zipios_test::auto_unlink_t auto_unlink("input.buf");
        {
            std::ofstream out("input.buf", std::ios::out | std::ios::binary);
            out << "Test file" << std::endl;
        }

        std::unique_ptr<std::ifstream> in_ptr(new std::ifstream("input.buf", std::ios::in | std::ios::binary));
        std::unique_ptr<zipios::FilterInputStreambuf> buf_ptr(new zipios::FilterInputStreambuf(in_ptr->rdbuf()));
    }

    SECTION("Invalid input stream buffer")
    {
        REQUIRE_THROWS_AS(new zipios::FilterInputStreambuf(nullptr), zipios::InvalidStateException);
    }
}


TEST_CASE("An output filter", "[Buffer]")
{
    SECTION("Valid output stream buffer")
    {
        zipios_test::auto_unlink_t auto_unlink("output.buf");

        std::ofstream out("output.buf", std::ios::out | std::ios::binary);
        std::unique_ptr<zipios::FilterOutputStreambuf> buf_ptr(new zipios::FilterOutputStreambuf(out.rdbuf()));
    }

    SECTION("Invalid output stream buffer")
    {
        REQUIRE_THROWS_AS(new zipios::FilterOutputStreambuf(nullptr), zipios::InvalidStateException);
    }
}





// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
