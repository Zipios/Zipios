
AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = test.zip

# CXXFLAGS= -g

INCLUDES = -I..

lib_LIBRARIES = libzipios.a

pkginclude_HEADERS = FlexLexer.h             flist.h                  \
                     backbuffer.h            flistentry.h             \
                     basicentry.h            flistscanner.h           \
                     collcoll.h              inflateinputstreambuf.h  \
                     dircoll.h               simplesmartptr.h         \
                     fcoll.h                 zipfile.h                \
                     fcoll_common.h          ziphead.h                \
                     fcollexceptions.h       zipinputstream.h         \
                     fileentry.h             zipinputstreambuf.h      \
                     filterinputstreambuf.h  virtualseeker.h          \
                     zipios-config.h

noinst_PROGRAMS = test_zipinputstreambuf test_zipinputstream test_zip example_zip \
                  test_dircoll test_collcoll
# test_flist 

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
                      virtualseeker.h            FlexLexer.h               \
                      flist.cpp                  flist.h                   \
                      flistentry.cpp             flistentry.h              \
                      flistscanner.h             flistscanner.ll

test_zipinputstreambuf_SOURCES = test_zipinputstreambuf.cpp

test_zipinputstream_SOURCES = test_zipinputstream.cpp

test_zip_SOURCES = test_zip.cpp

example_zip_SOURCES = example_zip.cpp

test_dircoll_SOURCES = test_dircoll.cpp

test_collcoll_SOURCES = test_collcoll.cpp

# test_flist_SOURCES = test_flist.cpp

# Notice that libzipios.a is not specified as -L. -lzipios
# If it was, automake would not include it as a dependency.

test_zipinputstreambuf_LDADD = libzipios.a -lz

test_zipinputstream_LDADD = libzipios.a -lz

test_zip_LDADD = libzipios.a -lz

example_zip_LDADD = libzipios.a -lz

test_dircoll_LDADD = libzipios.a -lz

test_collcoll_LDADD = libzipios.a -lz

# test_flist_LDADD = libzipios.a


flistscanner.cc : flistscanner.ll
	$(LEX) -+ -PFListScanner -o$@ $^

