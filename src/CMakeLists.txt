#
# File:
#      CMakeLists.txt
#
# Description:
#      Build zipios++ library.
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

project( zipios )

add_library( ${PROJECT_NAME} SHARED
    backbuffer.cpp
    collectioncollection.cpp
    deflateoutputstreambuf.cpp
    directorycollection.cpp
    directoryentry.cpp
    dostime.c
    filecollection.cpp
    fileentry.cpp
    filepath.cpp
    filterinputstreambuf.cpp
    filteroutputstreambuf.cpp
    gzipoutputstream.cpp
    gzipoutputstreambuf.cpp
    inflateinputstreambuf.cpp
    virtualseeker.cpp
    zipcentraldirectoryentry.cpp
    zipendofcentraldirectory.cpp
    zipfile.cpp
    zipinputstream.cpp
    zipinputstreambuf.cpp
    zipios_common.cpp
    ziplocalentry.cpp
    zipoutputstream.cpp
    zipoutputstreambuf.cpp
)

target_link_libraries( ${PROJECT_NAME}
    ${ZLIB_LIBRARIES}
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
    VERSION ${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}
    SOVERSION ${ZIPIOS_VERSION_MAJOR}
)

install( TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)


# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et
