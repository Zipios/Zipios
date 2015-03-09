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
    \anchor example_zip_anchor
    source code to a small program that demonstrates the central elements
    of Zipios++.
*/

#include "zipios++/zipfile.hpp"

//#include <memory>


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "error: usage: zipios_example <path to test.zip> <filename to extract and print>" << std::endl;
        return 1;
    }

    try
    {
        std::cout << "Instantiating a ZipFile" << std::endl;
        zipios::ZipFile zf(argv[1]);

        std::cout << "list length: " << zf.size() << std::endl;

        zipios::FileEntry::vector_t entries(zf.entries());
        for(auto it = entries.begin(); it != entries.end(); ++it)
        {
            std::cout << "  " << *(*it) << std::endl;
        }

        zipios::FileEntry::pointer_t ent(zf.getEntry(argv[2], zipios::FileCollection::MatchPath::IGNORE));
        if(ent)
        {
            zipios::ZipFile::stream_pointer_t is(zf.getInputStream(ent->getName()));
            if(is)
            {
                std::cout << "Contents of entry, " << ent->getName() << ":" << std::endl;

                std::cout << is->rdbuf();
            }
            else
            {
                std::cerr << "error: found an entry for " << ent->getName() << " in test.zip, but could not read it with getInputStream()" << std::endl;
            }
        }
        else
        {
            std::cerr << "error: could not read " << ent->getName() << " from test.zip" << std::endl;
        }

        std::cout << "end of main()" << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception caught in main() :" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// vim: ts=4 sw=4 et
