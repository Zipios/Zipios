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
 * Zipios unit tests used to verify the various functions defined in
 * zipios_common.hpp.
 */

#include "catch_main.hpp"

#include <src/zipios_common.hpp>
#include <zipios/zipiosexceptions.hpp>

#include <fstream>

#include <unistd.h>


CATCH_SCENARIO("Vector append", "[zipios_common]")
{
    CATCH_GIVEN("an empty vector")
    {
        std::vector<std::string> es;

        CATCH_WHEN("appending another empty vector")
        {
            std::vector<std::string> os;

            es += os;

            CATCH_THEN("the result is still an empty vector")
            {
                CATCH_REQUIRE(es.empty());
            }
        }

        CATCH_WHEN("appending a non-empty vector")
        {
            std::vector<std::string> os{ "a", "b", "c" };

            es += os;

            CATCH_THEN("the result is like that non-empty vector")
            {
                CATCH_REQUIRE(es.size() == 3);

                CATCH_REQUIRE(es[0] == "a");
                CATCH_REQUIRE(es[1] == "b");
                CATCH_REQUIRE(es[2] == "c");
            }
        }
    }

    CATCH_GIVEN("a non-empty vector")
    {
        std::vector<std::string> es{ "x", "y", "z" };

        CATCH_WHEN("appending an empty vector")
        {
            std::vector<std::string> os;

            es += os;

            CATCH_THEN("the result is still the 3 element vector")
            {
                CATCH_REQUIRE(es.size() == 3);

                CATCH_REQUIRE(es[0] == "x");
                CATCH_REQUIRE(es[1] == "y");
                CATCH_REQUIRE(es[2] == "z");
            }
        }

        CATCH_WHEN("appending a non-empty vector")
        {
            std::vector<std::string> os{ "a", "b", "c" };

            es += os;

            CATCH_THEN("the result is the original vector with the other vector appended")
            {
                CATCH_REQUIRE(es.size() == 6);

                CATCH_REQUIRE(es[0] == "x");
                CATCH_REQUIRE(es[1] == "y");
                CATCH_REQUIRE(es[2] == "z");
                CATCH_REQUIRE(es[3] == "a");
                CATCH_REQUIRE(es[4] == "b");
                CATCH_REQUIRE(es[5] == "c");
            }
        }
    }
}


CATCH_TEST_CASE("Verify the g_separator", "[zipios_common]")
{
    // Not too sure why we have that as a variable since it is always
    // a slash (/) and never a backslash (\) but it is there...
    CATCH_REQUIRE(zipios::g_separator == '/');
}


CATCH_SCENARIO("Read from file", "[zipios_common] [io]")
{
    CATCH_GIVEN("a simple file")
    {
        // create a file
        {
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            for(int i(0); i < 16; ++i)
            {
                os << static_cast<char>(i);
            }
        }

        // now open it for reading
        std::ifstream is("io.bin", std::ios::in | std::ios::binary);

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            CATCH_THEN("we get exactly the value we expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
            }
        }

        CATCH_WHEN("reading one 32 bit between two 16 bit values")
        {
            uint32_t b;
            uint16_t a, c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            CATCH_THEN("the result is exactly as expected")
            {
                CATCH_REQUIRE(a == 0x0100);
                CATCH_REQUIRE(b == 0x05040302);
                CATCH_REQUIRE(c == 0x0706);
            }
        }

        CATCH_WHEN("reading one 16 bit between two 32 bit values")
        {
            uint32_t a, c;
            uint16_t b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x0504);
                CATCH_REQUIRE(c == 0x09080706);
            }
        }

        CATCH_WHEN("reading three 16 bit values")
        {
            uint16_t a, b, c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x0100);
                CATCH_REQUIRE(b == 0x0302);
                CATCH_REQUIRE(c == 0x0504);
            }
        }

        CATCH_WHEN("reading one 32 bit, one 8 bit then one 16 bit value")
        {
            uint32_t a;
            uint8_t  b;
            uint16_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x04);
                CATCH_REQUIRE(c == 0x0605);
            }
        }

        CATCH_WHEN("reading one 32 bit, one 8 bit then one buffer value")
        {
            uint32_t a;
            uint8_t  b;
            zipios::buffer_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c, 8);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x04);
                CATCH_REQUIRE(c.size() == 8);
                CATCH_REQUIRE(c[0] == 0x05);
                CATCH_REQUIRE(c[1] == 0x06);
                CATCH_REQUIRE(c[2] == 0x07);
                CATCH_REQUIRE(c[3] == 0x08);
                CATCH_REQUIRE(c[4] == 0x09);
                CATCH_REQUIRE(c[5] == 0x0A);
                CATCH_REQUIRE(c[6] == 0x0B);
                CATCH_REQUIRE(c[7] == 0x0C);
            }
        }

        CATCH_WHEN("reading one 32 bit, one string and then one 8 bit value")
        {
            uint32_t a;
            std::string b;
            uint8_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b, 8);
            zipios::zipRead(is, c);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b.size() == 8);
                CATCH_REQUIRE(b[0] == 0x04);
                CATCH_REQUIRE(b[1] == 0x05);
                CATCH_REQUIRE(b[2] == 0x06);
                CATCH_REQUIRE(b[3] == 0x07);
                CATCH_REQUIRE(b[4] == 0x08);
                CATCH_REQUIRE(b[5] == 0x09);
                CATCH_REQUIRE(b[6] == 0x0A);
                CATCH_REQUIRE(b[7] == 0x0B);
                CATCH_REQUIRE(c == 0x0C);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            CATCH_THEN("another 8 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint8_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            CATCH_THEN("another 16 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint16_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            CATCH_THEN("another 32 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint32_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            CATCH_THEN("then a string that's too long")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                std::string e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, e, 12), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            CATCH_THEN("then a buffer that's too long")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                zipios::buffer_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, e, 12), zipios::IOException &);
            }
        }

        unlink("io.bin");
    }
}


