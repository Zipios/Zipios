#
# File:
#      CMakeLists.txt
#
# Description:
#      Build Zipios documentation.
#
# Documentation:
#      See the CMake documentation.
#
# License:
#      Zipios -- a small C++ library that provides easy access to .zip files.
#      Copyright (C) 2000-2007  Thomas Sondergaard
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

cmake_minimum_required(VERSION 2.8)

if (BUILD_DOCUMENTATION)

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
                DESTINATION ${DATA_INSTALL_DIR}/doc/${LOWER_TARGET_NAME}/html/
        )
        # The following installs the man3 files, we only install the man
        # pages of public classes. For more details the user will have to
        # go to the HTML documentation
        install( FILES ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_CollectionCollection.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_DirectoryCollection.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_DirectoryEntry.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_FileCollection.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_FileEntry.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_FilePath.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_VirtualSeeker.3
                       ${CMAKE_CURRENT_BINARY_DIR}/man/man3/zipios_ZipFile.3
                DESTINATION ${DATA_INSTALL_DIR}/man/man3
        )
    else()
        message( WARNING "You do not seem to have doxygen installed on this system, no documentation will be generated." )
    endif()
endfunction()

configure_file( ${CMAKE_SOURCE_DIR}/doc/zipios.doxy.in ${CMAKE_BINARY_DIR}/doc/zipios.doxy )

AddDoxygenTarget( zipios ${ZIPIOS_VERSION_MAJOR} ${ZIPIOS_VERSION_MINOR} ${ZIPIOS_VERSION_PATCH} )

else(BUILD_DOCUMENTATION)

message("No documentation will be created because you explicitly disabled it.")

endif(BUILD_DOCUMENTATION)

# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et nocindent
