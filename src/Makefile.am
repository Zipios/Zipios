
AUTOMAKE_OPTIONS = gnu

# CXXFLAGS= -g

INCLUDES = -I..

lib_LTLIBRARIES = libzipios.la

noinst_PROGRAMS = appendzip

libzipios_la_SOURCES = backbuffer.h                \
                      basicentry.cpp              \
                      collcoll.cpp                \
                      dircoll.cpp                 \
                      directory.cpp               \
                      directory.h                 \
                      fcoll.cpp                   \
                      fcollexceptions.cpp         \
                      fileentry.cpp               \
                      filepath.cpp                \
                      filterinputstreambuf.cpp    \
                      inflateinputstreambuf.cpp   \
                      outputstringstream.h        \
                      zipfile.cpp                 \
                      ziphead.cpp                 \
                      zipheadio.cpp               \
                      zipinputstream.cpp          \
                      zipinputstreambuf.cpp       \
                      zipios_common.h             \
                      filteroutputstreambuf.cpp   \
                      deflateoutputstreambuf.cpp  \
                      zipoutputstreambuf.cpp      \
                      zipoutputstream.cpp
libzipios_la_LIBADD = \
		     -lz

appendzip_SOURCES = appendzip.cpp
