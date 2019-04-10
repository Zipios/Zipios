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
 * Zipios unit tests used to verify the various funtions defined in
 * zipios_common.hpp.
 */

#include "tests.hpp"

#include "src/zipios_common.hpp"
#include "zipios/zipiosexceptions.hpp"

#include <fstream>

#include <unistd.h>


SCENARIO("Vector append", "[zipios_common]")
{
    GIVEN("an empty vector")
    {
        std::vector<std::string> es;

        WHEN("appending another empty vector")
        {
            std::vector<std::string> os;

            es += os;

            THEN("the result is still an emtpy vector")
            {
                REQUIRE(es.empty());
            }
        }

        WHEN("appending a non-empty vector")
        {
            std::vector<std::string> os{ "a", "b", "c" };

            es += os;

            THEN("the result is like that non-empty vector")
            {
                REQUIRE(es.size() == 3);

                REQUIRE(es[0] == "a");
                REQUIRE(es[1] == "b");
                REQUIRE(es[2] == "c");
            }
        }
    }

    GIVEN("a non-empty vector")
    {
        std::vector<std::string> es{ "x", "y", "z" };

        WHEN("appending an empty vector")
        {
            std::vector<std::string> os;

            es += os;

            THEN("the result is still the 3 element vector")
            {
                REQUIRE(es.size() == 3);

                REQUIRE(es[0] == "x");
                REQUIRE(es[1] == "y");
                REQUIRE(es[2] == "z");
            }
        }

        WHEN("appending a non-empty vector")
        {
            std::vector<std::string> os{ "a", "b", "c" };

            es += os;

            THEN("the result is the original vector with the other vector appended")
            {
                REQUIRE(es.size() == 6);

                REQUIRE(es[0] == "x");
                REQUIRE(es[1] == "y");
                REQUIRE(es[2] == "z");
                REQUIRE(es[3] == "a");
                REQUIRE(es[4] == "b");
                REQUIRE(es[5] == "c");
            }
        }
    }
}


TEST_CASE("Verify the g_separator", "[zipios_common]")
{
    // Not too sure why we have that as a variable since it is always
    // a slash (/) and never a backslash (\) but it is there...
    REQUIRE(zipios::g_separator == '/');
}


SCENARIO("Read from file", "[zipios_common] [io]")
{
    GIVEN("a simple file")
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

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            THEN("we get exactly the value we expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
            }
        }

        WHEN("reading one 32 bit between two 16 bit values")
        {
            uint32_t b;
            uint16_t a, c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            THEN("the result is exactly as expected")
            {
                REQUIRE(a == 0x0100);
                REQUIRE(b == 0x05040302);
                REQUIRE(c == 0x0706);
            }
        }

        WHEN("reading one 16 bit between two 32 bit values")
        {
            uint32_t a, c;
            uint16_t b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x0504);
                REQUIRE(c == 0x09080706);
            }
        }

        WHEN("reading three 16 bit values")
        {
            uint16_t a, b, c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x0100);
                REQUIRE(b == 0x0302);
                REQUIRE(c == 0x0504);
            }
        }

        WHEN("reading one 32 bit, one 8 bit then one 16 bit value")
        {
            uint32_t a;
            uint8_t  b;
            uint16_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x04);
                REQUIRE(c == 0x0605);
            }
        }

        WHEN("reading one 32 bit, one 8 bit then one buffer value")
        {
            uint32_t a;
            uint8_t  b;
            zipios::buffer_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c, 8);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x04);
                REQUIRE(c.size() == 8);
                REQUIRE(c[0] == 0x05);
                REQUIRE(c[1] == 0x06);
                REQUIRE(c[2] == 0x07);
                REQUIRE(c[3] == 0x08);
                REQUIRE(c[4] == 0x09);
                REQUIRE(c[5] == 0x0A);
                REQUIRE(c[6] == 0x0B);
                REQUIRE(c[7] == 0x0C);
            }
        }

        WHEN("reading one 32 bit, one string and then one 8 bit value")
        {
            uint32_t a;
            std::string b;
            uint8_t c;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b, 8);
            zipios::zipRead(is, c);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b.size() == 8);
                REQUIRE(b[0] == 0x04);
                REQUIRE(b[1] == 0x05);
                REQUIRE(b[2] == 0x06);
                REQUIRE(b[3] == 0x07);
                REQUIRE(b[4] == 0x08);
                REQUIRE(b[5] == 0x09);
                REQUIRE(b[6] == 0x0A);
                REQUIRE(b[7] == 0x0B);
                REQUIRE(c == 0x0C);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            THEN("another 8 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint8_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            THEN("another 16 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint16_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);
            zipios::zipRead(is, c);
            zipios::zipRead(is, d);

            THEN("another 32 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint32_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, e), zipios::IOException);
            }
        }

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            THEN("then a string that's too long")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                std::string e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, e, 12), zipios::IOException);
            }
        }

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            zipios::zipRead(is, a);
            zipios::zipRead(is, b);

            THEN("then a buffer that's too long")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                zipios::buffer_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, e, 12), zipios::IOException);
            }
        }

        unlink("io.bin");
    }
}


