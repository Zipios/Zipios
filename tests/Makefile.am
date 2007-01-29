
EXTRA_DIST = test.zip file1.txt file2.txt file3.txt testfile.bin

AM_CPPFLAGS = -I$(srcdir)/..
LDADD = ../src/libzipios.la

noinst_PROGRAMS = \
	test_zipinputstreambuf \
	test_zipinputstream \
	test_zip \
	example_zip \
	test_dircoll \
	test_collcoll \
	test_appzip \
	test_zipoutputstreambuf  \
	test_simplesmartptr \
	test_zipoutputstream \
	test_gzipoutputstream \
	all_tests

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
test_gzipoutputstream_SOURCES = test_gzipoutputstream.cpp

all_tests_CXXFLAGS = @CPPUNIT_CFLAGS@
all_tests_LDFLAGS = @CPPUNIT_LIBS@
all_tests_SOURCES = \
	all_tests.cpp \
	zipfiletest.cpp \
	zipfiletest.h \
	zipinputstreamtest.h \
	zipinputstreamtest.cpp \
	zipoutputstreamtest.h \
	zipoutputstreamtest.cpp \
	commontest.h \
	commontest.cpp


check:
	./all_tests
	./test_zip
	./test_simplesmartptr
