# - Try to find SnapWebsites
#
# Once done this will define
#
# ZIPIOSCC_FOUND        - System has SnapWebsites
# ZIPIOSCC_INCLUDE_DIR  - The SnapWebsites include directories
# ZIPIOSCC_LIBRARY      - The libraries needed to use SnapWebsites (none)
# ZIPIOSCC_DEFINITIONS  - Compiler switches required for using SnapWebsites (none)

find_path( ZIPIOSCC_INCLUDE_DIR zipios++/zipfile.h
		   PATHS /usr/include /usr/local/include $ENV{ZIPIOSCC_INCLUDE_DIR}
		   PATH_SUFFIXES zipios++
		 )
find_library( ZIPIOSCC_LIBRARY libzipios.so
			PATHS /usr/lib /usr/local/lib $ENV{ZIPIOSCC_LIBRARY}
		 )
mark_as_advanced( ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

set( ZIPIOSCC_INCLUDE_DIRS ${ZIPIOSCC_INCLUDE_DIR} )
set( ZIPIOSCC_LIBRARIES    ${ZIPIOSCC_LIBRARY}     )

include( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set ZIPIOSCC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( ZipIos++ DEFAULT_MSG ZIPIOSCC_INCLUDE_DIR ZIPIOSCC_LIBRARY )

