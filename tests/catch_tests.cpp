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
 * \anchor catch_tests_anchor
 *
 * Zipios++ unit test suite using catch.hpp, see for details:
 *
 *   https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
 */

// Ask Catch to define the main() function in this file
#define CATCH_CONFIG_RUNNER

#include "catch_tests.h"

#include "zipios++/zipios-config.hpp"

#include <cstring>



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
        g_progname = e + 1;
    }
    e = strrchr(g_progname, '\\');
    if(e)
    {
        g_progname = e + 1;
    }

    unsigned int seed(static_cast<unsigned int>(time(nullptr)));
    for(int i(1); i < argc; ++i)
    {
        if(strcmp(argv[i], "--seed") == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "error: --seed need to be followed by the actual seed." << std::endl;
                exit(1);
            }
            seed = atoll(argv[i + 1]);
            // remove the --seed and <value>
            for(int j(i); j + 2 < argc; ++j)
            {
                argv[j] = argv[j + 2];
            }
            argc -= 2;
        }
    }
    srand(seed);
    std::cout << g_progname << "[" << getpid() << "]" << ": version " << ZIPIOS_VERSION_STRING << ", seed is " << seed << std::endl;

    return Catch::Session().run(argc, argv);
}



// vim: ts=4 sw=4 et
