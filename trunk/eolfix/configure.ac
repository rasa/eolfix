dnl Process this file with autoconf to produce a configure script.

dnl $Id$

MAJOR_VERSION=0
MINOR_VERSION=2
RELEASE_NUMBER=0
SUBRELEASE_NUMBER=0

AC_INIT(eolfix, 0.2.0, [rossta@users.sf.net])

AC_CONFIG_SRCDIR([eolfix.c])

AC_PREREQ(2.52)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

AC_PROG_CC
dnl AC_PROG_LIBTOOL

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

case "${host}" in
  *-pc-os2* )
    AC_DEFINE([__OS2__], [], [OS/2 Operating System])
	;;
  *-darwin* )
    AC_DEFINE([__OSX__], [], [Mac OS X Operating System])
	;;
esac

dnl Checks for header files.
AC_HEADER_STDC([])
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([\
	dirent.h \
	fcntl.h \
	fnmatch.h \
	getopt.h \
	stdlib.h \
	string.h \
	strings.h \
	sys/file.h \
	sys/stat.h \
	sys/time.h \
	sys/utime.h \
	time.h \
	unistd.h \
	utime.h \
])

AC_C_INLINE 

AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

AC_CHECK_TYPES(ssize_t)
AC_CHECK_TYPES([struct timeval], [], [],
    [#include <sys/time.h>
    #include <time.h>])

AC_TYPE_SIZE_T

AC_CHECK_FUNCS(\
	chown \
	fnmatch \
	getopt \
	getopt_long \
	getpid \
	gettimeofday \
	lstat \
	mkstemps \
	opendir \
)

if test "$ac_cv_func_getopt_long" = "no"; then
	dnl FreeBSD has a gnugetopt library
	AC_CHECK_LIB([gnugetopt], [getopt_long], [HAVE_GETOPT_LONG="yes"])
	LDFLAGS="${LDFLAGS} -L/usr/local/lib"
	for lib in gnugetopt; do
		AC_CHECK_LIB($lib, getopt_long, [AC_DEFINE(HAVE_GETOPT_LONG)
		LIBS="${LIBS} -l$lib"; break])
	done
fi

AM_CONDITIONAL([USE_LOCAL_GETOPT], [test "$ac_cv_func_getopt_long" = "no"])

to_debug=no
AC_ARG_ENABLE(debug, [  --enable-debug          enable debug build],
  [to_debug="$enableval"])
if test "$to_debug" = "yes" ; then
  CFLAGS="${CFLAGS} -O0 -g -D_DEBUG=1"
else
  CFLAGS="${CFLAGS} -O2 -DNDEBUG=1"
fi

dnl AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT([Makefile doc/Makefile])

dnl configure.ac ends here
