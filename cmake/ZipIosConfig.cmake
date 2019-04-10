# - Try to find the Zipios++ (libzipios.so)
#
# Once done this will define
#
# ZIPIOSCC_FOUND        - System has Zipios++
# ZIPIOSCC_INCLUDE_DIR  - The zipios include directories
# ZIPIOSCC_LIBRARY      - The libraries needed to use Zipios++ (none)
# ZIPIOSCC_DEFINITIONS  - Compiler switches required for using Zipios++ (none)
#
# License:
#      Zipios -- a small C++ library that provides easy access to .zip files.
#      Copyright (C) 2015-2019  Made to Order Software Corporation
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

find_path( ZIPIOSCC_INCLUDE_DIR zipios/zipfile.hpp
                          HINTS $ENV{ZIPIOSCC_INCLUDE_DIR}
                  PATH_SUFFIXES zipios
)

find_library( ZIPIOSCC_LIBRARY zipios
                         HINTS $ENV{ZIPIOSCC_LIBRARY}
)

mark_as_advanced( ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

set( ZIPIOSCC_INCLUDE_DIRS ${ZIPIOSCC_INCLUDE_DIR} )
set( ZIPIOSCC_LIBRARIES    ${ZIPIOSCC_LIBRARY}     )

include( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set ZIPIOSCC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( ZipIos DEFAULT_MSG ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

# vim: ts=4 sw=4 et
