#
# File:
#      CMakeLists.txt
#
# Description:
#      Build Zipios tools.
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


###
### ZipIOS Tool
###
project( zipios_tool )

add_executable( ${PROJECT_NAME}
    zipios.cpp
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME zipios
)

target_link_libraries( ${PROJECT_NAME}
    zipios
)

install( TARGETS ${PROJECT_NAME}
    DESTINATION ${BIN_INSTALL_DIR}
)


###
### Append Zip Tool
###
project( appendzip )

add_executable( ${PROJECT_NAME}
    appendzip.cpp
)

install( TARGETS ${PROJECT_NAME}
    DESTINATION ${BIN_INSTALL_DIR}
)



###
### DOS Date & Time Tool
###
project( dosdatetime )

add_executable( ${PROJECT_NAME}
    dosdatetime.cpp
)

target_link_libraries( ${PROJECT_NAME}
    zipios
)

install( TARGETS ${PROJECT_NAME}
    DESTINATION ${BIN_INSTALL_DIR}
)


###
### zipios_example.cpp
###
project( zipios_example )

add_executable( ${PROJECT_NAME}
    zipios_example.cpp
)

target_link_libraries( ${PROJECT_NAME}
    zipios
)

# DO NOT INSTALL THIS ONE, IT IS JUST AN EXAMPLE!


# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et
