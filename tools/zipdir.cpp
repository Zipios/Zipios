/*
  Zipios -- a small C++ library that provides easy access to .zip files.

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
 * \brief Tool used to create a Zip archive from a file or a directory.
 * \anchor appendzip_anchor
 *
 * This tool is mainly an example of how one can create a zip file using
 * the zipios library. It is not in any way meant to replace the standard
 * zip command line tool (although it's probably much smaller if you do
 * not need all the capabilities offered by zip).
 */

#include <zipios/directorycollection.hpp>
#include <zipios/zipfile.hpp>

//#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
//#include <stdint.h>


// static variables
namespace
{

char *g_progname;


void usage()
{
    std::cout << "Usage:  " << g_progname << " <output>[.zip] <input-dir>" << std::endl;
    std::cout << "This tool creates a zip file from a directory or a file." << std::endl;
    std::cout << "This is a way to exercise the library." << std::endl;
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

    int limit(256);
    zipios::FileEntry::CompressionLevel level(zipios::FileEntry::COMPRESSION_LEVEL_DEFAULT);
    std::string in;
    std::string out;
    for(int i(1); i < argc; ++i)
    {
        if(strcmp(argv[i], "-h") == 0
        || strcmp(argv[i], "--help") == 0)
        {
            usage();
        }
        if(strcmp(argv[i], "-V") == 0
        || strcmp(argv[i], "--version") == 0)
        {
            std::cout << ZIPIOS_VERSION_STRING << std::endl;
            exit(0);
        }
        if(strcmp(argv[i], "--level") == 0)
        {
            ++i;
            if(i >= argc)
            {
                std::cerr << "error: the --level option must be followed by a level.";
                return 1;
            }
            if(strcmp(argv[i], "default") == 0)
            {
                level = zipios::FileEntry::COMPRESSION_LEVEL_DEFAULT;
            }
            else if(strcmp(argv[i], "smallest") == 0)
            {
                level = zipios::FileEntry::COMPRESSION_LEVEL_SMALLEST;
            }
            else if(strcmp(argv[i], "fastest") == 0)
            {
                level = zipios::FileEntry::COMPRESSION_LEVEL_FASTEST;
            }
            else if(strcmp(argv[i], "none") == 0)
            {
                level = zipios::FileEntry::COMPRESSION_LEVEL_NONE;
            }
            else
            {
                level = static_cast<zipios::FileEntry::CompressionLevel>(std::atoi(argv[i]));
                if(level < zipios::FileEntry::COMPRESSION_LEVEL_MINIMUM
                || level > zipios::FileEntry::COMPRESSION_LEVEL_MAXIMUM)
                {
                    std::cerr
                        << "error: the --level parameter expects one of"
                           " \"default\", \"smallest\", \"fastest\", \"none\""
                           " or a number between "
                        << zipios::FileEntry::COMPRESSION_LEVEL_MINIMUM
                        << " and "
                        << zipios::FileEntry::COMPRESSION_LEVEL_MAXIMUM
                        << ".\n";
                    return 1;
                }
            }
        }
        else if(strcmp(argv[i], "--limit") == 0)
        {
            ++i;
            if(i >= argc)
            {
                std::cerr << "error: the --limit option must be followed by a limit.";
                return 1;
            }
            if(strcmp(argv[i], "default") == 0)
            {
                limit = 256;
            }
            else
            {
                limit = std::atoi(argv[i]);
            }
        }
        else if(out.empty())
        {
            out = argv[i];
        }
        else if(in.empty())
        {
            in = argv[i];
        }
        else
        {
            std::cerr << "error: unknown command line option \""
                << argv[i]
                << "\". Try --help for additional information.";
            return 1;
        }
    }

    if(out.empty())
    {
        std::cerr << "error: missing input directory name on command line.\n";
        return 1;
    }
    if(in.empty())
    {
        in = out;
    }

    zipios::DirectoryCollection collection(in);

    // at this time, we do not have support for any other method
    // so no need for a command line option
    //
    if(level == zipios::FileEntry::COMPRESSION_LEVEL_NONE)
    {
        // ignore the method if the compression is set to "none"
        //
        collection.setMethod(
                  limit
                , zipios::StorageMethod::STORED
                , zipios::StorageMethod::STORED);
        collection.setLevel(
                  limit
                , zipios::FileEntry::COMPRESSION_LEVEL_NONE
                , zipios::FileEntry::COMPRESSION_LEVEL_NONE);
    }
    else
    {
        collection.setMethod(
                  limit
                , zipios::StorageMethod::STORED
                , zipios::StorageMethod::DEFLATED);
        collection.setLevel(
                  limit
                , zipios::FileEntry::COMPRESSION_LEVEL_NONE
                , level);
    }

    std::string zipname(out);
    if(zipname.find(".zip", zipname.length() - 4) == std::string::npos)
    {
        zipname += ".zip";
    }
    std::ofstream output(zipname, std::ios_base::binary);

    zipios::ZipFile::saveCollectionToArchive(output, collection);

    return 0;
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
