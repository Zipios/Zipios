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
#      Zipios++ - a small C++ library that provides easy access to .zip files.
#      Copyright (C) 2015  Made to Order Software Corporation
#
#      This library is free software; you can redistribute it and/or
#      modify it under the terms of the GNU Lesser General Public
#      License as published by the Free Software Foundation; either
#      version 2 of the License, or (at your option) any later version.
#
#      This library is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#      Lesser General Public License for more details.
#
#      You should have received a copy of the GNU Lesser General Public
#      License along with this library; if not, write to the Free Software
#      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
#

find_path( ZIPIOSCC_INCLUDE_DIR zipios/zipfile.h
		   PATHS /usr/include /usr/local/include $ENV{ZIPIOSCC_INCLUDE_DIR}
		   PATH_SUFFIXES zipios
		 )
find_library( ZIPIOSCC_LIBRARY libzipios.so
			PATHS /usr/lib /usr/local/lib $ENV{ZIPIOSCC_LIBRARY}
		 )
mark_as_advanced( ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

set( ZIPIOSCC_INCLUDE_DIRS ${ZIPIOSCC_INCLUDE_DIR} )
set( ZIPIOSCC_LIBRARIES    ${ZIPIOSCC_LIBRARY}     )

include( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set ZIPIOSCC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( ZipIos DEFAULT_MSG ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

