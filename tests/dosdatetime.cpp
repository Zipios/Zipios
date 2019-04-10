/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2019  Made to Order Software Corporation

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
 * Zipios unit tests verify the dosdatetime.cpp/hpp implementation.
 *
 * Keep in mind that the date is saved as a local time date in a zip file.
 * It's crap. But that comes from the old days of DOS which chose that format
 * at the time. Sharing between people in the whole world was not viewed in
 * a way similar to today's world...
 *
 * \warning
 * This test will fail if run around a standard time to a saving time
 * or a saving time to a standard time changing period (i.e. during the
 * 1h or so when time changes in spring and autumn.)
 */

#include "tests.hpp"

#include "zipios/dosdatetime.hpp"
#include "zipios/zipiosexceptions.hpp"

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <iostream>




// min = Jan  1, 1980 at 00:00:00
std::time_t g_minimum_unix = -1;

// max = Dec 31, 2107 at 23:59:59
std::time_t g_maximum_unix = -1;


void init_min_max()
{
    if(g_minimum_unix == -1)
    {
        struct tm t;

        memset(&t, 0, sizeof(t));
        t.tm_sec = 0;
        t.tm_min = 0;
        t.tm_hour = 0;
        t.tm_mday = 1;
        t.tm_mon = 0;
        t.tm_year = 1980 - 1900;
        t.tm_isdst = -1;
        g_minimum_unix = mktime(&t);

        struct tm c;
        localtime_r(&g_minimum_unix, &c);
    }

    if(g_maximum_unix == -1)
    {
        struct tm t;

        memset(&t, 0, sizeof(t));
        t.tm_sec = 58;
        t.tm_min = 59;
        t.tm_hour = 23;
        t.tm_mday = 31;
        t.tm_mon = 11;
        t.tm_year = 2107 - 1900;
        t.tm_isdst = -1;
        g_maximum_unix = mktime(&t);
    }
}



TEST_CASE("dosdatetime_minmax", "[dosdatetime]")
{
    SECTION("DOS time minimum")
    {
        zipios::DOSDateTime t;
        t.setYear(1980);
        t.setMonth(1);
        t.setMDay(1);
        t.setHour(0);
        t.setMinute(0);
        t.setSecond(0);
        REQUIRE(zipios::DOSDateTime::g_min_dosdatetime == t.getDOSDateTime());
    }

    SECTION("DOS time maximum")
    {
        zipios::DOSDateTime t;
        t.setYear(2107);
        t.setMonth(12);
        t.setMDay(31);
        t.setHour(23);
        t.setMinute(59);
        t.setSecond(59);
        REQUIRE(zipios::DOSDateTime::g_max_dosdatetime == t.getDOSDateTime());
    }
}


#if 0
// this test is too long, which is why it is commented out by default
// still, it is great if you want to verify that all possible DOS Time & Date
//
TEST_CASE("valid_dosdatetime", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    SECTION("all valid DOS Date Time values")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_minimum_unix); t <= g_maximum_unix; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            REQUIRE(u == et);
        }
    }
}
#endif


TEST_CASE("small_dosdatetime", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    SECTION("just under the minimum")
    {
        // make sure the minimum limit is checked properly
        //
        for(std::time_t t(g_minimum_unix - 20); t < 0; ++t)
        {
            zipios::DOSDateTime td;
            REQUIRE_FALSE(td.isValid());
            REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
            REQUIRE_FALSE(td.isValid());
        }
    }

    SECTION("just around minimum, but valid")
    {
        // the "g_minimum_unix - 1" case is peculiar, the Unix date is not
        // usable as is because it is odd and we're going to use the next
        // second which means we're getting `minimum_unix` as the date,
        // which is valid!
        //
        for(std::time_t t(g_minimum_unix); t <= g_minimum_unix + 20; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            REQUIRE(u == et);
        }
    }
}


TEST_CASE("large_dosdatetime", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    SECTION("just around maximum, but valid")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_maximum_unix - 20); t <= g_maximum_unix; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            REQUIRE(u == et);
        }
    }

    SECTION("just a bit too large")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_maximum_unix + 1); t <= g_maximum_unix + 20; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            REQUIRE(td.getDOSDateTime() == 0);
            REQUIRE_FALSE(td.isValid());
            REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
            REQUIRE_FALSE(td.isValid());
            REQUIRE(td.getDOSDateTime() == 0);
            REQUIRE(td.getUnixTimestamp() == 0);
        }
    }
}


TEST_CASE("random_dosdatetime", "[dosdatetime]")
{
    init_min_max();

    SECTION("more random tests")
    {
        std::time_t const max(sizeof(std::time_t) < sizeof(uint64_t)
                                ? 0x7FFFFFFF        // max. in 32 bits is less in Unix time_t than what the DOS Date Time supports
                                : 0x104000000LL);   // Dec 31, 2107 23:59:59 is 0x10391447F, so use something a little higher

        for(std::time_t t(0); t <= max; t += rand() & 0x7FFF)
        {
            if(t < g_minimum_unix
            || t > g_maximum_unix)
            {
                zipios::DOSDateTime td;
                REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
            }
            else
            {
                std::time_t et(t);
                if(t == g_maximum_unix)
                {
                    et += 1;
                }
                et &= ~1;

                zipios::DOSDateTime td;
                td.setUnixTimestamp(t);

                zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

                zipios::DOSDateTime tu;
                tu.setDOSDateTime(d);

                std::time_t const u(tu.getUnixTimestamp());
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
