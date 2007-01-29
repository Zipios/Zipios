AC_PREREQ(2.59)

AC_INIT([zipios++], [0.1.5], [zipios-devel@lists.sourceforge.net], [zipios++])
AC_CONFIG_SRCDIR([src/appendzip.cpp])
AC_CONFIG_HEADER([zipios++/zipios-config.h])


# Use std compliant iostream library if present?
AC_ARG_WITH(
  [std-compliant-iostream],
  AS_HELP_STRING([--with-std-compliant-iostream], [Include <sstream> instead of <strstream> [[yes]]]),
  [
    if test -n "$withval" -a "$withval" != "no" -a "$withval" != "yes" ; then
      AC_MSG_ERROR([illegal argument specified for --with-std-compliant-iostream. Only 'yes' and 'no' are accepted.])
    fi
  ]
)
if test -z "$with_std_compliant_iostream" -o "$with_std_compliant_iostream" = "yes" ; then
  AC_DEFINE([USE_STD_IOSTREAM], [1], [Define if the std compliant iostream library should be used (if present)])
fi

AM_INIT_AUTOMAKE([gnu])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for libraries.
# Allow alternate location for zlib
AC_ARG_WITH(
  [zlib],
  [AS_HELP_STRING([--with-zlib=DIR], [specify location of zlib])],
  [
    AC_MSG_NOTICE([using `$withval' as alternative zlib location])
    AC_SUBST([ZLIB_CFLAGS], [-I$withval])
    AC_SUBST([ZLIB_LDFLAGS], [-L$withval])
  ]
)
AC_CHECK_LIB([z], [inflate], , AC_MSG_ERROR([zlib is required to compile $PACKAGE.]), [$ZLIB_LDFLAGS])
AC_CHECK_LIB([z], [zError], AC_DEFINE([HAVE_ZERROR], [1], [Define if zlib has zError]), , [$ZLIB_LDFLAGS])

AM_PATH_CPPUNIT(1.6.0)

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([unistd.h])
AC_CXX_HAVE_STL
AC_CXX_HAVE_STD
AC_CXX_HAVE_STD_IOSTREAM

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_STRUCT_TM
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_STAT
AC_CHECK_FUNCS([uname])


AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
  zipios++/Makefile
  zipios++.spec
])

AC_OUTPUT
