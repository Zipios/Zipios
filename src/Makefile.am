
AUTOMAKE_OPTIONS = gnu

# CXXFLAGS= -g

INCLUDES = -I..

lib_LIBRARIES = libzipios.a

noinst_PROGRAMS = appendzip

libzipios_a_SOURCES = backbuffer.h                \
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
                      zipinputstream.cpp          \
                      zipinputstreambuf.cpp


appendzip_SOURCES = appendzip.cpp
