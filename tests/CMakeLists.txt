# Zipios -- a small C++ library that provides easy access to .zip files.
# Copyright (C) 2000-2007  Thomas Sondergaard
# Copyright (C) 2015-2022  Made to Order Software Corp.  All Rights Reserved
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

project(unittest)

OPTION(BUILD_ZIPIOS_TESTS "Whether the zipios tests should be built. True by default." ON)

if(BUILD_ZIPIOS_TESTS)

    message("Tests are turned ON.")

    find_package(SnapCatch2)

    if(SNAPCATCH2_FOUND)

        add_executable(${PROJECT_NAME}
            catch_main.cpp

            catch_backbuffer.cpp
            catch_collectioncollection.cpp
            catch_common.cpp
            catch_directorycollection.cpp
            catch_directoryentry.cpp
            catch_dosdatetime.cpp
            catch_filepath.cpp
            catch_stream.cpp
            catch_version.cpp
            catch_virtualseeker.cpp
            catch_zipfile.cpp

            catch_directory_helper.cpp
            catch_raii_helpers.cpp
        )

        target_include_directories(${PROJECT_NAME}
            PUBLIC
                ${SNAPCATCH2_INCLUDE_DIRS}
        )

        target_link_libraries(${PROJECT_NAME}
            zipios
            ${SNAPCATCH2_LIBRARIES}
        )

    else(SNAPCATCH2_FOUND)

        message("No test will be created because you do not seem to have catch.hpp installed...")

    endif(SNAPCATCH2_FOUND)

    if(SnapCatch2_FOUND)

        find_package(SnapTestRunner)
        if(SnapTestRunner_FOUND)
            AddUnitTestsTarget(
                PROJECT_NAME
                    rununittests
            )
        endif(SnapTestRunner_FOUND)

    endif(SnapCatch2_FOUND)

else(BUILD_ZIPIOS_TESTS)

    message("Tests are turned OFF. See BUILD_ZIPIOS_TESTS option.")

endif(BUILD_ZIPIOS_TESTS)

# vim: ts=4 sw=4 et
