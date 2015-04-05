#
# File:
#      CMakeLists.txt
#
# Description:
#      Build zipios++ library, tools, packages, tests, documentation.
#
# Documentation:
#      See the CMake documentation.
#
# License:
#      Zipios++ - a small C++ library that provides easy access to .zip files.
#      Copyright (C) 2000-2015  Thomas Sondergaard
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

cmake_minimum_required(VERSION 2.8.4)

project( zipios_project )

set( ZIPIOS_VERSION_MAJOR 2 )
set( ZIPIOS_VERSION_MINOR 0 )
set( ZIPIOS_VERSION_PATCH 2 )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# SUNOS is not set by cmake
string( REGEX MATCH "SunOS" SUNOS ${CMAKE_SYSTEM_NAME} )

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fdiagnostics-show-option -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Winit-self -Wlogical-op -Wmissing-include-dirs -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wundef -Wno-unused -Wunused-variable -Wno-variadic-macros -Wno-parentheses -Wno-unknown-pragmas -Wwrite-strings -Wswitch -Wunused-parameter -Wfloat-equal -Wold-style-cast -Wnoexcept" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fwrapv" )
set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Werror -Wall -Wextra -Wunused-parameter" )
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3" )


if( CYGWIN )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11" )
else()
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC" )
endif()

# To generate coverage, use -D<project name>_COVERAGE=ON
#                       and -DCMAKE_BUILD_TYPE=Debug
option( ${PROJECT_NAME}_COVERAGE "Turn on coverage for ${PROJECT_NAME}." OFF )

if( ${${PROJECT_NAME}_COVERAGE} )
    message("*** COVERAGE TURNED ON ***")
    find_program( COV gcov )
    if( ${COV} STREQUAL "COV-NOTFOUND" )
        message( FATAL_ERROR "Coverage requested, but gcov not installed!" )
    endif()
    if( NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
        message( FATAL_ERROR "Coverage requested, but Debug is not turned on! (i.e. -DCMAKE_BUILD_TYPE=Debug)" )
    endif()
    #
    set( COV_C_FLAGS             "-fprofile-arcs -ftest-coverage" )
    set( COV_CXX_FLAGS           "-fprofile-arcs -ftest-coverage" )
    set( COV_SHARED_LINKER_FLAGS "-fprofile-arcs -ftest-coverage" )
    set( COV_EXE_LINKER_FLAGS    "-fprofile-arcs -ftest-coverage" )
    #
    set( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} ${COV_C_FLAGS}"                         )
    set( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} ${COV_CXX_FLAGS}"                     )
    set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COV_SHARED_LINKER_FLAGS}" )
    set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${COV_EXE_LINKER_FLAGS}"       )
endif()

find_package( ZLIB REQUIRED )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/zipios++/zipios-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/zipios++/zipios-config.hpp )

# Generate the RPM package specification file
set(PACKAGE "libzipios++")
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zipios++.spec.in ${CMAKE_CURRENT_BINARY_DIR}/contrib/zipios++.spec )

include_directories( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

add_subdirectory( src )
add_subdirectory( tools )
add_subdirectory( tests )
add_subdirectory( doc )

install( DIRECTORY zipios++
    DESTINATION include
    PATTERN "*.in" EXCLUDE
)

install( FILES ${CMAKE_BINARY_DIR}/zipios++/zipios-config.hpp
    DESTINATION include/zipios++
)

install( DIRECTORY cmake/
    DESTINATION share/cmake-2.8/Modules
)

add_custom_target(zipios_code_analysis
    # Make sure we have an output folder
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/analysis

    # Boost inspect tool that reports various problems in the source
    COMMAND echo "inspect -- output ${PROJECT_BINARY_DIR}/analysis/inspect.html"
    COMMAND inspect -tab -crlf -end -path_name -ascii -minmax -assert_macro -deprecated_macro -unnamed -copyright >"${PROJECT_BINARY_DIR}/analysis/inspect.html"

    # Count the number of TODO, XXX, TBD, FIXME, and \todo
    COMMAND echo "TODO -- output ${PROJECT_BINARY_DIR}/analysis/todo.txt"
    COMMAND sh dev/todo.sh "${PROJECT_BINARY_DIR}/analysis"

    # Search for files with "invalid" (unwanted really) spaces
    COMMAND echo "Spaces -- output ${PROJECT_BINARY_DIR}/analysis/spaces.txt"
    COMMAND sh dev/spaces.sh "${PROJECT_BINARY_DIR}/analysis"

    # All of these are expected to work on source code so make sure we are
    # in the source code top directory
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

##
## To pack the source
##
set(CPACK_PACKAGE_NAME "zipios")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Zipios++ is a small C++ library for reading and writing zip files.")
set(CPACK_PACKAGE_VENDOR "Made to Order Software Corporation")
set(CPACK_PACKAGE_CONTACT "alexis@m2osw.com")
set(CPACK_RESOURCE_FILE_LICENSE "${zipios_project_SOURCE_DIR}/COPYING")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/CVS/;/work-files/;/.git/;.swp$;.*~;cscope.*;/tmp/;BUILD;Build")
set(CPACK_PACKAGE_VERSION "${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}.${ZIPIOS_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${ZIPIOS_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ZIPIOS_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ZIPIOS_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "zipios-${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}.${ZIPIOS_VERSION_PATCH}")
include(CPack)

# vim: ts=4 sw=4 et nocindent
