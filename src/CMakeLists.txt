project( zipios )

add_library( ${PROJECT_NAME} SHARED
	backbuffer.h
	../zipios++/basicentry.h
	basicentry.cpp
	collcoll.cpp
	dircoll.cpp
	directory.cpp
	directory.h
	dostime.h
	dostime.c
	fcoll.cpp
	../zipios++/fileentry.h
	fileentry.cpp
	filepath.cpp
	filterinputstreambuf.cpp
	inflateinputstreambuf.cpp
	outputstringstream.h
	gzipoutputstream.cpp
	gzipoutputstreambuf.cpp
	zipfile.cpp
	../zipios++/ziphead.h
	ziphead.cpp
	zipheadio.cpp
	zipinputstream.cpp
	zipinputstreambuf.cpp
	zipios_common.h
	filteroutputstreambuf.cpp
	deflateoutputstreambuf.cpp
	zipoutputstreambuf.cpp
	zipoutputstream.cpp
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
    VERSION ${VERSION}
    SOVERSION ${SOVERSION}
)

install( TARGETS ${PROJECT_NAME} DESTINATION lib )

project( appendzip )

add_executable( ${PROJECT_NAME} appendzip.cpp )

install( TARGETS ${PROJECT_NAME} DESTINATION bin )

