
AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = test.zip

# CXXFLAGS= -g

INCLUDES = -I..

lib_LIBRARIES = libzipios.a

pkginclude_HEADERS = \
                     backbuffer.h            \
                     basicentry.h            \
                     collcoll.h              inflateinputstreambuf.h  \
                     dircoll.h               simplesmartptr.h         \
                     fcoll.h                 zipfile.h                \
                     fcoll_common.h          ziphead.h                \
                     fcollexceptions.h       zipinputstream.h         \
                     fileentry.h             zipinputstreambuf.h      \
                     filterinputstreambuf.h  virtualseeker.h          \
                     zipios-config.h

noinst_PROGRAMS = test_zipinputstreambuf test_zipinputstream test_zip example_zip \
                  test_dircoll test_collcoll test_appzip appendzip

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
                      directory.h                directory.cpp             


test_zipinputstreambuf_SOURCES = test_zipinputstreambuf.cpp

test_zipinputstream_SOURCES = test_zipinputstream.cpp

test_zip_SOURCES = test_zip.cpp

example_zip_SOURCES = example_zip.cpp

test_dircoll_SOURCES = test_dircoll.cpp

test_collcoll_SOURCES = test_collcoll.cpp

test_appzip_SOURCES = test_appzip.cpp

appendzip_SOURCES = appendzip.cpp

# Notice that libzipios.a is not specified as -L. -lzipios
# If it was, automake would not include it as a dependency.

test_zipinputstreambuf_LDADD = libzipios.a -lz

test_zipinputstream_LDADD = libzipios.a -lz

test_zip_LDADD = libzipios.a -lz

example_zip_LDADD = libzipios.a -lz

test_dircoll_LDADD = libzipios.a -lz

test_collcoll_LDADD = libzipios.a -lz

test_appzip_LDADD = libzipios.a -lz

appendzip_LDADD = libzipios.a

