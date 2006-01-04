/*

$Id$

Copyright (c) 2002-2006, Ross Smith. All Rights Reserved

*/

#ifndef _COMPILER_H
#define _COMPILER_H

#if defined(__cplusplus) || \
	defined(__cplusplus__) || \
	defined(__CPLUSPLUS) || \
	defined(__CPLUSPLUS__)
# define CPLUSPLUS_COMPILER
#endif

#ifdef __BORLANDC__
# define COMPILER "__BORLANDC__"
# define COMPILER_VERSION __BORLANDC__
#endif

#ifdef __CYGWIN__
# define COMPILER "__CYGWIN__"
# define COMPILER_VERSION __CYGWIN__
#endif

#ifdef __DJGPP__
# define COMPILER "__DJGPP__"
# define COMPILER_VERSION __DJGPP__
#endif

#ifdef __DMC__
# define COMPILER "__DMC__"
# define COMPILER_VERSION __DMC__ 
#endif

/* Metaware High C (untested) */
#ifdef __HIGHC__
# define COMPILER "__HIGHC__"
# define COMPILER_VERSION __HIGHC__
#endif

/* IBM C Set/2 (untested) */
#ifdef __IBMC__
# define COMPILER "__IBMC__"
# define COMPILER_VERSION __IBMC__
#endif

#ifdef __INTEL_COMPILER
# define COMPILER "__INTEL_COMPILER"
# define COMPILER_VERSION __INTEL_COMPILER
#endif

#ifdef __LCC__
# define COMPILER "__LCC__"
# define COMPILER_VERSION 0
#endif

#ifdef __MINGW32__
# define COMPILER "__MINGW32__"
# define COMPILER_VERSION __MINGW32__
#endif

#ifndef __INTEL_COMPILER
# ifdef _MSC_FULL_VER
#  define COMPILER "_MSC_FULL_VER"
#  define COMPILER_VERSION _MSC_FULL_VER
# elif defined(_MSC_VER)
#  define COMPILER "_MSC_VER"
#  define COMPILER_VERSION _MSC_VER
# endif
#endif

/* Microsoft Quick C (untested) */
#ifdef _QC
# define COMPILER "_QC"
#  define COMPILER_VERSION _QC
#endif

/* Topspeed (untested) */
#ifdef __TSC__
# define COMPILER "__TSC__"
# define COMPILER_VERSION __TSC__
#endif

/* (untested) */
#ifdef THINK_C 
# define COMPILER "THINK_C"
# define COMPILER_VERSION THINK_C
#endif

/* Borland Turbo C (untested) */
#ifndef __BORLANDC__
#ifdef __TURBOC__
# define COMPILER "__TURBOC__"
# define COMPILER_VERSION __TURBOC__
#endif
#endif

#ifdef __WATCOMC__
# define COMPILER "__WATCOMC__"
# define COMPILER_VERSION __WATCOMC__
#endif

#ifndef COMPILER
# ifdef __GNUC__
#  define COMPILER_PRINTF "%d.%d"
#  define COMPILER "__GCC__"
#  define COMPILER_VERSION __GNUC__, __GNUC_MINOR__
# else
#  define COMPILER "Unknown compiler"
#  define COMPILER_VERSION 0
# endif
#endif

#ifndef COMPILER_PRINTF
# define COMPILER_PRINTF "%d"
#endif

#ifndef __WINDOWS__
# if defined(__BORLANDC__)	|| \
	defined(__DMC__)		|| \
	defined(__LCC__)		|| \
	defined(__MINGW32__)	|| \
	defined(_MSC_VER)		|| \
	defined(__OS2__)		|| \
	defined(__WATCOMC__)
#  define __WINDOWS__ 1
# endif
# ifndef __WINDOWS__
#  if defined(WINDOWS) || \
	defined(WIN32) || \
	defined(_WIN32) || \
	defined(__NT__) || \
	defined(__WIN32__)
#   define __WINDOWS__ 1
#  endif
# endif
#endif /* __WINDOWS__ */


#if defined(__WINDOWS__) || defined(__CYGWIN__)
#define __WINDOWS_OR_CYGWIN__ 1
#endif


#ifndef __DOS__
# if defined(__DJGPP__) || \
    defined(__GO32__) || \
    defined(__MSDOS__) || \
	defined(__WINDOWS__)
#  define __DOS__ 1
# endif
#endif /* __DOS__ */


