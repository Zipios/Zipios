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
 * \anchor catch_tests_anchor
 *
 * Zipios unit test suite using catch.hpp, see for details:
 *
 *   https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
 */

// Prevent Catch from defining a default main() function in this file
// but let it know this is the file that does contain the main() function
#define CATCH_CONFIG_RUNNER

#include "tests.hpp"

#include <cstring>

#include <stdlib.h>


// static variables
namespace
{

char *g_progname;

}


int main(int argc, char *argv[])
{
    // define program name
    g_progname = argv[0];
    char *e(strrchr(g_progname, '/'));
    if(e)
    {
        g_progname = e + 1; // LCOV_EXCL_LINE
    }
    e = strrchr(g_progname, '\\');
    if(e)
    {
        g_progname = e + 1; // LCOV_EXCL_LINE
    }

    unsigned int seed(static_cast<unsigned int>(time(nullptr)));
    bool help(false);
    for(int i(1); i < argc; ++i)
    {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) // LCOV_EXCL_LINE
        {
            help = true; // LCOV_EXCL_LINE
        }
        else if(strcmp(argv[i], "--seed") == 0) // LCOV_EXCL_LINE
        {
            if(i + 1 >= argc) // LCOV_EXCL_LINE
            {
                std::cerr << "error: --seed needs to be followed by the actual seed." << std::endl; // LCOV_EXCL_LINE
                exit(1); // LCOV_EXCL_LINE
            }
            seed = atoll(argv[i + 1]); // LCOV_EXCL_LINE
            // remove the --seed and <value>
            for(int j(i); j + 2 < argc; ++j) // LCOV_EXCL_LINE
            {
                argv[j] = argv[j + 2]; // LCOV_EXCL_LINE
            }
            argc -= 2; // LCOV_EXCL_LINE
        }
    }
    srand(seed);
    std::cout << g_progname << "[" << getpid() << "]" << ": version " << ZIPIOS_VERSION_STRING << ", seed is " << seed << std::endl;

    if(help)
    {
        std::cout << std::endl // LCOV_EXCL_LINE
                  << "WARNING: at this point we hack the main() to add the following options:" << std::endl // LCOV_EXCL_LINE
                  << "  --seed <seed>    to force the seed at the start of the process to a specific value (i.e. to reproduce the exact same test over and over again)" << std::endl // LCOV_EXCL_LINE
                  << std::endl; // LCOV_EXCL_LINE
    }

    return Catch::Session().run(argc, argv);
}



// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
