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
 * \brief Tool used to append a Zip archive at the end of another file.
 * \anchor appendzip_anchor
 *
 * Source code to a small program appendzip that appends a zip
 * archive to another file. Run appendzip without arguments to
 * get a helpful usage message.
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdint.h>


// static variables
namespace
{

char *g_progname;


void usage()
{
    // see the ZipFile::openEmbeddedZipFile() function for details...
    std::cout << "Usage:  " << g_progname << " exe-file zipfile" << std::endl;
    std::cout << "This tool appends a zipfile at the end of any other file (most often a .exe under MS-Windows)." << std::endl;
    std::cout << "The openEmbeddedZipFile() function can then be used to read the file." << std::endl;
    exit(1);
}

} // no name namespace


int main(int argc, char *argv[])
{
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

    if(argc != 3)
    {
        usage();
    }

    std::ofstream exef(argv[1], std::ios::app | std::ios::binary);
    if(!exef)
    {
        std::cerr << g_progname << ":error: Unable to open " << argv[1] << " for writing" << std::endl;
        usage();
    }

    std::ifstream zipf(argv[2], std::ios::in | std::ios::binary);
    if(!zipf)
    {
        std::cerr << g_progname << ":error: Unable to open " << argv[2] << " for reading." << std::endl;
        usage();
    }

    // get eof pos (to become zip file starting position).
    uint32_t const zip_start = exef.tellp();
    std::cout << "zip start will be at " << zip_start << std::endl;

    // Append zip file to exe file
    exef << zipf.rdbuf();

    // write zipfile start offset to file
    exef << static_cast<unsigned char>(zip_start);
    exef << static_cast<unsigned char>(zip_start >> 8);
    exef << static_cast<unsigned char>(zip_start >> 16);
    exef << static_cast<unsigned char>(zip_start >> 24);
    //zipios::writeUint32(zip_start, exef); -- TODO: delete once verified

    return 0;
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