SCENARIO("Read from buffer", "[zipios_common] [io]")
{
    GIVEN("a simple buffer")
    {
        zipios::buffer_t is;
        for(int i(0); i < 16; ++i)
        {
            is.push_back(static_cast<char>(i));
        }

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);

            THEN("we get exactly the value we expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
            }
        }

        WHEN("reading one 32 bit between two 16 bit values")
        {
            uint32_t b;
            uint16_t a, c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 2);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 6);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 8);

            THEN("the result is exactly as expected")
            {
                REQUIRE(a == 0x0100);
                REQUIRE(b == 0x05040302);
                REQUIRE(c == 0x0706);
            }
        }

        WHEN("reading one 16 bit between two 32 bit values")
        {
            uint32_t a, c;
            uint16_t b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 6);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 10);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x0504);
                REQUIRE(c == 0x09080706);
            }
        }

        WHEN("reading three 16 bit values")
        {
            uint16_t a, b, c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 2);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 6);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x0100);
                REQUIRE(b == 0x0302);
                REQUIRE(c == 0x0504);
            }
        }

        WHEN("reading one 32 bit, one 8 bit then one 16 bit value")
        {
            uint32_t a;
            uint8_t  b;
            uint16_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 5);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 7);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x04);
                REQUIRE(c == 0x0605);
            }
        }

        WHEN("reading one 32 bit, one 8 bit then one buffer value")
        {
            uint32_t a;
            uint8_t  b;
            zipios::buffer_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 5);
            zipios::zipRead(is, pos, c, 8);
            REQUIRE(pos == 13);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x04);
                REQUIRE(c.size() == 8);
                REQUIRE(c[0] == 0x05);
                REQUIRE(c[1] == 0x06);
                REQUIRE(c[2] == 0x07);
                REQUIRE(c[3] == 0x08);
                REQUIRE(c[4] == 0x09);
                REQUIRE(c[5] == 0x0A);
                REQUIRE(c[6] == 0x0B);
                REQUIRE(c[7] == 0x0C);
            }
        }

        WHEN("reading one 32 bit, one string and then one 8 bit value")
        {
            uint32_t a;
            std::string b;
            uint8_t c;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b, 8);
            REQUIRE(pos == 12);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 13);

            THEN("the result is as expected")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b.size() == 8);
                REQUIRE(b[0] == 0x04);
                REQUIRE(b[1] == 0x05);
                REQUIRE(b[2] == 0x06);
                REQUIRE(b[3] == 0x07);
                REQUIRE(b[4] == 0x08);
                REQUIRE(b[5] == 0x09);
                REQUIRE(b[6] == 0x0A);
                REQUIRE(b[7] == 0x0B);
                REQUIRE(c == 0x0C);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            REQUIRE(pos == 16);

            THEN("another 8 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint8_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            REQUIRE(pos == 16);

            THEN("another 16 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint16_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException);
            }
        }

        WHEN("reading four 32 bit values")
        {
            uint32_t a, b, c, d;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);
            zipios::zipRead(is, pos, c);
            REQUIRE(pos == 12);
            zipios::zipRead(is, pos, d);
            REQUIRE(pos == 16);

            THEN("another 32 bit value")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);
                REQUIRE(c == 0x0B0A0908);
                REQUIRE(d == 0x0F0E0D0C);

                uint32_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e), zipios::IOException);
            }
        }

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);

            THEN("then a string that's too long")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                std::string e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e, 12), zipios::IOException);
            }
        }

        WHEN("reading two 32 bit values")
        {
            uint32_t a, b;
            size_t pos(0);
            zipios::zipRead(is, pos, a);
            REQUIRE(pos == 4);
            zipios::zipRead(is, pos, b);
            REQUIRE(pos == 8);

            THEN("then a buffer that's too long")
            {
                REQUIRE(a == 0x03020100);
                REQUIRE(b == 0x07060504);

                // we have 8 bytes left, trying to read 12 fails
                zipios::buffer_t e;
                REQUIRE_THROWS_AS(zipios::zipRead(is, pos, e, 12), zipios::IOException);
            }
        }
    }
}


