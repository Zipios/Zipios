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
 *
 * Zipios RAII objects used in various unit tests.
 */

#include "catch_main.hpp"

#include <unistd.h>


namespace zipios_test
{


auto_unlink_t::auto_unlink_t(std::string const & filename, bool delete_on_creation)
    : m_filename(filename)
{
    if(delete_on_creation)
    {
        unlink();
    }
}

auto_unlink_t::~auto_unlink_t()
{
    unlink();
}


void auto_unlink_t::unlink()
{
    system(("rm -rf " + m_filename).c_str());
}



safe_chdir::safe_chdir(std::string const & path)
    : m_original_path(get_current_dir_name())
{
    if(m_original_path == nullptr)
    {
        int const e(errno);
        throw std::logic_error(
              "the current working directory could not be queried (err: "
            + std::to_string(e)
            + ", "
            + strerror(e)
            + ").");
    }

    if(chdir(path.c_str()) != 0)
    {
        if(errno == EFAULT)
        {
            throw std::logic_error("the path variable is invalid.");
        }
        if(errno == ENOMEM)
        {
            throw std::bad_alloc();
        }
        int const e(errno);
        throw std::runtime_error(
                  "chdir() generated error: "
                + std::to_string(e)
                + ", "
                + strerror(e));
    }
}


safe_chdir::~safe_chdir()
{
    chdir(m_original_path.get());
}



} // zipios_tests namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
