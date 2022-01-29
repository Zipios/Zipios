#
# Find the catch.hpp header file
#
# This module defines
# CATCH_INCLUDE_DIR, where to find catch.hpp
# CATCH_FOUND, If false, do not try to use Catch.
#
# License:
#      Zipios -- a small C++ library that provides easy access to .zip files.
#      Copyright (c) 2015-2022  Made to Order Software Corp.  All Rights Reserved
#
#      This library is free software; you can redistribute it and/or
#      modify it under the terms of the GNU Lesser General Public
#      License as published by the Free Software Foundation; either
#      version 2.1 of the License, or (at your option) any later version.
#
#      This library is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#      Lesser General Public License for more details.
#
#      You should have received a copy of the GNU Lesser General Public
#      License along with this library; if not, write to the Free Software
#      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

set(CATCH_FOUND "NO")

# The PATH_SUFFXES is necessary under Fedora
#
find_path(CATCH_INCLUDE_DIR
    catch.hpp
    PATHS
        ${CMAKE_SOURCE_DIR}/contrib
        /usr/local/include/catch2
        /usr/include/catch2
        /usr/local/include
        /usr/include
    PATH_SUFFIXES
        catch
)

if(CATCH_INCLUDE_DIR)
    set(CATCH_FOUND "YES")
else(CATCH_INCLUDE_DIR)
    if(Catch_FIND_REQUIRED)
        message(SEND_ERROR "Could not find library Catch.")
    endif(Catch_FIND_REQUIRED)
endif(CATCH_INCLUDE_DIR)

# vim: ts=4 sw=4 et
