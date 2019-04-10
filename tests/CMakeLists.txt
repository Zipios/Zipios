#
# File:
#      CMakeLists.txt
#
# Description:
#      Build Zipios tests.
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

find_package( Catch )

OPTION(BUILD_ZIPIOS_TESTS "Whether the zipios tests should be built. True by default." ON)

if(BUILD_ZIPIOS_TESTS)

message("Tests are turned ON.")

if(CATCH_FOUND)

project( zipios_tests )

include_directories(
    ${CATCH_INCLUDE_DIR}
)

add_executable( ${PROJECT_NAME}
    tests.cpp

    backbuffer.cpp
    collectioncollection.cpp
    common.cpp
    directorycollection.cpp
    directoryentry.cpp
    dosdatetime.cpp
    filepath.cpp
    stream.cpp
    virtualseeker.cpp
    zipfile.cpp

    directory_helper.cpp
    raii_helper.cpp
)

target_link_libraries( ${PROJECT_NAME}
    zipios
)

add_custom_target(run_zipios_tests
    # You can use the --success command line option to see all the tests
    # as they run; it is a LOT of output though, thus by default we don't
    # use it
    COMMAND ./zipios_tests
)

add_test(zipios_tests ${PROJECT_NAME})

else(CATCH_FOUND)

message("No test will be created because you do not seem to have catch.hpp installed...")

# For compatibility, just in case
add_custom_target(run_zipios_tests
    COMMAND echo "No tests were built because it looks like you are missing Catch."
)

endif(CATCH_FOUND)

else(BUILD_ZIPIOS_TESTS)

message("Tests are turned OFF. See BUILD_ZIPIOS_TESTS option.")

endif(BUILD_ZIPIOS_TESTS)

# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et
