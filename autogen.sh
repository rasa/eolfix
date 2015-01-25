#!/bin/sh

# $Id$

PROG=`basename $0`

KEYFILE=eolfix.c

# Some OS's have multiple versions (autoconf259, etc.) and don't have an 
# autoconf binary

if test -z "$1"
then
	LIBTOOLIZE_NAME=libtoolize
else
	LIBTOOLIZE_NAME=$1
fi

AUTOCONF=`which autoconf`
if test x"${AUTOCONF}" != x -a -f ${AUTOCONF}
then
    AUTOCONF=autoconf
    AUTOMAKE=automake
    ACLOCAL=aclocal
    LIBTOOLIZE=${LIBTOOLIZE_NAME}
    AUTOHEADER=autoheader
else
    FINDPATH=`echo ${PATH}|sed -e 's,:, ,g'` 
    AUTOCONF=`find ${FINDPATH} -name "autoconf*"|sort -r|head -1`
    AUTOMAKE=`find ${FINDPATH} -name "automake*"|sort -r|head -1`
    ACLOCAL=`find ${FINDPATH} -name "aclocal*"|sort -r|head -1`
    LIBTOOLIZE=`find ${FINDPATH} -name "${LIBTOOLIZE_NAME}*"|sort -r|head -1`
    AUTOHEADER=`find /usr/bin /usr/local/bin -name "autoheader*"|sort -r|head -1`
    echo "$0: autoconf: using ${AUTOCONF}"
    echo "$0: automake: using ${AUTOMAKE}"
    echo "$0: aclocal: using ${ACLOCAL}"
    echo "$0: libtoolize: using ${LIBTOOLIZE}"
    echo "$0: autoheader: using ${AUTOHEADER}"
fi

GETTEXTIZE_FLAGS=--no-changelog
AUTOPOINT_FLAGS=
LIBTOOLIZE_FLAGS=

# Some OS's require /usr/local/share/aclocal

if test -d /usr/local/share/aclocal
then
  ACLOCAL_FLAGS='-I /usr/local/share/aclocal'
fi
AUTOHEADER_FLAGS=-Wall
#AUTOMAKE_FLAGS='--add-missing -Wall'
AUTOMAKE_FLAGS=--add-missing
AUTOCONF_FLAGS=-Wno-obsolete

die()
{
    err=$?
    echo "$PROG: exited by previous error(s), return code was $err" >&2
    exit 1
}

if [ ! -f $KEYFILE ]
then
    echo "$PROG: key-file \`$KEYFILE' not found, exiting." >&2
    echo "$PROG: hint: you should run $PROG from top-level sourse directory." >&2
    exit 1
fi

# gettextize ${GETTEXTIZE_FLAGS}
# autopoint ${AUTOPOINT_FLAGS}
# xml-i18n-toolize       || die
# intltoolize            || die
if [ -e ${LIBTOOLIZE} ]
then
	${LIBTOOLIZE} ${LIBTOOLIZE_FLAGS} --force || die
fi
${ACLOCAL} ${ACLOCAL_FLAGS}        || die
${AUTOHEADER} ${AUTOHEADER_FLAGS}  || die
${AUTOMAKE} ${AUTOMAKE_FLAGS}      || die
${AUTOCONF} ${AUTOCONF_FLAGS}      || die
if [ "$OSTYPE" = "cygwin" ]
then
	rm -f cygwin.debug
	# Cygwin bug fix, as Cygwin's files are dos files by default :(
	for file in \
		config.guess \
		config.h.in \
		config.sub \
		depcomp \
		install-sh \
		missing
	do
		sed -e "s/\r\n/\n/" $file >$file.tmp || die
		touch -r $file $file.tmp || die
		mv -f $file.tmp $file || die
	done
fi
