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
 * \brief Define a command line tool to convert Unix/DOS times.
 *
 * A small utility to offer a command line tool that converts Unix to
 * DOS Date & Time and vice versa.
 */

#include "zipios/dosdatetime.hpp"
#include "zipios/zipios-config.hpp"

#include <cstring>
#include <iostream>
#include <stdlib.h>


// static variables
namespace
{

char *g_progname;



void usage()
{
    std::cout << "Usage:  " << g_progname << " [-opt] <time>" << std::endl;
    std::cout << "Where -opt is one or more of:" << std::endl;
    std::cout << "  --dec           display the result in decimal" << std::endl;
    std::cout << "  --dos           convert a Unix time to a DOS time (default)" << std::endl;
    std::cout << "  --help          display this help screen" << std::endl;
    std::cout << "  --hex           display the result in hexadecimal" << std::endl;
    std::cout << "  --unix          convert a DOS time to a Unix time" << std::endl;
    std::cout << "  --version       print the library version and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  to convert a Unix time to a DOS time:" << std::endl;
    std::cout << "    " << g_progname << " 956794294" << std::endl;
    std::cout << "  to convert a DOS time to a Unix time:" << std::endl;
    std::cout << "    " << g_progname << " --unix 681216369" << std::endl;
    std::cout << std::endl;
    std::cout << "Output:" << std::endl;
    std::cout << "<time entered on command line> <converted time> <date (YYYY/MM/DD)> <time (HH:MM:SS)>" << std::endl;
    std::cout << "  Note: <converted time> shows -1 if an error occurs" << std::endl;
    exit(1);
}


enum class time_mode_t
{
    DOS,
    UNIX
};


} // no name namespace


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

    // check the various command line options
    time_mode_t mode(time_mode_t::DOS);
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "--help") == 0
            || strcmp(argv[i], "-h") == 0)
            {
                usage();
            }
            if(strcmp(argv[i], "--version") == 0)
            {
                // version of this tool (compiled with this version)
                // it should be the same as the --version
                std::cout << ZIPIOS_VERSION_STRING << std::endl;
                exit(0);
            }
            if(strcmp(argv[i], "--dec") == 0)
            {
                std::cout << std::dec;
            }
            else if(strcmp(argv[i], "--dos") == 0)
            {
                mode = time_mode_t::DOS;
            }
            else if(strcmp(argv[i], "--hex") == 0)
            {
                std::cout << std::hex;
            }
            else if(strcmp(argv[i], "--unix") == 0)
            {
                mode = time_mode_t::UNIX;
            }
        }
        else
        {
            // TODO: test that the argument is a valid number
            int64_t const t(atoll(argv[i]));
            int64_t r(0);
            time_t dt(t);
            zipios::DOSDateTime conv;
            switch(mode)
            {
            case time_mode_t::DOS:
                conv.setUnixTimestamp(t);
                r = conv.getDOSDateTime();
                break;

            case time_mode_t::UNIX:
                conv.setDOSDateTime(t);
                r = conv.getUnixTimestamp();
                dt = r;
                break;

            }
            struct tm *dtm(gmtime(&dt));
            char buf[256];
            if(dt == -1)
            {
                strcpy(buf, "- -");
            }
            else
            {
                strftime(buf, sizeof(buf), "%Y/%m/%d %T", dtm);
            }
            buf[255] = '\0';
            std::cout << t << " " << r << " " << buf << std::endl;
        }
    }

    return 0;
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
