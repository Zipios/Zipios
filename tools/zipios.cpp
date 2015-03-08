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
    \anchor appendzip_anchor
    Source code to a small program appendzip that appends a zip
    archive to another file. Run appendzip without arguments to
    get a helpful usage message.
*/

#include "zipios++/ziphead.hpp"

#include <cstring>


// static variables
namespace
{

char *g_progname;

}


void usage()
{
    std::cout << "Usage:  " << g_progname << " [-opt] [file]" << std::endl;
    std::cout << "Where -opt is one or more of:" << std::endl;
    std::cout << "  --count         count the number of files in a .zip archive" << std::endl;
    std::cout << "  --help          show this help screen" << std::endl;
    std::cout << "  --version       print the library version and exit" << std::endl;
    std::cout << "  --version-tool  print the tool version and exit" << std::endl;
    exit(1);
}


enum class func_t
{
    UNDEFINED,
    COUNT
};


int main( int argc, char *argv[] )
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

    // check the various command line options
    std::vector<std::string> files;
    func_t function(func_t::UNDEFINED);
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "--help") == 0)
            {
                usage();
            }
            if(strcmp(argv[i], "--version") == 0)
            {
                // version of the .so library
                std::cout << zipios::get_version() << std::endl;
                exit(0);
            }
            if(strcmp(argv[i], "--version-tool") == 0)
            {
                // version of this tool (compiled with this version)
                // it should be the same as the --version
                std::cout << ZIPIOS_VERSION_STRING << std::endl;
                exit(0);
            }
            if(strcmp(argv[i], "--count") == 0)
            {
                function = func_t::COUNT;
            }
        }
        else
        {
            files.push_back(argv[i]);
        }
    }

    switch(function)
    {
    case func_t::COUNT:
        // TODO: ...
        std::cerr << "sorry, not implemented yet...\n";
        break;

    default:
        std::cerr << g_progname << ":error: undefined function." << std::endl;
        usage();
        break;

    }

    return 0;
}


// vim: ts=4 sw=4 et
