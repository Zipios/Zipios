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
 * \anchor catch_tests_anchor
 *
 * Zipios unit test suite using catch.hpp, see for details:
 *
 *   https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
 */

// Prevent Catch from defining a default main() function in this file
// but let it know this is the file that does contain the main() function
#define CATCH_CONFIG_RUNNER

#include "catch_main.hpp"



int main(int argc, char *argv[])
{
    return SNAP_CATCH2_NAMESPACE::snap_catch2_main(
              "zipios"
            , ZIPIOS_VERSION_STRING
            , argc
            , argv
        );
}



// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
