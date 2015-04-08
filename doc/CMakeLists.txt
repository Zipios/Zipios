#
# File:
#      CMakeLists.txt
#
# Description:
#      Build zipios++ documentation.
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

project( zipios_documentation )

################################################################################
# Copy of the AddDoxygenTarget from the Snap! C++ project
# https://sourceforge.net/projects/snapcpp/
#
find_package( Doxygen )
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

        if(SUNOS)
            set(TAR_OPTIONS cEzf)
        else()
            set(TAR_OPTIONS czf)
        endif()

        add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOCUMENTATION_OUTPUT}.tar.gz ${DOCUMENTATION_OUTPUT}
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.doxy
                1> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-doxy.log
                2> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-doxy.err
            COMMAND echo Compacting as ${DOCUMENTATION_OUTPUT}.tar.gz
            COMMAND rm -rf ${DOCUMENTATION_OUTPUT}
            COMMAND mv html ${DOCUMENTATION_OUTPUT}
            COMMAND tar ${TAR_OPTIONS} ${DOCUMENTATION_OUTPUT}.tar.gz ${DOCUMENTATION_OUTPUT}
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

configure_file( ${CMAKE_SOURCE_DIR}/doc/zipios.doxy.in ${CMAKE_BINARY_DIR}/doc/zipios.doxy )

AddDoxygenTarget( zipios ${ZIPIOS_VERSION_MAJOR} ${ZIPIOS_VERSION_MINOR} ${ZIPIOS_VERSION_PATCH} )

# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et nocindent
