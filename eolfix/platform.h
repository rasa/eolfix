/*

$Id$

Copyright (c) 2002-2006, Ross Smith. All Rights Reserved

*/

#ifndef	IS_UNIX
# if defined(__NetBSD__)
#  define IS_UNIX 1
# endif
#endif

#ifndef	IS_UNIX
# if (defined(unix) || defined(__unix) || defined(__unix__)) && !defined(__DJGPP__)
#  define IS_UNIX 1
# endif
#endif

/* Atari */
#if defined(tos) || defined(__tos)
# define IS_TOS 1
#endif

#ifdef __BEOS__
# ifndef IS_UNIX
#  define IS_UNIX 1
# endif
#endif

#if defined(__linux__) || defined(__linux)
# ifndef IS_UNIX
#  define IS_UNIX 1
# endif
#endif

#ifdef THINK_C
# define IS_MAC 1
#endif

#if defined(__APPLE__)
# define IS_MAC 1
# if defined(__MACH__)
#  ifndef IS_UNIX
#   define IS_UNIX 1
#  endif
#  define IS_MACOS_X 1
#endif
#endif

#if defined(__CYGWIN32__) || defined(__CYGWIN__)
# define IS_CYGWIN 1
#endif

/* OS/2 EMX */
#ifdef __EMX__
# ifndef IS_UNIX
#  define IS_UNIX 1
# endif
#endif

#if	defined(_IBMR2) || defined(_AIX)
# ifndef IS_UNIX
#  define IS_UNIX 1
# endif
#endif

#if defined(__QNX__)
# ifndef IS_UNIX
#  define IS_UNIX 1
# endif
#endif

#if defined(sun) || defined(__sun) || defined(__sun__)
# define IS_SUN 1
#endif
