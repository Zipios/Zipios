
AUTOMAKE_OPTIONS = gnu

# CXXFLAGS= -g

INCLUDES = -I..

lib_LIBRARIES = libzipios.a

noinst_PROGRAMS = appendzip

libzipios_a_SOURCES = backbuffer.h               basicentry.h              \
                      basicentry.cpp             fcoll.cpp                 \
                      fcoll.h                    fcoll_common.h            \
                      fcollexceptions.cpp        fcollexceptions.h         \
                      fileentry.cpp              fileentry.h               \
                      filterinputstreambuf.cpp                             \
                      filterinputstreambuf.h     inflateinputstreambuf.cpp \
                      inflateinputstreambuf.h    zipinputstreambuf.cpp     \
                      zipinputstreambuf.h        zipinputstream.cpp        \
                      zipinputstream.h           zipfile.cpp               \
                      zipfile.h                  ziphead.cpp               \
                      ziphead.h                  simplesmartptr.h          \
                      dircoll.h                  dircoll.cpp               \
                      collcoll.h                 collcoll.cpp              \
                      virtualseeker.h            filepath.h                \
                      filepath.cpp                                         \
                      directory.h                directory.cpp             \
                      outputstringstream.h



appendzip_SOURCES = appendzip.cpp