CATCH_SCENARIO("Read from buffer", "[zipios_common] [io]")
{
    CATCH_GIVEN("a simple buffer")
    {
        zipios::buffer_t is;
        for(int i(0); i < 16; ++i)
        {
            is.push_back(static_cast<char>(i));
        }

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);

            CATCH_THEN("we get exactly the value we expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
            }
        }

        CATCH_WHEN("reading one 32 bit between two 16 bit values")
        {
            uint32_t b;
            uint16_t a, c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 2);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 6);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 8);

            CATCH_THEN("the result is exactly as expected")
            {
                CATCH_REQUIRE(a == 0x0100);
                CATCH_REQUIRE(b == 0x05040302);
                CATCH_REQUIRE(c == 0x0706);
            }
        }

        CATCH_WHEN("reading one 16 bit between two 32 bit values")
        {
            uint32_t a, c;
            uint16_t b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 6);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 10);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x0504);
                CATCH_REQUIRE(c == 0x09080706);
            }
        }

        CATCH_WHEN("reading three 16 bit values")
        {
            uint16_t a, b, c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 2);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 6);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x0100);
                CATCH_REQUIRE(b == 0x0302);
                CATCH_REQUIRE(c == 0x0504);
            }
        }

        CATCH_WHEN("reading one 32 bit, one 8 bit then one 16 bit value")
        {
            uint32_t a;
            uint8_t  b;
            uint16_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 5);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 7);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x04);
                CATCH_REQUIRE(c == 0x0605);
            }
        }

        CATCH_WHEN("reading one 32 bit, one 8 bit then one buffer value")
        {
            uint32_t a;
            uint8_t  b;
            zipios::buffer_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 5);
            zipios::zipRead(is, pos, c, 8);
            CATCH_REQUIRE(pos == 13);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x04);
                CATCH_REQUIRE(c.size() == 8);
                CATCH_REQUIRE(c[0] == 0x05);
                CATCH_REQUIRE(c[1] == 0x06);
                CATCH_REQUIRE(c[2] == 0x07);
                CATCH_REQUIRE(c[3] == 0x08);
                CATCH_REQUIRE(c[4] == 0x09);
                CATCH_REQUIRE(c[5] == 0x0A);
                CATCH_REQUIRE(c[6] == 0x0B);
                CATCH_REQUIRE(c[7] == 0x0C);
            }
        }

        CATCH_WHEN("reading one 32 bit, one string and then one 8 bit value")
        {
            uint32_t a;
            std::string b;
            uint8_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b, 8);
            CATCH_REQUIRE(pos == 12);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 13);

            CATCH_THEN("the result is as expected")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b.size() == 8);
                CATCH_REQUIRE(b[0] == 0x04);
                CATCH_REQUIRE(b[1] == 0x05);
                CATCH_REQUIRE(b[2] == 0x06);
                CATCH_REQUIRE(b[3] == 0x07);
                CATCH_REQUIRE(b[4] == 0x08);
                CATCH_REQUIRE(b[5] == 0x09);
                CATCH_REQUIRE(b[6] == 0x0A);
                CATCH_REQUIRE(b[7] == 0x0B);
                CATCH_REQUIRE(c == 0x0C);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            CATCH_REQUIRE(pos == 16);

            CATCH_THEN("another 8 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint8_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            CATCH_REQUIRE(pos == 16);

            CATCH_THEN("another 16 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint16_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            CATCH_REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            CATCH_REQUIRE(pos == 16);

            CATCH_THEN("another 32 bit value")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);
                CATCH_REQUIRE(c == 0x0B0A0908);
                CATCH_REQUIRE(d == 0x0F0E0D0C);

                uint32_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);

            CATCH_THEN("then a string that's too long")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                std::string e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e, 12), zipios::IOException &);
            }
        }

        CATCH_WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            CATCH_REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            CATCH_REQUIRE(pos == 8);

            CATCH_THEN("then a buffer that's too long")
            {
                CATCH_REQUIRE(a == 0x03020100);
                CATCH_REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                zipios::buffer_t e;
                CATCH_REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e, 12), zipios::IOException &);
            }
        }
    }
}


