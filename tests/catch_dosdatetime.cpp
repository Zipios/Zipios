/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (c) 2019-2022  Made to Order Software Corp.  All Rights Reserved

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

#include "catch_main.hpp"

#include <zipios/dosdatetime.hpp>
#include <zipios/zipiosexceptions.hpp>

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



CATCH_TEST_CASE("DOS Date & Time Min/Max", "[dosdatetime]")
{
    CATCH_START_SECTION("DOS time minimum")
    {
        zipios::DOSDateTime t;
        t.setYear(1980);
        t.setMonth(1);
        t.setMDay(1);
        t.setHour(0);
        t.setMinute(0);
        t.setSecond(0);
        CATCH_REQUIRE(zipios::DOSDateTime::g_min_dosdatetime == t.getDOSDateTime());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("DOS time maximum")
    {
        zipios::DOSDateTime t;
        t.setYear(2107);
        t.setMonth(12);
        t.setMDay(31);
        t.setHour(23);
        t.setMinute(59);
        t.setSecond(59);
        CATCH_REQUIRE(zipios::DOSDateTime::g_max_dosdatetime == t.getDOSDateTime());
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("Invalid DOS Date & Time", "[dosdatetime]")
{
    CATCH_START_SECTION("daysInMonth() when month is invalid")
    {
        // by default the date is 0 which means the month is 0 and
        // trying to call daysInMonth() fails with -1
        //
        zipios::DOSDateTime d;
        CATCH_REQUIRE(d.daysInMonth() == -1);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("get/set seconds")
    {
        for(int i = 0; i < 60; ++i)
        {
            zipios::DOSDateTime t;
            t.setSecond(i);
            CATCH_REQUIRE(t.getSecond() == (i & -2));
        }
        for(int i = -20; i < 0; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 60);
            t.setSecond(r);
            CATCH_REQUIRE(t.getSecond() == (r & -2));
            CATCH_REQUIRE_THROWS_AS(t.setSecond(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getSecond() == (r & -2));
        }
        for(int i = 60; i <= 80; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 60);
            t.setSecond(r);
            CATCH_REQUIRE(t.getSecond() == (r & -2));
            CATCH_REQUIRE_THROWS_AS(t.setSecond(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getSecond() == (r & -2));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("get/set minutes")
    {
        for(int i = 0; i < 60; ++i)
        {
            zipios::DOSDateTime t;
            t.setMinute(i);
            CATCH_REQUIRE(t.getMinute() == i);
        }
        for(int i = -20; i < 0; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 60);
            t.setMinute(r);
            CATCH_REQUIRE(t.getMinute() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMinute(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMinute() == r);
        }
        for(int i = 60; i <= 80; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 60);
            t.setMinute(r);
            CATCH_REQUIRE(t.getMinute() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMinute(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMinute() == r);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("get/set hours")
    {
        for(int i = 0; i < 24; ++i)
        {
            zipios::DOSDateTime t;
            t.setHour(i);
            CATCH_REQUIRE(t.getHour() == i);
        }
        for(int i = -20; i < 0; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 24);
            t.setHour(r);
            CATCH_REQUIRE(t.getHour() == r);
            CATCH_REQUIRE_THROWS_AS(t.setHour(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getHour() == r);
        }
        for(int i = 24; i <= 44; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 24);
            t.setHour(r);
            CATCH_REQUIRE(t.getHour() == r);
            CATCH_REQUIRE_THROWS_AS(t.setHour(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getHour() == r);
        }
    }
    CATCH_END_SECTION()

    // day is limited between 1 and 31 on a setMDay()
    // use the isValid() to know whether it is valid for the current month
    // and year
    //
    CATCH_START_SECTION("get/set day of the month")
    {
        for(int i = 1; i < 32; ++i)
        {
            zipios::DOSDateTime t;
            t.setMDay(i);
            CATCH_REQUIRE(t.getMDay() == i);
        }
        for(int i = -20; i <= 0; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 31 + 1);
            t.setMDay(r);
            CATCH_REQUIRE(t.getMDay() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMDay(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMDay() == r);
        }
        for(int i = 32; i <= 52; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 31 + 1);
            t.setMDay(r);
            CATCH_REQUIRE(t.getMDay() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMDay(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMDay() == r);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("get/set month")
    {
        for(int i = 1; i < 12; ++i)
        {
            zipios::DOSDateTime t;
            t.setMonth(i);
            CATCH_REQUIRE(t.getMonth() == i);
        }
        for(int i = -20; i <= 0; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 12 + 1);
            t.setMonth(r);
            CATCH_REQUIRE(t.getMonth() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMonth(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMonth() == r);
        }
        for(int i = 13; i <= 33; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % 12 + 1);
            t.setMonth(r);
            CATCH_REQUIRE(t.getMonth() == r);
            CATCH_REQUIRE_THROWS_AS(t.setMonth(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getMonth() == r);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("get/set year")
    {
        for(int i = 1980; i <= 2107; ++i)
        {
            zipios::DOSDateTime t;
            t.setYear(i);
            CATCH_REQUIRE(t.getYear() == i);
        }
        for(int i = 1000; i < 1980; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % (2107 - 1980 + 1) + 1980);
            t.setYear(r);
            CATCH_REQUIRE(t.getYear() == r);
            CATCH_REQUIRE_THROWS_AS(t.setYear(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getYear() == r);
        }
        for(int i = 2108; i <= 2200; ++i)
        {
            zipios::DOSDateTime t;
            int const r(rand() % (2107 - 1980 + 1) + 1980);
            t.setYear(r);
            CATCH_REQUIRE(t.getYear() == r);
            CATCH_REQUIRE_THROWS_AS(t.setYear(i), zipios::InvalidException);
            CATCH_REQUIRE(t.getYear() == r);
        }
    }
    CATCH_END_SECTION()
}


#if 0
// this test is too long, which is why it is commented out by default
// still, it is great if you want to verify that all possible DOS Time & Date
//
CATCH_TEST_CASE("All Valid DOS Date & Time", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    CATCH_START_SECTION("all valid DOS Date Time values")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_minimum_unix); t <= g_maximum_unix; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            CATCH_REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            CATCH_REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            CATCH_REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            CATCH_REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            CATCH_REQUIRE(u == et);
        }
    }
}
#endif


CATCH_TEST_CASE("Small DOS Date & Time", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    CATCH_START_SECTION("just under the minimum")
    {
        // make sure the minimum limit is checked properly
        //
        for(std::time_t t(g_minimum_unix - 20); t < 0; ++t)
        {
            zipios::DOSDateTime td;
            CATCH_REQUIRE_FALSE(td.isValid());
            CATCH_REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
            CATCH_REQUIRE_FALSE(td.isValid());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("just around minimum, but valid")
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
            CATCH_REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            CATCH_REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            CATCH_REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            CATCH_REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            CATCH_REQUIRE(u == et);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("Large DOS Date & Time", "[dosdatetime]")
{
    if(sizeof(std::time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    init_min_max();

    CATCH_START_SECTION("just around maximum, but valid")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_maximum_unix - 20); t <= g_maximum_unix; ++t)
        {
            std::time_t et((t + 1) & ~1);

            zipios::DOSDateTime td;
            CATCH_REQUIRE_FALSE(td.isValid());
            td.setUnixTimestamp(t);
            CATCH_REQUIRE(td.isValid());

            zipios::DOSDateTime::dosdatetime_t const d(td.getDOSDateTime());

            zipios::DOSDateTime tu;
            CATCH_REQUIRE_FALSE(tu.isValid());
            tu.setDOSDateTime(d);
            CATCH_REQUIRE(tu.isValid());

            std::time_t const u(tu.getUnixTimestamp());
            CATCH_REQUIRE(u == et);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("just a bit too large")
    {
        // make sure the maximum limit is checked properly
        //
        for(std::time_t t(g_maximum_unix + 1); t <= g_maximum_unix + 20; ++t)
        {
            zipios::DOSDateTime td;
            CATCH_REQUIRE(td.getDOSDateTime() == 0);
            CATCH_REQUIRE_FALSE(td.isValid());
            CATCH_REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
            CATCH_REQUIRE_FALSE(td.isValid());
            CATCH_REQUIRE(td.getDOSDateTime() == 0);
            CATCH_REQUIRE(td.getUnixTimestamp() == 0);
        }
    }
    CATCH_END_SECTION()
}


#if INTPTR_MAX != INT32_MAX
// at this time only check on 64 bit computers because the DOS date can
// go out of range in a Unix date when we're on a 32 bit computer
CATCH_TEST_CASE("Random DOS Date & Time", "[dosdatetime]")
{
    init_min_max();

    CATCH_START_SECTION("more random tests")
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
                CATCH_REQUIRE_THROWS_AS(td.setUnixTimestamp(t), zipios::InvalidException);
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
                CATCH_REQUIRE(u == et);
            }
        }
    }
    CATCH_END_SECTION()
}
#endif



// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