SCENARIO("Write to file", "[zipios_common] [io]")
{
    GIVEN("create an empty file")
    {

        WHEN("writing two 32 bit values")
        {
            uint32_t a(0x03020100), b(0x07060504);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
            }

            THEN("we get exactly the value we expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 8);
                is.seekg(0, std::ios::beg);

                char buf[8];
                is.read(buf, 8);

                REQUIRE(buf[0] == 0x00);
                REQUIRE(buf[1] == 0x01);
                REQUIRE(buf[2] == 0x02);
                REQUIRE(buf[3] == 0x03);
                REQUIRE(buf[4] == 0x04);
                REQUIRE(buf[5] == 0x05);
                REQUIRE(buf[6] == 0x06);
                REQUIRE(buf[7] == 0x07);
            }
        }

        WHEN("writing one 32 bit between two 16 bit values")
        {
            uint32_t b(0x55112288);
            uint16_t a(0x3344), c(0x6677);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            THEN("the result is exactly as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 8);
                is.seekg(0, std::ios::beg);

                char buf[8];
                is.read(buf, 8);

                REQUIRE(buf[0] == 0x44);
                REQUIRE(buf[1] == 0x33);
                REQUIRE(buf[2] == static_cast<char>(0x88));
                REQUIRE(buf[3] == 0x22);
                REQUIRE(buf[4] == 0x11);
                REQUIRE(buf[5] == 0x55);
                REQUIRE(buf[6] == 0x77);
                REQUIRE(buf[7] == 0x66);
            }
        }

        WHEN("writing one 16 bit between two 32 bit values")
        {
            uint32_t a(0x01050803), c(0x10508030);
            uint16_t b(0xFF00);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 10);
                is.seekg(0, std::ios::beg);

                char buf[10];
                is.read(buf, 10);

                REQUIRE(buf[0] == 0x03);
                REQUIRE(buf[1] == 0x08);
                REQUIRE(buf[2] == 0x05);
                REQUIRE(buf[3] == 0x01);
                REQUIRE(buf[4] == 0x00);
                REQUIRE(buf[5] == static_cast<char>(0xFF));
                REQUIRE(buf[6] == 0x30);
                REQUIRE(buf[7] == static_cast<char>(0x80));
                REQUIRE(buf[8] == 0x50);
                REQUIRE(buf[9] == 0x10);
            }
        }

        WHEN("writing three 16 bit values")
        {
            uint16_t a(0xEECC), b(0xAADD), c(0xFFBB);
            {
                std::ofstream os("io.bin", std::ios::out | std::ios::binary);
                zipios::zipWrite(os, a);
                zipios::zipWrite(os, b);
                zipios::zipWrite(os, c);
            }

            THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 6);
                is.seekg(0, std::ios::beg);

                char buf[6];
                is.read(buf, 6);

                REQUIRE(buf[0] == static_cast<char>(0xCC));
                REQUIRE(buf[1] == static_cast<char>(0xEE));
                REQUIRE(buf[2] == static_cast<char>(0xDD));
                REQUIRE(buf[3] == static_cast<char>(0xAA));
                REQUIRE(buf[4] == static_cast<char>(0xBB));
                REQUIRE(buf[5] == static_cast<char>(0xFF));
            }
        }

        WHEN("writing one 32 bit, one 8 bit then one 16 bit value")
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

            THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 7);
                is.seekg(0, std::ios::beg);

                char buf[7];
                is.read(buf, 7);

                REQUIRE(buf[0] == 0x44);
                REQUIRE(buf[1] == 0x33);
                REQUIRE(buf[2] == 0x22);
                REQUIRE(buf[3] == 0x11);
                REQUIRE(buf[4] == static_cast<char>(0xAA));
                REQUIRE(buf[5] == static_cast<char>(0x88));
                REQUIRE(buf[6] == static_cast<char>(0x99));
            }
        }

        WHEN("writing one 32 bit, one 8 bit then one buffer value")
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

            THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 13);
                is.seekg(0, std::ios::beg);

                char buf[13];
                is.read(buf, 13);

                REQUIRE(buf[ 0] == 0x04);
                REQUIRE(buf[ 1] == 0x03);
                REQUIRE(buf[ 2] == 0x02);
                REQUIRE(buf[ 3] == 0x01);
                REQUIRE(buf[ 4] == static_cast<char>(0xFF));
                REQUIRE(buf[ 5] == static_cast<char>(0xA0));
                REQUIRE(buf[ 6] == static_cast<char>(0xA1));
                REQUIRE(buf[ 7] == static_cast<char>(0xA2));
                REQUIRE(buf[ 8] == static_cast<char>(0xA3));
                REQUIRE(buf[ 9] == static_cast<char>(0xA4));
                REQUIRE(buf[10] == static_cast<char>(0xA5));
                REQUIRE(buf[11] == static_cast<char>(0xA6));
                REQUIRE(buf[12] == static_cast<char>(0xA7));
            }
        }

        WHEN("writing one 32 bit, one string and then one 8 bit value")
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

            THEN("the result is as expected")
            {
                std::ifstream is("io.bin", std::ios::in | std::ios::binary);
                is.seekg(0, std::ios::end);
                REQUIRE(is.tellg() == 9);
                is.seekg(0, std::ios::beg);

                char buf[9];
                is.read(buf, 9);

                REQUIRE(buf[0] == 0x09);
                REQUIRE(buf[1] == 0x07);
                REQUIRE(buf[2] == 0x05);
                REQUIRE(buf[3] == 0x03);
                REQUIRE(buf[4] == 'T');
                REQUIRE(buf[5] == 'E');
                REQUIRE(buf[6] == 'S');
                REQUIRE(buf[7] == 'T');
                REQUIRE(buf[8] == 0x01);
            }
        }

        WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            THEN("writing a 8 bit value fails")
            {
                uint8_t c(0xFF);
                REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException);
            }
        }

        WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            THEN("writing a 16 bit value fails")
            {
                uint16_t c(0xFFEE);
                REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException);
            }
        }

        WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            THEN("writing a 32 bit value fails")
            {
                uint32_t c(0xFFEEDDCC);
                REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException);
            }
        }

        WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            THEN("writing a string fails")
            {
                std::string c("TEST");
                REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException);
            }
        }

        WHEN("writing some data and mark the output as invalid")
        {
            uint32_t a(0x03050709);
            std::string b("TEST");
            std::ofstream os("io.bin", std::ios::out | std::ios::binary);
            zipios::zipWrite(os, a);
            zipios::zipWrite(os, b);
            os.setstate(std::ios::failbit);

            THEN("writing a buffer fails")
            {
                zipios::buffer_t c;
                c.push_back('F');
                c.push_back('A');
                c.push_back('I');
                c.push_back('L');
                REQUIRE_THROWS_AS(zipios::zipWrite(os, c), zipios::IOException);
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