CATCH_SCENARIO("Write to file", "[zipios_common] [io]")
{
    CATCH_GIVEN("create an empty file")
    {

        CATCH_WHEN("writing two 32 bit values")
        {
            uint32_t a(0x03020100), b(0x07060504);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
            }

            CATCH_THEN("we get exactly the value we expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 8);
                is.seekg(0, std::ios::beg);

                char buf[8];
                is.read(buf, 8);

                CATCH_REQUIRE(buf[0] == 0x00);
                CATCH_REQUIRE(buf[1] == 0x01);
                CATCH_REQUIRE(buf[2] == 0x02);
                CATCH_REQUIRE(buf[3] == 0x03);
                CATCH_REQUIRE(buf[4] == 0x04);
                CATCH_REQUIRE(buf[5] == 0x05);
                CATCH_REQUIRE(buf[6] == 0x06);
                CATCH_REQUIRE(buf[7] == 0x07);
            }
        }

        CATCH_WHEN("writing one 32 bit between two 16 bit values")
        {
            uint32_t b(0x55112288);
            uint16_t a(0x3344), c(0x6677);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is exactly as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 8);
                is.seekg(0, std::ios::beg);

                char buf[8];
                is.read(buf, 8);

                CATCH_REQUIRE(buf[0] == 0x44);
                CATCH_REQUIRE(buf[1] == 0x33);
                CATCH_REQUIRE(buf[2] == static_cast<char>(0x88));
                CATCH_REQUIRE(buf[3] == 0x22);
                CATCH_REQUIRE(buf[4] == 0x11);
                CATCH_REQUIRE(buf[5] == 0x55);
                CATCH_REQUIRE(buf[6] == 0x77);
                CATCH_REQUIRE(buf[7] == 0x66);
            }
        }

        CATCH_WHEN("writing one 16 bit between two 32 bit values")
        {
            uint32_t a(0x01050803), c(0x10508030);
            uint16_t b(0xFF00);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 10);
                is.seekg(0, std::ios::beg);

                char buf[10];
                is.read(buf, 10);

                CATCH_REQUIRE(buf[0] == 0x03);
                CATCH_REQUIRE(buf[1] == 0x08);
                CATCH_REQUIRE(buf[2] == 0x05);
                CATCH_REQUIRE(buf[3] == 0x01);
                CATCH_REQUIRE(buf[4] == 0x00);
                CATCH_REQUIRE(buf[5] == static_cast<char>(0xFF));
                CATCH_REQUIRE(buf[6] == 0x30);
                CATCH_REQUIRE(buf[7] == static_cast<char>(0x80));
                CATCH_REQUIRE(buf[8] == 0x50);
                CATCH_REQUIRE(buf[9] == 0x10);
            }
        }

        CATCH_WHEN("writing three 16 bit values")
        {
            uint16_t a(0xEECC), b(0xAADD), c(0xFFBB);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 6);
                is.seekg(0, std::ios::beg);

                char buf[6];
                is.read(buf, 6);

                CATCH_REQUIRE(buf[0] == static_cast<char>(0xCC));
                CATCH_REQUIRE(buf[1] == static_cast<char>(0xEE));
                CATCH_REQUIRE(buf[2] == static_cast<char>(0xDD));
                CATCH_REQUIRE(buf[3] == static_cast<char>(0xAA));
                CATCH_REQUIRE(buf[4] == static_cast<char>(0xBB));
                CATCH_REQUIRE(buf[5] == static_cast<char>(0xFF));
            }
        }

        CATCH_WHEN("writing one 32 bit, one 8 bit then one 16 bit value")
        {
            uint32_t a(0x11223344);
            uint8_t  b(0xAA);
            uint16_t c(0x9988);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 7);
                is.seekg(0, std::ios::beg);

                char buf[7];
                is.read(buf, 7);

                CATCH_REQUIRE(buf[0] == 0x44);
                CATCH_REQUIRE(buf[1] == 0x33);
                CATCH_REQUIRE(buf[2] == 0x22);
                CATCH_REQUIRE(buf[3] == 0x11);
                CATCH_REQUIRE(buf[4] == static_cast<char>(0xAA));
                CATCH_REQUIRE(buf[5] == static_cast<char>(0x88));
                CATCH_REQUIRE(buf[6] == static_cast<char>(0x99));
            }
        }

        CATCH_WHEN("writing one 32 bit, one 8 bit then one buffer value")
        {
            uint32_t a(0x01020304);
            uint8_t  b(0xFF);
            zipios::buffer_t c;
            c.push_back(0xA0);
            c.push_back(0xA1);
            c.push_back(0xA2);
            c.push_back(0xA3);
            c.push_back(0xA4);
            c.push_back(0xA5);
            c.push_back(0xA6);
            c.push_back(0xA7);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 13);
                is.seekg(0, std::ios::beg);

                char buf[13];
                is.read(buf, 13);

                CATCH_REQUIRE(buf[ 0] == 0x04);
                CATCH_REQUIRE(buf[ 1] == 0x03);
                CATCH_REQUIRE(buf[ 2] == 0x02);
                CATCH_REQUIRE(buf[ 3] == 0x01);
                CATCH_REQUIRE(buf[ 4] == static_cast<char>(0xFF));
                CATCH_REQUIRE(buf[ 5] == static_cast<char>(0xA0));
                CATCH_REQUIRE(buf[ 6] == static_cast<char>(0xA1));
                CATCH_REQUIRE(buf[ 7] == static_cast<char>(0xA2));
                CATCH_REQUIRE(buf[ 8] == static_cast<char>(0xA3));
                CATCH_REQUIRE(buf[ 9] == static_cast<char>(0xA4));
                CATCH_REQUIRE(buf[10] == static_cast<char>(0xA5));
                CATCH_REQUIRE(buf[11] == static_cast<char>(0xA6));
                CATCH_REQUIRE(buf[12] == static_cast<char>(0xA7));
            }
        }

        CATCH_WHEN("writing one 32 bit, one string and then one 8 bit value")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            uint8_t c(0x01);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            CATCH_THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                CATCH_REQUIRE(is.tellg() == 9);
                is.seekg(0, std::ios::beg);

                char buf[9];
                is.read(buf, 9);

                CATCH_REQUIRE(buf[0] == 0x09);
                CATCH_REQUIRE(buf[1] == 0x07);
                CATCH_REQUIRE(buf[2] == 0x05);
                CATCH_REQUIRE(buf[3] == 0x03);
                CATCH_REQUIRE(buf[4] == 'T');
                CATCH_REQUIRE(buf[5] == 'E');
                CATCH_REQUIRE(buf[6] == 'S');
                CATCH_REQUIRE(buf[7] == 'T');
                CATCH_REQUIRE(buf[8] == 0x01);
            }
        }

        CATCH_WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            CATCH_THEN("writing a 8 bit value fails")
            {
                uint8_t c(0xFF);
                CATCH_REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException &);
            }
        }

        CATCH_WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            CATCH_THEN("writing a 16 bit value fails")
            {
                uint16_t c(0xFFEE);
                CATCH_REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException &);
            }
        }

        CATCH_WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            CATCH_THEN("writing a 32 bit value fails")
            {
                uint32_t c(0xFFEEDDCC);
                CATCH_REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException  &);
            }
        }

        CATCH_WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            CATCH_THEN("writing a string fails")
            {
                std::string c("TEST");
                CATCH_REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException &);
            }
        }

        CATCH_WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            CATCH_THEN("writing a buffer fails")
            {
                zipios::buffer_t c;
                c.push_back('F');
                c.push_back('A');
                c.push_back('I');
                c.push_back('L');
                CATCH_REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException &);
            }
        }

        unlink("io.bin");
    }
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
