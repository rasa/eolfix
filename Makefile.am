## Process this file with automake to produce Makefile.in

bin_PROGRAMS = eolfix
eolfix_SOURCES = eolfix.c compiler.h version.h

if USE_LOCAL_GETOPT
eolfix_SOURCES += getopt.c getopt.h
endif

#SUBDIRS = doc

#DIST_SUBDIRS = doc

EXTRA_DIST = autogen.sh README.md LICENSE

## Makefile.am ends here
