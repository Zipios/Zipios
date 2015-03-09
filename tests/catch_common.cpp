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
 * Zipios++ unit tests used to verify the various funtions defined in
 * zipios_common.hpp.
 */

#include "catch_tests.h"

#include "src/zipios_common.hpp"

//#include <fstream>
//
//#include <sys/stat.h>
//#include <unistd.h>


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


// vim: ts=4 sw=4 et