#ifdef __DOS__
# ifndef HAVE_SETMODE
#  define HAVE_SETMODE 1
# endif
# define IS_PATH_SEPARATOR(c) (((c) == '/') || ((c) == '\\'))
# define IS_CASE_INSENSITIVE_FILESYSTEM 1
#else
# define IS_PATH_SEPARATOR(c) ((c) == '/')
# undef IS_CASE_INSENSITIVE_FILESYSTEM
#endif


#ifdef __BORLANDC__
# define inline __inline
# define HAVE_DIRENT_H 1
# define HAVE_UTIME_H 1
/* Functions containing switch are not expanded inline */
#pragma warn -8027
/* Parameter '...' is never used in function ... */
#pragma warn -8057
#endif /* __BORLANDC__ */


#if defined(__DJGPP__) || defined(__GO32__)
# define HAVE_EIGHT_CHAR_FILENAMES 1
# define HAVE_BASENAME 1
# define HAVE_DIRENT_H 1
# define HAVE_DIRNAME 1
# define HAVE_GETTIMEOFDAY 1
# define HAVE_SSIZE_T 1
# define HAVE_STRCASECMP 1
# define HAVE_SYS_TIME_H
# define HAVE_UTIME_H 1
# define HAVE_UNISTD_H 1
#endif /* __DJGPP__ */


#ifdef __DMC__
# define HAVE_STDBOOL_H 1
# define HAVE_SYS_UTIME_H 1
/*
# include <dos.h>
EXPAND_WILDCARDS;
*/
#endif /* __DMC__ */


#ifdef __LCC__
# define HAVE__BOOL
# define HAVE_GETOPT_H 1
# define HAVE_GETOPT 1
# define HAVE_GETOPT_LONG 1
# define HAVE_SYS_UTIME_H 1
# define utime _utime
/* lcc has getopt.h, but fails to define struct option - emailed jacob@jacob.remcomp.fr */
struct option {
	const char *name;
	int  has_arg;
	int *flag;
	int val;
};
#endif /* __LCC__ */


#ifdef __MINGW32__
# define HAVE_DIRENT_H 1
# define HAVE_GETOPT_H 1
# define HAVE_GETOPT 1
# define HAVE_GETOPT_LONG 1
# define HAVE_SSIZE_T 1
# define HAVE_UTIME_H 1
#endif /* __MINGW32__ */


#ifdef _MSC_VER
# ifndef __cplusplus
#  define inline __inline
# endif
# define HAVE_SYS_UTIME_H 1
#endif /* _MSC_VER */


#ifdef __WATCOMC__
# define HAVE_DIRECT_H 1
# define HAVE_SYS_UTIME_H 1
#endif /* __WATCOMC__ */


#ifndef HAVE_CONFIG_H
# include "version.h"
# define HAVE_CONIO_H 1
# define HAVE_GETCH 1
# define HAVE_FCNTL_H 1
# define HAVE_IO_H 1
# define HAVE_STDLIB_H 1
# define HAVE_STRING_H 1
# define STDC_HEADERS 1
# define strcasecmp stricmp
# ifdef __WINDOWS__
#  define STRICT 1
#  define WIN32_LEAN_AND_MEAN 1
#  define HAVE_WINDOWS_H 1
#  define HAVE_WINSOCK_H 1
#  define HAVE_PROCESS_H 1
# endif
# if defined(HAVE_DIRECT_H) || defined(HAVE_DIRENT_H)
#  define HAVE_OPENDIR 1
# endif
#endif /* ! HAVE_CONFIG_H */

#ifndef longlong_t
#if defined(__CYGWIN__) || \
	(defined(__DJGPP__) && __DJGPP__ >= 2) || \
	defined(__GNUC__) || \
	(defined(__IBMCPP__) && __IBMCPP__ >= 400) || \
	defined(__LCC__) || \
	defined(__MINGW32__) || \
	defined(__WXMICROWIN__)
# define longlong_t long long
#elif defined(_MSC_VER) || \
	(defined(__BORLANDC__) && __BORLANDC__ >= 0x520) || \
	defined(__DIGITALMARS__) || \
	defined(__DMC__) || \
	defined(__RCC__) || \
	defined(__WATCOMC__)
# define longlong_t __int64
#elif defined(__PALMOS__)
# define longlong_t int64_t
#elif defined(__MWERKS__)
# if __option(longlong)
#  define longlong_t long long
# else
#  error "The 64 bit integer support in CodeWarrior has been disabled."
#  error "See the documentation on the 'longlong' pragma."
# endif
#endif
#endif /* longlong_t */

#endif /* _COMPILER_H */
