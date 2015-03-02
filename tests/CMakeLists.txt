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

find_package( CppUnit )

if(CPPUNIT_FOUND)

project( zipios_tests )

# The tests also include a set of test files:
#          file1.txt    a text file part of test.zip
#          file2.txt    a text file part of test.zip
#          file3.txt    a text file part of test.zip
#          testfile.bin a binary file part of test.zip
#          test.zip     a zip file for test purposes

include_directories(
  SYSTEM
    ${CPPUNIT_INCLUDE_DIR}
)

add_executable( ${PROJECT_NAME}
  all_tests.cpp

  commontest.cpp
  zipfiletest.cpp
  zipinputstreamtest.cpp
  zipoutputstreamtest.cpp

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

else(CPPUNIT_FOUND)
  message("No test will be created because you do not seem to have cppunit installed")
endif(CPPUNIT_FOUND)

# vim: ts=2 sw=2 et
