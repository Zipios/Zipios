
AM_CXXFLAGS = -I$(srcdir)/.. @ZLIB_CFLAGS@

lib_LTLIBRARIES = libzipios.la
libzipios_la_LDFLAGS = @ZLIB_LDFLAGS@
libzipios_la_SOURCES = \
	backbuffer.h \
	basicentry.cpp \
	collcoll.cpp \
	dircoll.cpp \
	directory.cpp \
	directory.h \
	fcoll.cpp \
	fcollexceptions.cpp \
	fileentry.cpp \
	filepath.cpp \
	filterinputstreambuf.cpp \
	inflateinputstreambuf.cpp \
	outputstringstream.h \
	gzipoutputstream.cpp \
	gzipoutputstreambuf.cpp \
	zipfile.cpp \
	ziphead.cpp \
	zipheadio.cpp \
	zipinputstream.cpp \
	zipinputstreambuf.cpp \
	zipios_common.h \
	filteroutputstreambuf.cpp \
	deflateoutputstreambuf.cpp \
	zipoutputstreambuf.cpp \
	zipoutputstream.cpp

noinst_PROGRAMS = appendzip
appendzip_SOURCES = appendzip.cpp