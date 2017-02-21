/*
  Zipios++ - a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2017  Made to Order Software Corporation

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
 * Zipios++ unit tests verify the dostime.c/h private implementations.
 *
 * \warning
 * This directly accesses dostime.h which is a private header file.
 */

#include "tests.hpp"

#include "src/dostime.h"

#include <fstream>

#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

TEST_CASE("Unix to DOS time conversions and vice versa", "[dostime]")
{
    REQUIRE(mindostime() == dostime(1980, 1, 1, 0, 0, 0));
    REQUIRE(maxdostime() == dostime(2107, 12, 31, 23, 59, 59));

    if(sizeof(time_t) < sizeof(uint64_t))
    {
        std::cerr << "warning: Unix to DOS time conversion is ignored on platform with a 32 bit time_t definition." << std::endl;
        return;
    }

    time_t const clock = time(NULL);
    struct tm *mm_time = localtime(&clock);

    mm_time->tm_isdst = -1;     /* let mktime() determine if DST is in effect */
    mm_time->tm_sec   = 0; // min = Jan 1, 1980 at 00:00:00
    mm_time->tm_min   = 0;
    mm_time->tm_hour  = 0;
    mm_time->tm_mday  = 1;
    mm_time->tm_mon   = 0;
    mm_time->tm_year  = 80;
    time_t const minimum_unix = mktime(mm_time);

    mm_time->tm_isdst = -1;     /* let mktime() determine if DST is in effect */
    mm_time->tm_sec   = 59; // max = Dec 3, 2107 at 23:59:59
    mm_time->tm_min   = 59;
    mm_time->tm_hour  = 23;
    mm_time->tm_mday  = 31;
    mm_time->tm_mon   = 11;
    mm_time->tm_year  = 207;
    time_t const maximum_unix = mktime(mm_time);

    // make sure the minimum limit is checked properly
    for(time_t t(minimum_unix - 20); t <= minimum_unix + 20; ++t)
    {
        time_t et((t + 1) & ~1);
        dostime_t const d(unix2dostime(t));
        time_t const u(dos2unixtime(d));
        if(et < minimum_unix)
        {
            REQUIRE(d == 0);
            REQUIRE(u == -1);
        }
        else
        {
            // TODO: add necessary code to verify DST?
            long long int const diff(llabs(u - et));
            bool const valid(diff == 0 || diff == 3600);
            REQUIRE(valid);
        }
    }

    // make sure the maximum limit is checked properly
    for(time_t t(maximum_unix - 20); t <= maximum_unix + 20; ++t)
    {
        time_t et((t + 1) & ~1);
        dostime_t const d(unix2dostime(t));
        time_t const u(dos2unixtime(d));
        if(et > maximum_unix)
        {
            REQUIRE(d == 0);
            REQUIRE(u == -1);
        }
        else
        {
            // TODO: add necessary code to verify DST?
            long long int const diff(llabs(u - et));
            bool const valid(diff == 0 || diff == 3600);
            REQUIRE(valid);
        }
    }

    // we verified that time_t is at least a 64 bit type
#if defined(_ILP32)
    for(time_t t(0); t <=  0x7FFFFFFF; t += rand() & 0x7FFF)
#else
    for(time_t t(0); t <= 0x104000000; t += rand() & 0xFFFF)
#endif
    {
        time_t et((t + 1) & ~1);
        dostime_t const d(unix2dostime(t));
        time_t const u(dos2unixtime(d));
        if(et < minimum_unix)
        {
            REQUIRE(d == 0);
            REQUIRE(u == -1);
        }
        else if(et > maximum_unix)
        {
            REQUIRE(d == 0);
            REQUIRE(u == -1);
        }
        else
        {
            // TODO: add necessary code to verify DST?
            long long int const diff(llabs(u - et));
            bool const valid(diff == 0 || diff == 3600);
            REQUIRE(valid);
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
