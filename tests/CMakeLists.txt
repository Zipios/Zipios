#
# File:
#      CMakeLists.txt
#
# Description:
#      Build zipios++ tests.
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

find_package( Catch )

if(CATCH_FOUND)

project( zipios_tests )

# The tests also include a set of test files:
#          file1.txt    a text file part of test.zip
#          file2.txt    a text file part of test.zip
#          file3.txt    a text file part of test.zip
#          testfile.bin a binary file part of test.zip
#          test.zip     a zip file for test purposes

include_directories(
    ${CATCH_INCLUDE_DIR}
)

add_executable( ${PROJECT_NAME}
    catch_tests.cpp

    catch_backbuffer.cpp
    catch_collectioncollection.cpp
    catch_common.cpp
    catch_directorycollection.cpp
    catch_directoryentry.cpp
    catch_dostime.cpp
    catch_filepath.cpp
    catch_virtualseeker.cpp
    catch_zipfile.cpp

    catch_directory_helper.cpp
    catch_raii_helper.cpp

# commontest.cpp
# zipfiletest.cpp
# zipinputstreamtest.cpp
# zipoutputstreamtest.cpp

# test_appzip.cpp                                        test_zipinputstream.cpp
# test_collcoll.cpp          test_simplesmartptr.cpp     test_zipoutputstreambuf.cpp
# test_dircoll.cpp                                       test_zipoutputstream.cpp
# example_zip.cpp            test_zip.cpp
# test_gzipoutputstream.cpp  test_zipinputstreambuf.cpp
)

target_link_libraries( ${PROJECT_NAME}
    zipios
    ${CPPUNIT_LIBRARIES}
)

add_custom_target(run_zipios_tests
    # You can use the --success command line option to see all the tests
    # as they run; it is a LOT of output though, thus by default we don't
    # use it
    COMMAND ./zipios_tests
)

else(CATCH_FOUND)

message("No test will be created because you do not seem to have catch.hpp installed...")

# For compatibility, just in case
add_custom_target(run_zipios_tests
    COMMAND echo "No tests were built because it looks like you are missing Catch."
)

endif(CATCH_FOUND)

# vim: ts=4 sw=4 et
