/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2021  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/** \file
 *
 * Zipios unit tests for the ZipFile class.
 */

#include "catch_main.hpp"

#include <zipios/zipfile.hpp>
#include <zipios/directorycollection.hpp>
#include <zipios/zipiosexceptions.hpp>
#include <zipios/dosdatetime.hpp>

#include <algorithm>
#include <fstream>

#include <unistd.h>
#include <string.h>
#include <zlib.h>



namespace
{


zipios::StorageMethod const g_supported_storage_methods[]
{
    zipios::StorageMethod::STORED,
    zipios::StorageMethod::DEFLATED
};


} // no name namespace




CATCH_TEST_CASE("The library version", "[Version]")
{
    std::string cmd("cd ");
    cmd += zipios_test::g_source_path;
    cmd += " && dev/version";
    CATCH_REQUIRE(system(cmd.c_str()) == 0);
}


// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et

