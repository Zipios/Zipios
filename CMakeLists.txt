#
# File:
#      CMakeLists.txt
#
# Description:
#      Build zipios++ library, tools, packages, tests.
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

cmake_minimum_required(VERSION 2.8)

project( zipios_project )

set( ZIPIOS_VERSION_MAJOR 2 )
set( ZIPIOS_VERSION_MINOR 0 )
set( ZIPIOS_VERSION_PATCH 0 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Winit-self -Wlogical-op -Wmissing-include-dirs -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wundef -Wno-unused -Wunused-variable -Wno-variadic-macros -Wno-parentheses -Wno-unknown-pragmas -Wwrite-strings -Wswitch -fdiagnostics-show-option -fPIC -Wunused-parameter -Wfloat-equal" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -fPIC -Wunused-parameter -fwrapv" )

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

################################################################################
# Copy of the AddDoxygenTarget from the Snap! C++ project
# https://sourceforge.net/projects/snapcpp/
#
function( AddDoxygenTarget TARGET_NAME VERSION_MAJOR VERSION_MINOR VERSION_PATCH )
    project( ${TARGET_NAME}_Documentation )

    set( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" )
    set( FULL_VERSION "${VERSION}.${VERSION_PATCH}" )

    if( DOXYGEN_FOUND )
        if( NOT DOXYGEN_DOT_FOUND )
            message( WARNING "The dot executable was not found. Did you install Graphviz? No graphic output shall be generated in documentation." )
        endif()

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.doxy.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.doxy @ONLY)
        set( DOCUMENTATION_OUTPUT ${TARGET_NAME}-doc-${VERSION} )

        add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOCUMENTATION_OUTPUT}.tar.gz ${DOCUMENTATION_OUTPUT}
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.doxy
                1> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-doxy.log
                2> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-doxy.err
            COMMAND echo Compacting as ${DOCUMENTATION_OUTPUT}.tar.gz
            COMMAND rm -rf ${DOCUMENTATION_OUTPUT}
            COMMAND mv html ${DOCUMENTATION_OUTPUT}
            COMMAND tar czf ${DOCUMENTATION_OUTPUT}.tar.gz ${DOCUMENTATION_OUTPUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        add_custom_target( ${TARGET_NAME}_Documentation ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOCUMENTATION_OUTPUT}.tar.gz
            COMMENT "Generating API documentation with Doxygen" VERBATIM
        )

        string( TOLOWER ${TARGET_NAME} LOWER_TARGET_NAME )
        install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DOCUMENTATION_OUTPUT}/
                DESTINATION share/doc/${LOWER_TARGET_NAME}/html/
        )
    else()
        message( WARNING "You do not seem to have doxygen installed on this system, no documentation will be generated." )
    endif()
endfunction()

find_package( ZLIB REQUIRED )

configure_file(
    ${CMAKE_SOURCE_DIR}/zipios++/zipios-config.h.in
    ${CMAKE_BINARY_DIR}/zipios++/zipios-config.h
)

AddDoxygenTarget( zipios ${ZIPIOS_VERSION_MAJOR} ${ZIPIOS_VERSION_MINOR} ${ZIPIOS_VERSION_PATCH} )

include_directories( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

add_subdirectory( src )

install( DIRECTORY zipios++
    DESTINATION include
    PATTERN "*.in" EXCLUDE
    PATTERN "*.w32" EXCLUDE
    PATTERN "*.am" EXCLUDE
)

install( FILES ${CMAKE_BINARY_DIR}/zipios++/zipios-config.h
    DESTINATION include/zipios++
)

install( DIRECTORY cmake/ DESTINATION share/cmake-2.8/Modules )

# vim: ts=4 sw=4 et nocindent
