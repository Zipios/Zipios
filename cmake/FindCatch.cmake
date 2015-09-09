#
# Find the Catch.hpp header file
#
# This module defines
# CATCH_INCLUDE_DIR, where to find catch.hpp
# CATCH_FOUND, If false, do not try to use Catch.
#
# License:
#      Zipios++ - a small C++ library that provides easy access to .zip files.
#      Copyright (C) 2015  Made to Order Software Corporation
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

SET(CATCH_FOUND "NO")

FIND_PATH(CATCH_INCLUDE_DIR
    catch.hpp
    PATHS
        ${CMAKE_SOURCE_DIR}/contrib
        /usr/local/include
        /usr/include
)

IF(CATCH_INCLUDE_DIR)
    IF(CATCH_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find library Catch.")
    ELSE(CATCH_FIND_REQUIRED)
        SET(CATCH_FOUND "YES")
    ENDIF(CATCH_FIND_REQUIRED)
ELSE(CATCH_INCLUDE_DIR)
    IF(CATCH_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find library Catch.")
    ENDIF(CATCH_FIND_REQUIRED)
ENDIF(CATCH_INCLUDE_DIR)

# vim: ts=4 sw=4 et
