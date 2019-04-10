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
 * \brief A tool to test various things in a zip file.
 *
 * Also zipios can be used to determine the version of your Zipios
 * installation (this tool version, the libzipios.so/dll version, etc.)
 *
 * \todo
 * Actually make this tool really useful (as a simple replacement to
 * zip and unzip for example).
 */

#include "zipios/zipfile.hpp"
#include "zipios/zipiosexceptions.hpp"

#include <cstring>

#include <stdlib.h>


/** \brief A few static variables and functions.
 *
 * This namesapce includes various declarations, variables, and funtions
 * that are specific to the zipios tool, not to be shared with anyone else.
 */
namespace
{

/** \brief Name of the program.
 *
 * This variable holds the name of the program. As soon as the main()
 * function is entered, this variable gets defined.
 */
char *g_progname;


/** \brief Usage of the zipios tool.
 *
 * This function prints out the zipios tool usage and then exits with
 * error code 1.
 */
void usage()
{
    std::cout << "Usage:  " << g_progname << " [-opt] [file]" << std::endl;
    std::cout << "Where -opt is one or more of:" << std::endl;
    std::cout << "  --count                 count the number of files in a .zip archive" << std::endl;
    std::cout << "  --count-directories     count the number of files in a .zip archive" << std::endl;
    std::cout << "  --count-files           count the number of files in a .zip archive" << std::endl;
    std::cout << "  --help                  show this help screen" << std::endl;
    std::cout << "  --version               print the library version and exit" << std::endl;
    std::cout << "  --version-tool          print the tool version and exit" << std::endl;
    exit(1);
}


/** \brief The function to apply.
 *
 * This enumeration lists all the functions the zipios tool can apply to
 * a Zip archive file.
 */
enum class func_t
{
    /** \brief Still undefined.
     *
     * This is used to know whether the user had a function on the command
     * line that would be run by zipios.
     */
    UNDEFINED,

    /** \brief Count the number of files in a Zip archive.
     *
     * This function is used when the user specify --count. It represents
     * the total number of entries in a Zip archive.
     */
    COUNT,

    /** \brief Count the number of directories in a Zip archive.
     *
     * This function is used when the user specify --count-directories.
     * It represents the number of entries representing directories.
     */
    COUNT_DIRECTORIES,

    /** \brief Count the number of regular files in a Zip archive.
     *
     * This function is used when the user specify --count-files.
     * It represents the number of entries representing regular files
     * found in a Zip archive.
     */
    COUNT_FILES
};

} // no name namespace


int main(int argc, char *argv[])
{
    // define program name
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
    }

    try
    {
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
                    std::cout << zipios::getVersion() << std::endl;
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
                else if(strcmp(argv[i], "--count-directories") == 0)
                {
                    function = func_t::COUNT_DIRECTORIES;
                }
                else if(strcmp(argv[i], "--count-files") == 0)
                {
                    function = func_t::COUNT_FILES;
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
            for(auto it(files.begin()); it != files.end(); ++it)
            {
                zipios::ZipFile zf(*it);
                if(files.size() > 1)
                {
                    // write filename in case there is more than one file
                    std::cout << *it << ": ";
                }
                std::cout << zf.entries().size() << std::endl;
            }
            break;

        case func_t::COUNT_DIRECTORIES:
            for(auto it(files.begin()); it != files.end(); ++it)
            {
                zipios::ZipFile zf(*it);
                if(files.size() > 1)
                {
                    // write filename in case there is more than one file
                    std::cout << *it << ": ";
                }
                int count(0);
                zipios::FileEntry::vector_t entries(zf.entries());
                for(auto entry(entries.begin()); entry != entries.end(); ++entry)
                {
                    if((*entry)->isDirectory())
                    {
                        ++count;
                    }
                }
                std::cout << count << std::endl;
            }
            break;

        case func_t::COUNT_FILES:
            for(auto it(files.begin()); it != files.end(); ++it)
            {
                zipios::ZipFile zf(*it);
                if(files.size() > 1)
                {
                    // write filename in case there is more than one file
                    std::cout << *it << ": ";
                }
                int count(0);
                zipios::FileEntry::vector_t entries(zf.entries());
                for(auto entry(entries.begin()); entry != entries.end(); ++entry)
                {
                    if(!(*entry)->isDirectory())
                    {
                        ++count;
                    }
                }
                std::cout << count << std::endl;
            }
            break;

        default:
            std::cerr << g_progname << ":error: undefined function." << std::endl;
            usage();
            break;

        }
    }
    catch(zipios::Exception const & e)
    {
        std::cerr << g_progname << ":error: an exception occurred: "
                  << e.what() << std::endl;
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
