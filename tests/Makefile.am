
AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = test.zip

# CXXFLAGS= -g

INCLUDES = -I..

CXXFLAGS += @CPPUNIT_CFLAGS@

noinst_PROGRAMS = test_zipinputstreambuf test_zipinputstream test_zip example_zip \
                  test_dircoll test_collcoll test_appzip test_zipoutputstreambuf  \
                  test_simplesmartptr test_zipoutputstream all_tests

test_zipinputstreambuf_SOURCES = test_zipinputstreambuf.cpp

test_zipinputstream_SOURCES = test_zipinputstream.cpp

test_zip_SOURCES = test_zip.cpp

example_zip_SOURCES = example_zip.cpp

test_dircoll_SOURCES = test_dircoll.cpp

test_collcoll_SOURCES = test_collcoll.cpp

test_appzip_SOURCES = test_appzip.cpp

test_zipoutputstreambuf_SOURCES = test_zipoutputstreambuf.cpp

test_simplesmartptr_SOURCES = test_simplesmartptr.cpp

test_zipoutputstream_SOURCES = test_zipoutputstream.cpp

all_tests_SOURCES = all_tests.cpp zipfiletest.cpp zipfiletest.h \
                    zipoutputstreamtest.h zipoutputstreamtest.cpp

# Notice that libzipios.a is not specified as -L. -lzipios
# If it was, automake would not include it as a dependency.

test_zipinputstreambuf_LDADD = -L../src -lzipios -lz

test_zipinputstream_LDADD = -L../src -lzipios -lz

test_zip_LDADD = -L../src -lzipios -lz

example_zip_LDADD = -L../src -lzipios -lz

test_dircoll_LDADD = -L../src -lzipios

test_collcoll_LDADD = -L../src -lzipios -lz

test_appzip_LDADD = -L../src -lzipios -lz

test_zipoutputstreambuf_LDADD = -L../src -lzipios -lz

test_zipoutputstream_LDADD = -L../src -lzipios -lz

all_tests_LDADD = -L../src -lzipios -lz @CPPUNIT_LIBS@

check:
	./all_tests
	./test_zip
	./test_simplesmartptr
