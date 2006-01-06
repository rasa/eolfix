/*

$Id$

Copyright (c) 2002-2006, Ross Smith. All Rights Reserved

TODO
====

http://packages.debian.org/stable/text/flip

flip
This program converts line endings of text files between MS-DOS and **IX formats.
It detects binary files in a nearly foolproof way and leaves them alone unless you override this.
It will also leave files alone that are already in the right format and preserves file timestamps.
User interrupts are handled gracefully and no garbage or corrupted files left behind.
'flip' does not convert files to a different character set, and it can not handle Apple Macintosh line endings (CR only). For that (and more), you can use the 'recode' program (package 'recode').

fix: eolfix -v -s *.c -l *.c

support runon options: eolfix -vib -d

support comments in @files

only process files in @files

one filename per line?

deal with embedded spaces in @file files

documentation

add asserts()

implement -o directory_name ?

	"-o | --output filespec Send output to a file/dir named 'filespec' (-=stdout)\n"

implement reading options from EOLFIX environment variable

implement reading options from ~/.eolfix settings file


remove debugging code

move basename/dirname/mkstemp/gettimeofday to separate files ?

use ruby code to add windows CE port

todo:

	only support conv compatibility, if program is named conv, or the user uses -C or --conv option

	how to implement the opposite of an option if the user uses the capital of it:

test symlink code

research how cp deals with permissions

dynamically assign BUFSIZ from statbuf.st_blksize?

if windows, copy windows rights specially

-c | --compress        Compress doubled-spaced files to be single-spaced

flush out license() contents

support reading unicode files?

shr	long						sht	long
--- ---------------------------	---	-----------------
-a	--Abort						-A	--no-abort
-b	--Backup					-B	--no-backup
-c  --Compress					-C	--no-compress
-d	--Dos
-e:	--Exclude
-f	--Force						-F	--no-force
-g: --confiG
-h	--Help (undocumented)
-i:	--Include
-j	--in-place					-J	--no-in-place
-k:	--bacKup-ext				-K	--no-backup-ext
-l	--Leave
-m	--Mac
-n	--Native
-o	--Output					-O	--no-output
-p	--Preserve					-P	--no-preserve
-q	--Quiet						-Q	--no-quiet
-r	--Recursive					-R	--no-recursive
-s	--Skip
-t:	--Temp-dir					-T	--no-temp-dir
-u	--Unix
-v	--Verbose					-V	--no-verbose
-w	--Windows (undocumented)
-x (unused)
-y	--drY-run					-Y	--no-dry-run
-z  --ignore-case               -Z  --no-ignore-case
-?	--Help

    --license
	--version

conv_mode:

-A	--auto (conv_mode)				--no-auto
-D	--dos (conv_mode)
-F	--force (conv_mode)				--no-force
-M	--mac (conv_mode)
-U	--unix (conv_mode)
-S	--safe (conv_mode)

input	--unix	--dos			--mac
======= ======= ============	=====
LF		LF		CR LF			CR
CR		LF		CR LF			CR
CRLF	LF		CR LF			CR
CRCR	LF LF	CR LF CR LF		CR CR
CRCRLF	LF		CR LF			CR
*/

#undef CONV_SUPPORT_ENABLED

#define COPYRIGHT "Copyright (c) 2002-2006, Ross Smith. All rights reserved."

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "compiler.h"

#include <stdio.h>		/* fprintf() */
#include <assert.h>		/* assert() */
#include <ctype.h>		/* iscntrl() */
#include <errno.h>		/* errno() */
#include <limits.h>		/* PATH_MAX */
#include <signal.h>		/* signal() */
#include <stdarg.h>		/* vfprintf() */
#include <sys/stat.h>	/* stat() */
#include <sys/types.h>	/* stat() */

#ifdef HAVE_CONIO_H
# include <conio.h> /* getch() */
#endif

#ifdef HAVE_DIRECT_H
# include <direct.h> /* opendir() (on Watcom) */
#endif

#ifdef HAVE_DIRENT_H
# include <dirent.h> /* opendir() */
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h> /* O_RDWR, etc. */
#endif

#ifdef HAVE_FNMATCH_H
# include <fnmatch.h> /* fnmatch() */
#else
# include "fnmatch.h" /* local version */
#endif

#ifdef HAVE_GETOPT_LONG
# include <getopt.h> /* getopt() */
#else
# include "getopt.h" /* local version */
#endif

#ifdef HAVE_IO_H
# include <io.h> /* open() */
#endif

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#   else
typedef unsigned char _Bool;
#  endif
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h> /* NULL, EXIT_SUCCESS */
#else
extern void exit();
#endif

#ifdef HAVE_STRING_H
# include <string.h> /* strlen */
#endif

#ifdef HAVE_STRINGS_H
# include <strings.h> /* strlen */
#endif

#ifdef HAVE_SYS_FILE_H
# include <sys/file.h> /* O_RDWR, etc. */
#endif

#ifdef HAVE_SYS_TIME_H
# include <sys/time.h> /* struct timeval */
#endif

#ifdef HAVE_SYS_UTIME_H
# include <sys/utime.h> /* utime() */
#endif

#ifdef HAVE_TIME_H
# include <time.h> /* struct timeval */
#endif

#ifdef HAVE_UTIME_H
# include <utime.h> /* utime() */
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h> /* open() */
#endif

#ifdef HAVE_WINDOWS_H
# include <windows.h> /* GetSystemTimeAsFileTime(), OutputDebugString() */
#endif

#ifndef longlong_t
#if defined(SIZEOF_LONG) && (SIZEOF_LONG >= 8)
# define longlong_t long
#elif (defined(SIZEOF_LONG_LONG) && (SIZEOF_LONG_LONG >= 8)) || defined(__GNUC__)
# define longlong_t long long
#else
# define longlong_t long
#endif
#endif /* longlong_t */

#define ulonglong_t unsigned longlong_t

#ifndef BUFSIZ
# define BUFSIZ 512
#endif

#ifndef O_BINARY
# define O_BINARY 0
#endif

#ifndef PATH_MAX
# ifdef _MAX_PATH
#  define PATH_MAX _MAX_PATH
# else
#  ifdef FILENAME_MAX
#   define PATH_MAX FILENAME_MAX
#  else
#   define PATH_MAX 260
#  endif
# endif
#endif

#ifndef HAVE_SSIZE_T
# define ssize_t int
#endif

#ifndef S_IRUSR
# ifdef S_IREAD
#  define S_IRUSR S_IREAD
# endif
#endif

#ifndef S_IWUSR
# ifdef S_IWRITE
#  define S_IWUSR S_IWRITE
# endif
#endif

/* unix EOF marker */
#define CTRL_D '\x04'

/* Line feed character */
#define CTRL_J '\x0a'

/* Carriage return character */
#define CTRL_M '\x0d'

/* dos EOF marker */
#define CTRL_Z '\x1a'

#define EOLFIX_ENVVAR "EOLFIX"

#define TEMP_FILE_TEMPLATE "/eoXXXXXXX.tmp"

#define TEMP_FILE_TEMPLATE_SUFFIX_LEN 4

#define UNIMPLEMENTED_OPTION "Unimplemented option"
#define PROCESSING_OPTION "Processing option"

static char *progname;

/* input file format */

enum input_e {
	INPUT_NONE		= 0,
	INPUT_UNIX		= 1,
	INPUT_DOS		= 2,
	INPUT_MAC		= 4,
	INPUT_VARIED	= 8,
	INPUT_BINARY	= 16
};

typedef enum input_e input_t;

#define INPUT_ALL_ASCII ((input_t) (INPUT_UNIX | INPUT_DOS | INPUT_MAC | INPUT_VARIED))
#define INPUT_ALL ((input_t) (INPUT_ALL_ASCII | INPUT_BINARY))

/* output file format */

enum output_e {
	OUTPUT_LEAVE	= 0,
	OUTPUT_UNIX		= 1,
	OUTPUT_DOS		= 2,
	OUTPUT_MAC		= 4,
	OUTPUT_SKIP		= 32
};

typedef enum output_e output_t;

#ifdef __DOS__
# define NATIVE "DOS/Windows"
# define OUTPUT_NATIVE OUTPUT_DOS
#elif defined(__MAC__)
/* how to determine? CodeWarrior compiler? */
# define NATIVE "Macintosh"
# define OUTPUT_NATIVE OUTPUT_MAC
#else
# define NATIVE "Unix"
# define OUTPUT_NATIVE OUTPUT_UNIX
#endif

#define T_UNCHANGED	"unchanged"
#define T_UNIX		"unix"
#define T_DOS		"dos"
#define T_MAC		"mac"
#define T_VARIED	"varied"
#define T_NONE		"none"
#define T_BINARY	"binary"
#define T_ALL		"all"

static char *input_desc[] = {
	T_NONE,			/* no cr/lf's found */
	T_UNIX,			/* unix */
	T_DOS,			/* dos */
	T_VARIED,		/* dos & unix */
	T_MAC,			/* mac */
	T_VARIED,		/* mac & unix */
	T_VARIED,		/* mac & dos */
	T_VARIED		/* mac & dos & unix */
};

static char *input_descv[] = {
	T_NONE,						/* no cr/lf's found */
	T_UNIX,						/* unix */
	T_DOS,						/* dos */
	T_DOS "/" T_UNIX,			/* dos & unix */
	T_MAC,						/* mac */
	T_MAC "/" T_UNIX,			/* mac & unix */
	T_DOS "/" T_MAC,			/* mac & dos */
	T_DOS "/" T_MAC "/" T_UNIX	/* mac & dos & unix */
};

static char *output_desc[] = {
	T_UNCHANGED,	/* OUTPUT_LEAVE (0) */
	T_UNIX,			/* OUTPUT_UNIX (1) */
	T_DOS,			/* OUTPUT_DOS (2) */
	NULL,			/* not used */
	T_MAC			/* OUTPUT_MAC (4) */
};

struct _options {
	bool	abort;
	bool	backup;
	char	bak_ext[PATH_MAX];
	bool	compress;
	bool	dry_run;
	bool	force;
	int		include; /* input_t */
	bool	in_place;
	char	output_filename[PATH_MAX];
	output_t output_format;
	bool	preserve;
	bool	quiet;
	bool	recursive;
	char	temp_dir[PATH_MAX];
	int		verbose;
	bool	ignore_case;
	
	bool	std_in;
	bool	std_out;
	bool	wildcards_used;
#ifdef CONV_SUPPORT_ENABLED
	bool	conv_mode;
	bool	_auto;
#endif /* CONV_SUPPORT_ENABLED */
};

typedef struct _options options_t;

#define BACKUP_EXT ".bak"

static options_t opt = {
	false,				/* abort */
	false,				/* backup */
	BACKUP_EXT,			/* backup-ext */
	false,				/* compress */
	false,				/* dry_run */
	false,				/* force */
	INPUT_ALL_ASCII,	/* include */
	false,				/* in_place */
	"",					/* output_filename */
	OUTPUT_LEAVE,		/* output_format */
	false,				/* preserve */
	false,				/* quiet */
	false,				/* recursive */
	"",					/* temp_dir */
	0,					/* verbose */
#ifdef IS_CASE_INSENSITIVE_FILESYSTEM		
	true,				/* ignore_case */
#else
	false,
#endif
	false,				/* stdin */
	false,				/* stdout */
	false,				/* wildcard_found */
#ifdef CONV_SUPPORT_ENABLED
	,false,				/* _auto */
	false				/* conv_mode */
#endif /* CONV_SUPPORT_ENABLED */
};

static char *input_filename;
static char temp_filename[PATH_MAX] = "";

static int output_handle = -1;

static int exit_value = 0;

#ifdef CONV_SUPPORT_ENABLED
# define CONV_MODE_SWITCHES "DMU"
# define FORCE_OPTION		"force-rewrite"
# define FORCEOPTION		"forcerewrite"
# define NO_FORCE_OPTION	"no-force-rewrite"
# define NOFORCEOPTION		"noforcerewrite"
#else
# define CONV_MODE_SWITCHES
# define FORCE_OPTION		"force"
# define FORCEOPTION		"force"
# define NO_FORCE_OPTION	"no-force"
# define NOFORCEOPTION		"noforce"
#endif /* CONV_SUPPORT_ENABLED */

#define UNDOCUMENTED_SWITCHES "wRh"

#define NEGATE_SWITCHES "ABCFJKOPQRTVYZ"

static char* short_options = "abcde:fg:i:jk:lmno:pqrst:uvyz?-" NEGATE_SWITCHES UNDOCUMENTED_SWITCHES CONV_MODE_SWITCHES;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with UCHAR_MAX + 1.  */
enum opt_e {
	OPT_LICENSE = UCHAR_MAX + 1,
	OPT_VERSION
#ifdef CONV_SUPPORT_ENABLED
	,OPT_AUTO,
	OPT_FORCE,
	OPT_SAFE,
	OPT_NO_AUTO,
	OPT_NO_FORCE
#endif /* CONV_SUPPORT_ENABLED */
};

static struct option long_options[] = {
	{"abort",			no_argument,		0, 'a'},
	{"backup",			no_argument,		0, 'b'},
	{"backup-ext",		required_argument,	0, 'k'},
	{"compress",		no_argument,		0, 'c'},
	{"config",			required_argument,	0, 'g'},
	{"dos",				no_argument,		0, 'd'},
	{"dry-run",			no_argument,		0, 'y'},
	{"exclude",			required_argument,	0, 'e'},
	{FORCE_OPTION,		no_argument,		0, 'f'},
	{"help",			no_argument,		0, '?'},
	{"ignore-case",		no_argument,		0, 'z'},
	{"include",			required_argument,	0, 'i'},
	{"in-place",		no_argument,		0, 'j'},
	{"leave",			no_argument,		0, 'l'},
	{"license",			no_argument,		0, OPT_LICENSE},
	{"mac",				no_argument,		0, 'm'},
	{"native",			no_argument,		0, 'n'},
	{"output",			required_argument,	0, 'o'},
	{"preserve",		no_argument,		0, 'p'},
	{"quiet",			no_argument,		0, 'q'},
	{"recursive",		no_argument,		0, 'r'},
	{"skip",			no_argument,		0, 's'},
	{"temp-dir",		required_argument,	0, 't'},
	{"unix",			no_argument,		0, 'u'},
	{"verbose",			no_argument,		0, 'v'},
	{"version",			no_argument,		0, OPT_VERSION},

	/* negate options */
	{"no-abort",		no_argument,		0, 'A'},
	{"no-backup",		no_argument,		0, 'B'},
	{"no-backup-ext",	no_argument,		0, 'K'},
	{"no-compress",		no_argument,		0, 'C'},
	{"no-dry-run",		no_argument,		0, 'Y'},
	{NO_FORCE_OPTION,	no_argument,		0, 'F'},
	{"no-ignore-case",	no_argument,		0, 'Z'},
	{"no-in-place",		no_argument,		0, 'J'},
	{"no-output",		no_argument,		0, 'O'},
	{"no-preserve",		no_argument,		0, 'P'},
	{"no-quiet",		no_argument,		0, 'Q'},
	{"no-recursive",	no_argument,		0, 'R'},
	{"no-temp-dir",		no_argument,		0, 'T'},
	{"no-tmp-dir",		no_argument,		0, 'T'},
	{"no-verbose",		no_argument,		0, 'V'},

	/* undocumented alternatives */

	/* abort */
	{"abort-on-error",	no_argument,		0, 'a'},

	/* backup-ext */
	{"backupext",		required_argument,	0, 'k'},

	/* dos */
	{"crlf",			no_argument,		0, 'd'},
	{"msdos",			no_argument,		0, 'd'},
	{"u2d",				no_argument,		0, 'd'},
	{"unix2dos",		no_argument,		0, 'd'},
	{"m2d",				no_argument,		0, 'd'},
	{"mac2dos",			no_argument,		0, 'd'},
	{"win",				no_argument,		0, 'd'},
	{"windows",			no_argument,		0, 'd'},

	/* dry-run */
	{"dryrun",			no_argument,		0, 'y'},

	/* force */
	{FORCEOPTION,		no_argument,		0, 'f'},

	/* help */
	{"help",			no_argument,		0, '?'},
	{"usage",			no_argument,		0, '?'},

	/* ignore-case */
	{"ignorecase",		no_argument,		0, 'Z'},

	/* in-place */
	{"inplace",			no_argument,		0, 'j'},

	/* leave */
	{"leave-alone",		no_argument,		0, 'l'},

	/* license */
	{"licence",			no_argument,		0, OPT_LICENSE},

	/* mac */
	{"cr",				no_argument,		0, 'm'},
	{"macintosh",		no_argument,		0, 'm'},
	{"d2m",				no_argument,		0, 'u'},
	{"dos2mac",			no_argument,		0, 'u'},
	{"u2m",				no_argument,		0, 'u'},
	{"unix2mac",		no_argument,		0, 'u'},

	/* no-change */
	{"nochange",		no_argument,		0, 'n'},

	/* recursive */
	{"recurse",			no_argument,		0, 'r'},

	/* temp-dir */
	{"tempdir",			required_argument,	0, 't'},
	{"tmp-dir",			required_argument,	0, 't'},
	{"tmpdir",			required_argument,	0, 't'},

	/* unix */
	{"lf",				no_argument,		0, 'u'},
	{"nl",				no_argument,		0, 'u'},
	{"d2u",				no_argument,		0, 'u'},
	{"dos2unix",		no_argument,		0, 'u'},
	{"m2u",				no_argument,		0, 'u'},
	{"mac2unix",		no_argument,		0, 'u'},

	/* negate options */
	{"noabort",			no_argument,		0, 'A'},
	{"nobackup",		no_argument,		0, 'B'},
	{"nobackupext",		no_argument,		0, 'K'},
	{"nocaseinsensitive",no_argument,		0, 'Z'},
	{"nodryrun",		no_argument,		0, 'Y'},
	{NOFORCEOPTION,		no_argument,		0, 'F'},
	{"noignorecase",	no_argument,		0, 'Z'},
	{"noinplace",		no_argument,		0, 'J'},
	{"nooutput",		no_argument,		0, 'O'},
	{"nopreserve",		no_argument,		0, 'P'},
	{"noquiet",			no_argument,		0, 'Q'},
	{"norecursive",		no_argument,		0, 'R'},
	{"notempdir",		no_argument,		0, 'T'},
	{"notmpdir",		no_argument,		0, 'T'},
	{"noverbose",		no_argument,		0, 'V'}
#ifdef CONV_SUPPORT_ENABLED
   ,{"auto",			no_argument,		0, OPT_AUTO},
	{"dos",				no_argument,		0, 'D'},
	{"force",			no_argument,		0, OPT_FORCE},
	{"mac",				no_argument,		0, 'M'},
	{"safe",			no_argument,		0, OPT_SAFE},
	{"unix",			no_argument,		0, 'U'},
    {"no-auto",			no_argument,		0, OPT_NO_AUTO},
	{"no-force",		no_argument,		0, OPT_NO_FORCE},

    {"noauto",			no_argument,		0, OPT_NO_AUTO},
	{"noforce",			no_argument,		0, OPT_NO_FORCE}
#endif /* CONV_SUPPORT_ENABLED */
};

#ifdef CONV_SUPPORT_ENABLED
#  define CONV_USAGE \
	"-A | --auto             Convert to UNIX if DOS, Mac, or varied, or DOS if UNIX\n" \
	"-D | --u2d      [files] Convert text files to have DOS/Windows line endings\n" \
	"     --unix2dos [files] Convert text files to have DOS/Windows line endings\n" \
	"-U | --d2u      [files] Convert text files to have UNIX line endings\n" \
	"     --dos2unix [files] Convert text files to have UNIX line endings\n" \
	"     --force            Force conversion of binary files\n" \
	"     --safe             Do not convert binary files (default)\n"
#else
# define CONV_USAGE
#endif /* CONV_SUPPORT_ENABLED */

/****************************************************************************/

static
#if !defined(_DEBUG) && !defined(__LCC__)
/* Workaround for LCC's: Error eolfix.c: 2168  operands of = have illegal types 'pointer to char' and 'int' */
inline
#endif
void _DBG(char *fmt, ...) {
#ifdef _DEBUG
	static char buffer[65535];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	strcat(buffer, "\n");
# if defined(__WINDOWS__)
	OutputDebugString(buffer);
# endif
	fprintf(stdout, buffer);
	fflush(stdout);
	va_end(args);
#endif
}

/****************************************************************************/

static void usage(int exit_value) {
	printf(
"Usage: %s [options] [files...]\n"
"Options:\n",
		progname
	);
#ifdef CONV_SUPPORT_ENABLED
	if (opt.conv_mode) {
		printf(CONV_USAGE);
		printf("\n");
	}
#endif /* CONV_SUPPORT_ENABLED */
/*2345678901234567890123456789012345678901234567890123456789012345678901234567890*/
	printf(
"-u | --unix    [files] Convert text files to have UNIX line endings\n"
"-d | --dos     [files] Convert text files to have DOS/Windows line endings\n"
"-m | --mac     [files] Convert text files to have Macintosh line endings\n"
"-n | --native  [files] Convert text files to have %s line endings\n"
"-l | --leave   [files] Report on the type of file, but do not convert (default)\n"
"-s | --skip    [files] Skip these files entirely\n"
"-i | --include type    Include types: unix, dos, mac, varied, binary and all\n"
"                       Default is to include unix, dos, mac and varied files\n"
"-e | --exclude type    Exclude types: unix, dos, mac, varied, binary and all\n"

"-a | --abort           Abort processing on the first error encountered\n"
"-b | --backup          Backup each file before converting it\n"
"-k | --backup-ext .ext Use <.ext> for backup file extension (default is %s)\n"
"-c | --compress        Compress doubled-spaced files to be single-spaced (todo)\n"
"-g | --config file     Process options in <file>\n"
"-y | --dry-run         Skip conversion, but show what would have been converted\n"
"-f | --%-15s Convert every file, even if no change would result\n"
"-z | --ignore-case     Perform case-insensitive filename matching\n"
"-j | --in-place        Convert text files in-place (slower)\n"
"-o | --output file     Send output to <file> (use - for stdout)\n"
"-p | --preserve        Preserve file ownership and times\n"
"-r | --recursive       Recurse into directories\n"
"-t | --temp-dir dir    Use <dir> for temporary files (default is file's dir)\n"
, NATIVE, opt.bak_ext, FORCE_OPTION);
#ifdef HAVE_GETCH
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("Press any key: ");
		while (!getch())
			;
		printf("\r");
	}
#endif
	printf(
"-v | --verbose         Report on every file, even if it would not be changed\n"
"-q | --quiet           Only report errors\n"
"-? | --help            Show this help message and quit\n"
"     --version         Display version information and quit\n"
"     --license         Display licensing information\n"
"     --                Stop processing options\n"
);
#ifdef CONV_SUPPORT_ENABLED
	if (!opt.conv_mode) {
		printf("\n");
		printf(CONV_USAGE);
	}
#endif /* CONV_SUPPORT_ENABLED */

	exit(exit_value);
}

/****************************************************************************/
#ifndef HAVE_BASENAME
/****************************************************************************/

/* per http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+basename */
static char* basename(char* s) {
	char* rv;

	if (!s || !*s)
		return ".";

	rv = s + strlen(s) - 1;

	do {
		if (IS_PATH_SEPARATOR(*rv))
			return rv + 1;
		--rv;
	} while (rv >= s);

	return s;
}

#define HAVE_BASENAME 1

#endif /* ! HAVE_BASENAME */


/****************************************************************************/
#ifndef HAVE_DIRNAME
/****************************************************************************/

/* per http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+dirname */
static char* dirname(char* s) {
	int i;
	static char rv[PATH_MAX];
	int at_end = 1;

	if (s) {
		for (i = strlen(s) - 1; i >= 0; --i) {
			if (IS_PATH_SEPARATOR(s[i])) {
				if (!at_end) {
					if (i > 0) {
						if (i > PATH_MAX - 1)
							return NULL;
						strncpy(rv, s, i - 1);
					}
					rv[i] = '\0';
					return rv;
				}
			} else
				at_end = 0;
		}
	}

	return ".";
}

#define HAVE_DIRNAME 1

#endif /* ! HAVE_DIRNAME */


/****************************************************************************/
#ifndef HAVE_GETTIMEOFDAY
/****************************************************************************/

#if !defined(HAVE_STRUCT_TIMEVAL) && defined(HAVE_WINSOCK_H) && !defined(__CYGWIN__)
# include <winsock.h> /* struct timeval */
#endif

#define MAKE_ULONGLONG(hi, lo) ((((ulonglong_t) hi) << 32) + ((ulonglong_t) lo))

static int gettimeofday(struct timeval* tv, void* timezone) {
/*
	FILETIME time;
	double timed;

	GetSystemTimeAsFileTime(&time);
	timed = ((time.dwHighDateTime * 4294967296e-7) - 11644473600.0) + (time.dwLowDateTime  * 1e-7);
	tv->tv_sec  = (long) timed;
	tv->tv_usec = (long) ((timed - tv->tv_sec) * 1e6);
*/
    SYSTEMTIME nowst;
    FILETIME nowft;
    static ulonglong_t then = 0;
    ulonglong_t now;
    ulonglong_t diff;

    if (!then) {
	    SYSTEMTIME thenst = {1970, 1, 4, 1, 0, 0, 0, 0};  /* 00:00:00 Jan 1st 1970 */
	    FILETIME thenft;

    	SystemTimeToFileTime(&thenst, &thenft);
    	then = MAKE_ULONGLONG(thenft.dwHighDateTime, thenft.dwLowDateTime);
	}

    GetLocalTime(&nowst);
    SystemTimeToFileTime(&nowst, &nowft);
    now = MAKE_ULONGLONG(nowft.dwHighDateTime, nowft.dwLowDateTime);

	diff = now - then; /* time from 00:00:00 Jan 1st 1970 to now in 100 nanos */

	/* 100 nanos / 10 = 1 micro / 1000 = 1 milli / 1000 = 1 second */
    tv->tv_sec = (long) (diff / 10000000);
    diff %= 10000000;
	tv->tv_usec = (long) (diff * 100);

	return 0;
}

# define HAVE_GETTIMEOFDAY 1

#endif /* ! HAVE_GETTIMEOFDAY */


/****************************************************************************/
#ifndef HAVE_MKSTEMPS
/****************************************************************************/

#ifdef HAVE_PROCESS_H
# include <process.h> /* getpid() */
#endif /* HAVE_PROCESS_H */

#ifndef TMP_MAX
# define TMP_MAX 32767
#endif

static int mkstemps (char *templat, int suffix_len) {
	static const char letters[] =
#ifndef IS_CASE_INSENSITIVE_FILESYSTEM
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#endif
		"abcdefghijklmnopqrstuvwxyz0123456789";
	static ulonglong_t value;
#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
#endif
	char *XXXXXX;
	size_t len;
	int count;

	len = strlen(templat);

	if ((int) len < 6 + suffix_len || strncmp(&templat[len - 6 - suffix_len], "XXXXXX", 6))
		return -1;

	XXXXXX = &templat[len - 6 - suffix_len];

#ifdef HAVE_GETTIMEOFDAY
	gettimeofday(&tv, NULL);
	value += ((ulonglong_t) tv.tv_usec << 16) ^ tv.tv_sec ^ getpid();
#else
	value += getpid();
#endif

	for (count = 0; count < TMP_MAX; ++count) {
		ulonglong_t v = value;
		int fd;

		/* Fill in the random bits.  */
		XXXXXX[0] = letters[(unsigned int) (v % sizeof(letters))];
		v /= sizeof(letters);
		XXXXXX[1] = letters[(unsigned int) (v % sizeof(letters))];
		v /= sizeof(letters);
		XXXXXX[2] = letters[(unsigned int) (v % sizeof(letters))];
		v /= sizeof(letters);
		XXXXXX[3] = letters[(unsigned int) (v % sizeof(letters))];
		v /= sizeof(letters);
		XXXXXX[4] = letters[(unsigned int) (v % sizeof(letters))];
		v /= sizeof(letters);
		XXXXXX[5] = letters[(unsigned int) (v % sizeof(letters))];

		fd = open(templat, O_RDWR | O_CREAT | O_EXCL | O_BINARY, 0600);
		if (fd >= 0)
			return fd;

		value += 7777;
	}

	templat[0] = '\0';
	return -1;
}

#define HAVE_MKSTEMPS 1

#endif /* ! HAVE MKSTEMPS */


/****************************************************************************/
#ifndef HAVE_OPENDIR
/****************************************************************************/

struct dirent {
	char *d_name;
};

struct _dir {
	long				findfirst_handle;
	struct _finddata_t	*finddata;
	struct dirent		*direntry;
	struct dirent		*firstentry;
};

typedef struct _dir DIR;

static void dh_free(DIR *dh) {
	free(dh->finddata);
	free(dh->direntry);
	free(dh);
	dh = NULL;
}

/* http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+opendir */
static DIR *opendir(const char *dir) {
	char path[PATH_MAX];
	DIR *dh;
	struct stat statbuf;

	if (!dir) {
		errno = EFAULT;
		return (DIR *) NULL;
	}

	if (!*dir) {
		errno = ENOTDIR;
		return (DIR *) NULL;
	}

	if (stat(dir, &statbuf) == -1)
		return (DIR *) NULL;

	if (!(statbuf.st_mode & S_IFDIR)) {
		errno = ENOTDIR;
		return (DIR *) NULL;
	}

	dh = (DIR *) malloc(sizeof(DIR));
	if (!dh) {
		errno = ENOMEM;
		return (DIR *) NULL;
	}

	dh->finddata = (struct _finddata_t*) malloc(sizeof(struct _finddata_t));
	if (!dh->finddata) {
		free(dh);
		errno = ENOMEM;
		return (DIR *) NULL;
	}

	dh->direntry = (struct dirent*) malloc(sizeof(struct dirent));
	if (!dh->direntry) {
		free(dh);
		free(dh->finddata);
		errno = ENOMEM;
		return (DIR *) NULL;
	}

	if (strlen(dir) + 2 > sizeof(path) - 1) {
		dh_free(dh);
		errno = ENAMETOOLONG;
		return (DIR *) NULL;
	}

	sprintf(path, "%s/%s", dir, "*");

	dh->findfirst_handle = _findfirst(path, dh->finddata);
	if (dh->findfirst_handle == -1) {
		errno = ENOENT;
		dh_free(dh);
		return (DIR *) NULL;
	}

	dh->firstentry = dh->direntry;
	dh->direntry->d_name = dh->finddata->name;

	return dh;
}

/* http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+readdir */
static struct dirent *readdir(DIR *dh) {
	struct dirent *de;

	if (!dh) {
		errno = EBADF;
		return NULL;
	}

	if (dh->firstentry) {
		de = dh->firstentry;
		dh->firstentry = NULL;
		return de;
	}

	if (_findnext(dh->findfirst_handle, dh->finddata) == -1) {
		if (errno == ENOENT)
			errno = 0;
		return NULL;
	}

	de = dh->direntry;
	de->d_name = dh->finddata->name;

	return de;
}

/* http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+closedir */
static int closedir(DIR *dh) {
	int rv;

	if (!dh) {
		errno = EBADF;
		return -1;
	}

	rv = _findclose(dh->findfirst_handle);

	dh_free(dh);

	return rv;
}

#endif /* HAVE_OPENDIR */

/****************************************************************************/

static void version(void) {
	printf("%s - Version %s - %s", progname, VERSION, __DATE__);
#ifdef _DEBUG
	printf(" (debug build: %s=" COMPILER_PRINTF ")", COMPILER, COMPILER_VERSION);
#endif
	printf("\n%s\n", COPYRIGHT);
}

static void license(void) {
	version();
	/* todo */
}

static void warning(char *fmt, ...) {
	va_list args;

	fprintf(stderr, "%s: ", progname);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	if (errno) {
		exit_value |= errno;
		fprintf(stderr, ": ");
		fprintf(stderr, strerror(errno));
	}
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(args);
}

static void error(char *fmt, ...) {
	va_list args;

	fprintf(stderr, "%s: ", progname);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	if (errno) {
		exit_value |= errno;
		fprintf(stderr, ": ");
		fprintf(stderr, strerror(errno));
	}
	fprintf(stderr, "\n");
	fflush(stderr);
	va_end(args);
	if (errno == ENOMEM || opt.abort)
		exit(exit_value ? exit_value : 1);
	errno = 0;
}

static inline void *xalloc(int size) {
	void *p = malloc(size);
	if (!p) {
		error("out of memory");
		exit(ENOMEM);
	}

	return p;
}

static inline char *xstrdup(char *s) {
	char *p = strdup(s);
	if (!p) {
		error("out of memory");
		exit(ENOMEM);
	}

	return p;
}

static inline int read1(int fh, char* c) {
	ssize_t bytes = read(fh, c, 1);
	if (bytes < 0)
		error("cannot read %s", input_filename);
	return bytes;
}

static inline void write1(int fh, char c) {
/*	_DBG("write1(%s%c)", ((c < 32) ? "^" : ""), ((c < 32) ? c + 64 : c)); */
	if (fh != -1 && write(fh, &c, 1) < 0)
		error("cannot write to %s", temp_filename);
}

static inline void write_nl(int fh, char c, output_t output_format) {
	switch (output_format) {
		case OUTPUT_LEAVE:
			write1(fh, c);
			break;
		case OUTPUT_UNIX:
			write1(fh, CTRL_J);
			break;
		case OUTPUT_DOS:
			write1(fh, CTRL_M);
			write1(fh, CTRL_J);
			break;
		case OUTPUT_MAC:
			write1(fh, CTRL_M);
			break;
		case OUTPUT_SKIP:
			break;
	}
}

static int convert(int fh, int fh2, input_t *input_format, bool *rewrite, bool *binary, int include, output_t output_format) {
	int rv = 0;
	char c = -1;
	bool read_next_char = true;
	bool check_binary = true;
#ifdef CONV_SUPPORT_ENABLED
	bool first_line = true;
#endif /* CONV_SUPPORT_ENABLED */

	*input_format = INPUT_NONE;
	*rewrite = false;
	*binary = false;

#ifdef HAVE_SETMODE
	if (!isatty(fh))
		setmode(fh, O_BINARY);
	if (fh2 != -1)
		if (!isatty(fh2))
			setmode(fh2, O_BINARY);
#endif

	for (;;) {
		if (read_next_char)
			if ((rv = read1(fh, &c)) <= 0)
				break;

		/* Does the file contain ASCII 0x00-0x08, 0x0e-0x1f, or 0x7f ? */
		if (check_binary && iscntrl(c) && !isspace(c)) {
			/* A Ctrl-D or Ctrl-Z at the end of the file, does not make it a binary file */
			if (c == CTRL_D || c == CTRL_Z) {
				write1(fh2, c);
				if ((rv = read1(fh, &c)) <= 0)
					break;
			}
			*binary = true;
			if ((include & INPUT_BINARY) != INPUT_BINARY) {
				*rewrite = false;
				break;
			}
			check_binary = false;
		}

		read_next_char = true;

		switch (c) {
			case CTRL_J:
#ifdef CONV_SUPPORT_ENABLED
				if (first_line && opt._auto) {
					/* UNIX to DOS */
					output_format = OUTPUT_DOS;
					first_line = false;
				}
#endif /* CONV_SUPPORT_ENABLED */
				*input_format = (input_t) (*input_format | INPUT_UNIX);
				/* LF: rewrite if DOS or Mac */
				*rewrite |= (output_format & OUTPUT_DOS) | (output_format & OUTPUT_MAC);
				write_nl(fh2, CTRL_J, output_format);
				break;

			case CTRL_M:
				/* CR: rewrite if Unix */
				*rewrite |= (output_format & OUTPUT_UNIX);
				if ((rv = read1(fh, &c)) <= 0) {
					if (*input_format == INPUT_NONE)
						*input_format = (input_t) (*input_format | INPUT_MAC);
					*rewrite |= (output_format & OUTPUT_DOS);
					write_nl(fh2, CTRL_M, output_format);
					break;
				}

				if (c == CTRL_J) { /* CR / LF */
#ifdef CONV_SUPPORT_ENABLED
					if (first_line && opt._auto) {
						/* DOS to UNIX */
						output_format = OUTPUT_UNIX;
						first_line = false;
					}
#endif /* CONV_SUPPORT_ENABLED */

					*input_format = (input_t) (*input_format | INPUT_DOS);
					/* rewrite if Mac */
					*rewrite |= (output_format & OUTPUT_MAC);
					if (output_format == OUTPUT_LEAVE) {
						write1(fh2, CTRL_M);
						write1(fh2, CTRL_J);
						continue;
					}
					write_nl(fh2, CTRL_J, output_format);
					continue;
				}

#ifdef CONV_SUPPORT_ENABLED
				if (first_line && opt._auto) {
					/* MAC/varied to UNIX */
					output_format = OUTPUT_UNIX;
					first_line = false;
				}
#endif /* CONV_SUPPORT_ENABLED */

				*input_format = (input_t) (*input_format | INPUT_MAC);

				if (c != CTRL_M) { /* CR / not CR or LF */
					/* rewrite if Unix or DOS */
					*rewrite |= (output_format & OUTPUT_UNIX) | (output_format & OUTPUT_DOS);
					write_nl(fh2, CTRL_M, output_format);
					read_next_char = false;
					continue;
				}

				/* CR / CR, reading the 3rd character */
				if ((rv = read1(fh, &c)) <= 0) {
					/* rewrite if Unix or DOS */
					*rewrite |= (output_format & OUTPUT_UNIX) | (output_format & OUTPUT_DOS);
					write_nl(fh2, CTRL_M, output_format);
					write_nl(fh2, CTRL_M, output_format);
					break;
				}

				if (c == CTRL_J) { /* CR / CR / LF: bad perforce sync, treat as one EOL */
					*input_format = (input_t) (*input_format | INPUT_DOS);
					/* rewrite if Unix or DOS or Mac */
					*rewrite |= OUTPUT_UNIX | OUTPUT_DOS | OUTPUT_MAC;
					if (output_format == OUTPUT_LEAVE) {
						write1(fh2, CTRL_M);
						write1(fh2, CTRL_M);
						write1(fh2, CTRL_J);
						continue;
					}
					write_nl(fh2, CTRL_M, output_format);
					continue;
				}

				if (c == CTRL_M) { /* CR / CR / CR: must be a mac file, treat as three EOLs */
					/* rewrite if Unix or DOS */
					*rewrite |= (output_format & OUTPUT_UNIX) | (output_format & OUTPUT_DOS);
					write_nl(fh2, CTRL_M, output_format);
					write_nl(fh2, CTRL_M, output_format);
					write_nl(fh2, CTRL_M, output_format);
					continue;
				}

				/* CR / CR / not CR or LF: treat as two EOLs */
				*rewrite |= (output_format & OUTPUT_UNIX) | (output_format & OUTPUT_DOS);
				write_nl(fh2, CTRL_M, output_format);
				write_nl(fh2, CTRL_M, output_format);
				read_next_char = false;
				break;

			default:
				write1(fh2, c);
				break;
		}
	}

	return rv == -1 ? -1 : 0;
}

static int copy_file(char *src, char *dst, int dst_options) {
	int fh;
	int fh2;
	int rv = 0;

	fh = open(src, O_RDONLY | O_BINARY);
	if (fh == -1) {
		error("cannot open %s", src);
		return errno;
	}

	fh2 = open(dst, O_WRONLY | O_TRUNC | O_BINARY | dst_options);
	if (fh2 == -1) {
		error("cannot open %s", dst);
		close(fh);
		return errno;
	}

	for (;;) {
		char buf[BUFSIZ];
		ssize_t bytes;

		bytes = read(fh, buf, sizeof(buf));

		if (bytes < 0) {
			error("cannot read %s", src);
			rv = errno;
			break;
		}

		if (bytes == 0) /* EOF */
			break;

		if (write(fh2, buf, bytes) < 0) {
			error("cannot write to %s", dst);
			rv = errno;
			break;
		}
	}

	if (close(fh) == -1) {
		error("cannot close %s", src);
		rv = errno;
	}

	if (close(fh2) == -1) {
		error("cannot close %s", dst);
		rv = errno;
	}

	return rv;
}

static int preserve(char *name, struct stat *statbuf) {
	struct utimbuf utimebuf;
	int rv = 0;

	utimebuf.actime		= statbuf->st_atime;
	utimebuf.modtime	= statbuf->st_mtime;

	if (utime(name, &utimebuf) == -1) {
		rv |= errno;
		error("cannot set time for %s", name);
	}

#ifdef HAVE_CHOWN
	if (chown(name, statbuf->st_uid, statbuf->st_gid) == -1) {
		rv |= errno;
		if (opt.verbose >= 1)
			warning("cannot set ownership for %s", name);
	}
#endif

	if (chmod(name, statbuf->st_mode & 07777) == -1) {
		rv |= errno;
		error("cannot set rights for %s", name);
	}

	return rv;
}

struct _list {
	void			*data;
	struct _list	*next;
};

typedef struct _list List;

static List *list_new(void *data) {
	List *node = (List *) xalloc(sizeof(List));

	node->data = data;
	node->next = NULL;

	return node;
}

static List *list_last(List *node) {
	if (!node)
		return NULL;

	while (node->next)
		node = node->next;

	return node;
}

static List *list_append(List **head, void *data) {
	List *node = list_new(data);

	if (*head == NULL)
		*head = node;
	else {
		List *tail = list_last(*head);
		tail->next = node;
	}

	return node;
}

/*
static int list_count(List *node) {
	int rv = 0;

	while (node) {
		++rv;
		node = node->next;
	}

	return rv;
}
*/

static void list_free(List *node) {
	while (node) {
		List *next = node->next;
		free(node);
		node = next;
	}
}

struct _file {
	char		*filespec;
	int			include;
	output_t	output_format;
};

typedef struct _file File;

static File *filespec_new(char *filespec, output_t output_format, int include) {
	File *node = (File *) xalloc(sizeof(File));

	node->filespec		= filespec;
	node->include		= include;
	node->output_format = output_format;

	return node;
}

static List *filespec_append(List **head, char *filespec, output_t output_format, int include, bool dont_alloc) {
	char *p;
	void *data;
	List *node = *head;

	if (dont_alloc)
		p = filespec;
	else
		p = xstrdup(filespec);

	data = (void *) filespec_new(p, output_format, include);
	if (strchr(p, '*') || strchr(p, '?')) {
		opt.wildcards_used = true;
	}

	while (node) {
		File *data = (File *) node->data;
		bool filespecs_equal;

		if (opt.ignore_case) {
				filespecs_equal = stricmp(data->filespec, filespec) == 0;
		} else {
				filespecs_equal = strcmp(data->filespec, filespec) == 0;
		}

		if (filespecs_equal) {
			/* free(data->filespec); */
			data->filespec = p;
			data->include = include;
			data->output_format = output_format;
			return node;
		}
		node = node->next;
	}

	return list_append(head, data);
}

static void filespec_free(List *head) {
	List *node = head;

	while (node) {
		File *data = (File *) node->data;
		free(data->filespec);
		node = node->next;
	}

	list_free(head);
}

static int process_directory(char *directory, List *file_list);

char *get_filename(char *filename, char *real_filename) {
	static char buf[PATH_MAX * 2 + 4];

	if (strcmp(filename, real_filename)) {
		sprintf(buf, "%s (%s)", filename, real_filename);
		return buf;
	}

	return filename;
}

static int process_file(char *filename, output_t output_format, int include, bool do_file, bool *is_directory) {
	int		fh;
	struct	stat statbuf;
	input_t input_format = INPUT_NONE;
	bool	rewrite	= false;
	bool	binary	= false;
	bool	excluded = false;
	bool	do_write = (bool)(!(opt.dry_run || output_format == OUTPUT_LEAVE));

	char	backup_filename[PATH_MAX];
	char	real_filename[PATH_MAX];
	char	tmp_filename[PATH_MAX];

	strncpy(tmp_filename, filename, sizeof(tmp_filename));

#define DEBUG_STAT

#ifdef DEBUG_STAT
	_DBG("tmp_filename='%s'", tmp_filename);
#endif

	for (;;) {
		errno = 0;

#ifndef HAVE_LSTAT
		if (stat(tmp_filename, &statbuf) != -1)
			strncpy(real_filename, tmp_filename, sizeof(real_filename));
# ifdef DEBUG_STAT
		_DBG("real_filename='%s'", real_filename);
# endif
#else
		while (errno == 0) {
			if (lstat(tmp_filename, &statbuf) == -1)
				break;

			if (!S_ISLNK(statbuf.st_mode)) {
				strcpy(real_filename, tmp_filename);
				break;
			}

			memset(real_filename, 0, sizeof(real_filename)); /* Cygwin bugfix */
			if (readlink(tmp_filename, real_filename, sizeof(real_filename)) == -1)
				break;

# ifdef DEBUG_STAT
		_DBG("real_filename='%s'", real_filename);
# endif

			strncpy(tmp_filename, real_filename, sizeof(tmp_filename));
		}
#endif

		if (errno) {
			if (strcmp(filename, tmp_filename) == 0)
				error("cannot access %s", get_filename(filename, tmp_filename));
			break;
		}

		if ((statbuf.st_mode & S_IFDIR) == S_IFDIR) {
			*is_directory = true;
			errno = 0;
			break;
		}

		if (!do_file) {
			errno = 0;
			break;
		}

		if ((statbuf.st_mode & S_IFREG) != S_IFREG) {
			error("%s is not a regular file", filename);
			break;
		}

		if ((statbuf.st_mode & S_IRUSR) != S_IRUSR) {
			error("cannot read %s", filename);
			break;
		}

		if (do_write && (statbuf.st_mode & S_IWUSR) != S_IWUSR) {
			error("cannot write to %s", filename);
			break;
		}

		output_handle = -1;

		if (do_write) {
			char *dir;

			if (*opt.temp_dir) {
				dir = opt.temp_dir;
			} else {
				dir = dirname(filename);
				if (dir == NULL) {
					errno = ENAMETOOLONG;
					error("path too long: %s", filename);
					break;
				}

				if (!*dir)
					dir = ".";
			}

			strcpy(temp_filename, dir);
			if (strlen(temp_filename) + strlen(TEMP_FILE_TEMPLATE) > sizeof(temp_filename) - 1) {
				errno = ENAMETOOLONG;
				error("path too long: %s", filename);
				break;
			}

			strcat(temp_filename, TEMP_FILE_TEMPLATE);
			output_handle = mkstemps(temp_filename, TEMP_FILE_TEMPLATE_SUFFIX_LEN);
			if (output_handle == -1) {
				error("cannot open %s", temp_filename);
				break;
			}
		}

		fh = open(real_filename, O_RDONLY | O_BINARY);
		if (fh == -1) {
			int e = errno;
			error("cannot open %s", filename);
			if (do_write) {
				close(output_handle);
				output_handle = -1;
				unlink(temp_filename);
			}
			errno = e;
			break;
		}

		convert(fh, output_handle, &input_format, &rewrite, &binary, include, output_format);

		if (close(fh) == -1) {
			int e = errno;
			error("cannot close %s", filename);
			if (do_write) {
				close(output_handle);
				output_handle = -1;
				unlink(temp_filename);
			}
			errno = e;
			break;
		}

		if (input_format == INPUT_NONE) {
			rewrite = false;
			if (include != INPUT_ALL_ASCII)
				excluded = true;
		} else {
			if (binary) {
				if ((include & INPUT_BINARY) != INPUT_BINARY) {
					rewrite = false;
					excluded = true;
				}
			} else
				if (input_format == INPUT_UNIX || input_format == INPUT_DOS || input_format == INPUT_MAC) {
					if ((include & input_format) != input_format) {
						rewrite = false;
						excluded = true;
					}
				} else
					if ((include & INPUT_VARIED) != INPUT_VARIED) {
						rewrite = false;
						excluded = true;
					}
		}

		if (opt.force)
			rewrite = true;

		if (!excluded && !opt.quiet) {
			char *input_str;
			char *output_str;

			input_str = opt.verbose >= 2 ? input_descv[input_format] : input_desc[input_format];
			output_str = output_desc[rewrite ? output_format : OUTPUT_LEAVE];

			if (binary) {
				input_str = T_BINARY;
				if ((include & INPUT_BINARY) != INPUT_BINARY)
					output_str = T_UNCHANGED;
			}

			if (opt.verbose >= 1 || rewrite) {
				if (do_write || opt.dry_run)
					printf("%-13s: %-6s > %s\n", filename, input_str, output_str);
				else
					printf("%-13s: %-6s\n", filename, input_str);
			}
		}

		if (!do_write) {
			errno = 0;
			break;
		}

		if (close(output_handle) == -1) {
			int e = errno;
			error("cannot close %s", temp_filename);
			unlink(temp_filename);
			errno = e;
			break;
		}

		if (excluded || !rewrite) {
			if (unlink(temp_filename) == -1)
				if (errno != ENOENT) {
					error("cannot delete %s", temp_filename);
					/* ignore */
				}
			errno = 0;
			break;
		}

		if (opt.backup) {
			/* todo strip off file extension if 8.3 filesystem? */

			strcpy(backup_filename, filename);
			if (strlen(backup_filename) + strlen(opt.bak_ext) > sizeof(backup_filename) - 1) {
				errno = ENAMETOOLONG;
				error("path too long: %s", filename);
				break;
			}
			strcat(backup_filename, opt.bak_ext);

			if (unlink(backup_filename) == -1)
				if (errno != ENOENT) {
					warning("cannot delete %s", backup_filename);
					errno = 0;
					/* ignore */
				}
		}

		if (opt.in_place) {
			if (opt.backup)
				if (copy_file(filename, backup_filename, O_CREAT) != 0)
					break;

			if (copy_file(temp_filename, filename, 0) != 0) {
				if (opt.backup) {
					if (copy_file(backup_filename, filename, 0) == 0)
						unlink(backup_filename);
				}
				break;
			}

			if (unlink(temp_filename) == -1) {
				error("cannot delete %s", temp_filename);
				break;
			}
		} else { /* !opt.in_place */
			if (opt.backup) {
				if (rename(filename, backup_filename) == -1) {
					error("cannot rename %s to %s", filename, backup_filename);
					break;
				}
			} else {
				if (unlink(filename) == -1)
					if (errno != ENOENT) {
						warning("cannot delete %s", filename);
						errno = 0;
						/* ignore */
					}
			}

			if (rename(temp_filename, filename) == -1) {
				bool failed = true;
				if (*opt.temp_dir) {
					/* maybe rename failed because temp_dir is on another
					   filesystem, so let's try to copy */
					failed = (bool) copy_file(temp_filename, filename, 0);
				}
				if (!failed) {
					if (opt.backup)
						rename(backup_filename, filename);
					error("cannot rename %s to %s", temp_filename, filename);
					break;
				}
			}
		} /* opt.in_place */

		if (opt.preserve)
			preserve(filename, &statbuf);

		break;
	}

	temp_filename[0] = '\0';

	return errno;
}

static int process_directory(char *directory, List *file_list) {
	DIR *dirp;
	int rv = 0;
	int fnmatch_flags = 0;
	
	List *dir_list = NULL;
	List *node;

	dirp = opendir(directory);

	if (dirp == NULL) {
		error("cannot read directory %s", directory);
		return 1;
	}

	if (opt.ignore_case) {
		fnmatch_flags |= FNM_CASEFOLD;
	} else {
		fnmatch_flags &= ~FNM_CASEFOLD;
	}

	for (;;) {
		struct dirent *dp;
		char *path;
		int len;
		File *file = NULL;
		bool is_directory = false;
		bool do_file;
		bool skip;
		output_t output_format = OUTPUT_LEAVE;

		errno = 0;
		dp = readdir(dirp);
		if (dp == NULL) {
			if (errno == 0)
				break;
			error("error reading directory %s", directory);
			break;
		}

		if (strcmp(dp->d_name, ".") == 0)
			continue;

		if (strcmp(dp->d_name, "..") == 0)
			continue;

		do_file = false;
		skip = false;
		node = file_list;
		while (node) {
			file = (File *) node->data;

			if (fnmatch(file->filespec, dp->d_name, fnmatch_flags) == FNM_NOMATCH) {
				node = node->next;
				continue;
			}

			if (file->output_format == OUTPUT_SKIP)
				skip = true;
			else
				do_file = true;

			output_format = file->output_format;
			break;
		}

		if (strcmp(directory, ".") == 0) {
			path = xstrdup(dp->d_name);
		} else {
			len = strlen(directory) + strlen(dp->d_name) + 1;

			if (len > PATH_MAX - 1) {
				errno = ENAMETOOLONG;
				error("path too long: %s/%s", directory, dp->d_name);
				continue;
			}

			path = (char *) xalloc(len + 1);

			sprintf(path, "%s/%s", directory, dp->d_name);
		}

		rv |= process_file(path, output_format, file->include, (bool) (do_file && !skip), &is_directory);

		if (is_directory && opt.recursive)
			filespec_append(&dir_list, path, file->output_format, file->include, true);
	}

	node = dir_list;
	while (node) {
		File *file = (File *) node->data;

		rv |= process_directory(file->filespec, file_list);

		node = node->next;
	}

	filespec_free(dir_list);

	closedir(dirp);

	return rv;
}

static void sighandler(int sig) {
	signal(sig, SIG_IGN);

	if (output_handle != -1)
		close(output_handle);
	if (*temp_filename && output_handle != fileno(stdout))
		unlink(temp_filename);

	if (sig == SIGINT)
		fprintf(stderr, "terminated by user");
	exit(EXIT_FAILURE);
}

static int process_filelist(List **file_list, char *filename) {
	FILE* fp;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		error("cannot open %s", filename);
		return errno;
	}

	for (;;) {
		char buf[BUFSIZ];
		char *pstart;
		char *pend;

		pstart = fgets(buf, sizeof(buf), fp);

		if (pstart == NULL) {
			if (ferror(fp)) {
				error("read error reading %s", filename);
				return errno;
			}
			fclose(fp);
			break;
		}

		pend = strstr(pstart, "//");
		if (pend)
			*pend = '\0';

		while (isspace(*pstart))
			++pstart;
		if (!*pstart)
			continue;

		pend = pstart + strlen(pstart) - 1;
		while (isspace(*pend))
			--pend;
		++pend;
		*pend = '\0';

		if (!*pstart)
		continue;

		filespec_append(file_list, pstart, opt.output_format, opt.include, false);
	}

	return 0;
}

static char* str_trim(char *p) {
	char *pend;

	while (isspace(*p))
		++p;
	if (*p) {
		pend = p + strlen(p) - 1;
		while (isspace(*pend))
			--pend;
		++pend;
		*pend = '\0';
	}

	return p;
}

static int process_options(List **file_list, int argc, char **argv);

#define COMMENT_DELIMETER "//"
#define TOKEN_DELIMETERS " \t"

static int process_envvar(List **file_list, char *envvar) {
	List *argv_list = NULL;
	int argc = 1;
	char **argv;
	List *node;
	char **p;

	char *pstart;
	char *pend;
	char *token;

	pstart = getenv(envvar);

	if (!pstart || !*pstart) {
		return 0;
	}
		
	pend = strstr(pstart, COMMENT_DELIMETER);
	if (pend)
		*pend = '\0';

	pstart = str_trim(pstart);

	if (strlen(pstart) == 0)
		return 0;

	token = strtok(pstart, TOKEN_DELIMETERS);

	while (token != NULL) {
		token = str_trim(token);

		if (strlen(token)) {
			++argc;
			list_append(&argv_list, (void *) token);
		}

		token = strtok(NULL, TOKEN_DELIMETERS);
	}

	if (argc == 1)
		return 0;

	argv = (char **) xalloc((argc + 1) * sizeof(char *));
	p = argv;
	*p++ = progname;
	node = argv_list;
	while (node) {
		*p++ = (char *) node->data;
		node = node->next;
	}
	*p = NULL;

	process_options(file_list, argc, argv);

	free(argv);
	
	list_free(argv_list);

	return 0;
}

static int process_option_file(List **file_list, char *filename) {
	FILE* fp;
	List *argv_list = NULL;
	int argc = 1;
	char **argv;
	List *node;
	char **p;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		error("cannot open %s", filename);
		return errno;
	}

	for (;;) {
		char buf[BUFSIZ];
		char *pstart;
		char *pend;
		char *pnew;
		char *token;
		
		pstart = fgets(buf, sizeof(buf), fp);

		if (pstart == NULL) {
			if (ferror(fp)) {
				error("read error reading %s", filename);
				return errno;
			}
			fclose(fp);
			break;
		}

		pend = strstr(pstart, COMMENT_DELIMETER);
		if (pend)
			*pend = '\0';

		pstart = str_trim(pstart);
	
		if (strlen(pstart) == 0)
			continue;
		
		if (*pstart == '-') {
			pnew = xstrdup(pstart);
		} else {
			pnew = (char *) xalloc(strlen(pstart) + 3);
			strcpy(pnew, "--");
			strcat(pnew, pstart);
		}

		token = strtok(pnew, TOKEN_DELIMETERS);
		while (token != NULL) {
			token = str_trim(token);

			if (strlen(token)) {
				++argc;
				list_append(&argv_list, (void *) xstrdup(token));
			}

			token = strtok(NULL, TOKEN_DELIMETERS);
		}
		
		free(pnew);
	}

	fclose(fp);

	if (argc == 1)
		return 0;
	
	argv = (char **) xalloc((argc + 1) * sizeof(char *));
	p = argv;
	*p++ = xstrdup(progname);
	node = argv_list;
	while (node) {
		*p++ = (char *) node->data;
		node = node->next;
	}
	*p = NULL;

	process_options(file_list, argc, argv);

	free(argv);

	node = argv_list;
	while (node) {
		free(node->data);
		node = node->next;
	}

	list_free(argv_list);

	return 0;
}

static bool opt_compare(char **optarg, char *option) {
	if (strcasecmp(*optarg, option) == 0) {
		(*optarg) += strlen(*optarg);
		return true;
	}

	if (*optarg[0] == option[0]) {
		(*optarg)++;
		return true;
	}

	return false;
}

static char *long_option_name(int c) {
	int i;

	for (i = 0; i < (sizeof(long_options) / sizeof(long_options[0])); ++i) {
		if (long_options[i].val == c) {
			return (char *) long_options[i].name;
		}
	}

	return NULL;
}

static void unimplemented_option(int c) {
	errno = 0;
	if (long_option_name(c)) {
		if (c > CHAR_MAX)
			error("%s %d (--%s)", UNIMPLEMENTED_OPTION, c, long_option_name(c));
		else
			error("%s -%c (--%s)", UNIMPLEMENTED_OPTION, c, long_option_name(c));
	} else {
		if (c > CHAR_MAX)
			error("%s %d", UNIMPLEMENTED_OPTION, c);
		else
			error("%s -%c", UNIMPLEMENTED_OPTION, c);
	}
	exit(EINVAL);
}

static bool process_filename(List **file_list, char *arg) {
	if (*arg == '@') {
		process_filelist(file_list, ++arg);
		return true;
	}

	filespec_append(file_list, arg, opt.output_format, opt.include, false);

	return true;
}

static int process_options(List **file_list, int argc, char **argv) {
	int last_optind = 0;

	/* turn off getopt's error messages */
	opterr = 0;
	optind = 1;
	optreset = 1;
	
	while (optind < argc) {
		int c;
		int option_index = 0;

		if (optind <= last_optind)
			optind = last_optind + 1;

		if (optind >= argc)
			break;

		if (last_optind < optind)
			last_optind = optind;

		c = getopt_long(argc, argv, short_options, long_options, &option_index);

		if (c <= 1) {
			if (strcmp(argv[optind - 1], "--") == 0) {
				++last_optind;
				for (; last_optind < argc; ++last_optind) {
					process_filename(file_list, argv[last_optind]);
				}
				break;
			}
			process_filename(file_list, argv[last_optind]);
			continue;
		}

		for (; last_optind < optind - 1; ++last_optind) {
			if (*argv[last_optind] == '-') {
				continue;
			}
			process_filename(file_list, argv[last_optind]);
		}

#ifdef _DEBUG
		if (long_option_name(c)) {
			if (c > CHAR_MAX)
				_DBG("%s %d (--%s) (%s)", PROCESSING_OPTION, c, long_option_name(c), argv[optind - 1]);
			else
				_DBG("%s -%c (--%s) (%s)", PROCESSING_OPTION, c, long_option_name(c), argv[optind - 1]);
		} else {
			if (c > CHAR_MAX)
				_DBG("%s %d (%s)", PROCESSING_OPTION, c, argv[optind - 1]);
			else
				_DBG("%s -%c (%s)", PROCESSING_OPTION, c, argv[optind - 1]);
		}
#endif

		switch (c) {
			case '-':	/* - */
				/* read from stdin */
				opt.std_in = true;
				break;
			case 'a':	/* --abort */
				opt.abort = true;
				break;
			case 'b': 	/* --backup */
				opt.backup = true;
				break;
			case 'c': 	/* --compress */
				unimplemented_option(c);
				opt.compress = true;
				break;
			case 'd':	/* --dos */
			case 'w':	/* --windows (undocumented) */
				opt.output_format = OUTPUT_DOS;
#ifdef CONV_SUPPORT_ENABLED
				opt._auto = false;
#endif /* CONV_SUPPORT_ENABLED */
				break;
			case 'e':	/* --exclude */
				while (optarg && *optarg) {
					_DBG("Processing exclude option %c (%s)", *optarg, optarg);
					if (opt_compare(&optarg, T_UNIX))
						opt.include &= ~INPUT_UNIX;
					else if (opt_compare(&optarg, T_DOS))
						opt.include &= ~INPUT_DOS;
					else if (opt_compare(&optarg, T_MAC))
						opt.include &= ~INPUT_MAC;
					else if (opt_compare(&optarg, T_VARIED))
						opt.include &= ~INPUT_VARIED;
					else if (opt_compare(&optarg, T_BINARY))
						opt.include &= ~INPUT_BINARY;
					else if (opt_compare(&optarg, T_ALL))
						opt.include = INPUT_NONE;
					else if (opt_compare(&optarg, T_NONE))
						opt.include = INPUT_ALL;
					else {
						errno = 0;
						error("invalid exclude option: %s", optarg);
						usage(EINVAL);
					}
				}
				break;
			case 'f':	/* --force */
				opt.force = true;
				break;
			case 'g': 	/* --config */
				process_option_file(file_list, optarg);
				break;
			/* case 'h': --help (undocumented) */
			case 'i':	/* --include */
				while (optarg && *optarg) {
					_DBG("Processing include option %c (%s)", *optarg, optarg);
					if (opt_compare(&optarg, T_UNIX))
						opt.include |= INPUT_UNIX;
					else if (opt_compare(&optarg, T_DOS))
						opt.include |= INPUT_DOS;
					else if (opt_compare(&optarg, T_MAC))
						opt.include |= INPUT_MAC;
					else if (opt_compare(&optarg, T_VARIED))
						opt.include |= INPUT_VARIED;
					else if (opt_compare(&optarg, T_BINARY))
						opt.include |= INPUT_BINARY;
					else if (opt_compare(&optarg, T_ALL))
						opt.include = INPUT_ALL;
					else if (opt_compare(&optarg, T_NONE))
						opt.include = INPUT_NONE;
					else {
						errno = 0;
						error("invalid include option: %s", optarg);
						usage(EINVAL);
					}
				}
				break;
			case 'j':	/* --in-place */
				opt.in_place = true;
				break;
			case 'k': 	/* --backup-ext */
				if (strlen(optarg) >= sizeof(opt.bak_ext)) {
					errno = ENAMETOOLONG;
					error("backup-ext argument too long: '%s", optarg);
				} else
					strcpy(opt.bak_ext, optarg);
				break;
			case 'l': 	/* --leave */
				opt.output_format = OUTPUT_LEAVE;
#ifdef CONV_SUPPORT_ENABLED
				opt._auto = false;
#endif /* CONV_SUPPORT_ENABLED */
				break;
			case 'm':	/* --mac */
				opt.output_format = OUTPUT_MAC;
#ifdef CONV_SUPPORT_ENABLED
				opt._auto = false;
#endif /* CONV_SUPPORT_ENABLED */
				break;
			case 'n': 	/* --native */
				opt.output_format = OUTPUT_NATIVE;
#ifdef CONV_SUPPORT_ENABLED
				opt._auto = false;
#endif /* CONV_SUPPORT_ENABLED */
				break;
			case 'o': 	/* --output */
				if (strlen(optarg) >= PATH_MAX) {
					errno = ENAMETOOLONG;
					error("output argument too long: '%s", optarg);
				} else {
					if (strcmp(optarg, "-") == 0) {
						opt.std_out = true;
					} else {
						opt.std_out = false;
						strcpy(opt.output_filename, optarg);
					}
				}
				break;
			case 'p':	/* --preserve */
				opt.preserve = true;
				break;
			case 'q':	/* --quiet */
				opt.quiet = true;
				opt.verbose = 0;
				break;
			case 'r':	/* --recursive */
				opt.recursive = true;
				break;
			case 's':	/* --skip */
				opt.output_format = OUTPUT_SKIP;
				break;
			case 't': 	/* --temp-dir */
				if (strlen(optarg) >= PATH_MAX - strlen(TEMP_FILE_TEMPLATE)) {
					errno = ENAMETOOLONG;
					error("temp-dir argument too long: '%s", optarg);
				} else
					strcpy(opt.temp_dir, optarg);
				break;
			case 'u':	/* --unix */
				opt.output_format = OUTPUT_UNIX;
#ifdef CONV_SUPPORT_ENABLED
				opt._auto = false;
#endif /* CONV_SUPPORT_ENABLED */
				break;
			case 'v':	/* --verbose */
				++opt.verbose;
				opt.quiet = false;
				break;
			/* case 'w': --windows (undocumented) */
			case 'y':	/* --dry-run */
				opt.dry_run = true;
				break;
			case 'z':	/* --ignore_case */
				opt.ignore_case = true;
				break;
			case '?':	/* --help */
				if (optopt != '?') {
					errno = 0;
					error("invalid option: %s", argv[optind - 1]);
					usage(EINVAL);
				}
			case 'h':	/* --help (undocumented) */
				usage(EXIT_SUCCESS);
				break;
			case OPT_LICENSE: 	/* --license */
				license();
				exit(EXIT_SUCCESS);
				break;
			case OPT_VERSION:	/* --version */
				version();
				exit(EXIT_SUCCESS);
				break;

			/* negate options */
			case 'A':	/* --no-abort */
				opt.abort = false;
				break;
			case 'B':	/* --no-backup */
				opt.backup = false;
				break;
			case 'C': 	/* --no-compress */
				unimplemented_option(c);
				opt.compress = false;
				break;
			case 'F':	/* --no-force */
				opt.force = false;
				break;
			case 'K':	/* --no-backup-ext */
				strcpy(opt.bak_ext, BACKUP_EXT);
				break;
			case 'J':	/* --no-in-place */
				opt.in_place = false;
				break;
			case 'O':	/* --no-output */
				strcpy(opt.output_filename, "");
				break;
			case 'P':	/* --no-preserve */
				opt.preserve = false;
				break;
			case 'Q':	/* --no-quiet */
				opt.quiet = false;
				break;
			case 'R':	/* --no-recursive */
				opt.recursive = false;
				break;
			case 'T':	/* --no-temp-dir */
				strcpy(opt.temp_dir, "");
				break;
			case 'V':	/* --no-verbose */
				--opt.verbose;
				break;
			case 'Y':	/* --no-dry-run */
				opt.dry_run = false;
				break;
			case 'Z':	/* --no-ignore_case */
				opt.ignore_case = false;
				break;
#ifdef CONV_SUPPORT_ENABLED
			case OPT_AUTO:	/* --auto */
				opt._auto = true;
				break;
			case 'D':	/* -D (conv_mode) */
				opt.output_format = OUTPUT_DOS;
				opt._auto = false;
				break;
			case OPT_FORCE:	/* --force */
				opt.include |= INPUT_BINARY;
				break;
			case 'M':	/* -M */
				opt.output_format = OUTPUT_MAC;
				opt._auto = false;
				break;
			case OPT_SAFE:	/* --safe */
				opt.include &= ~INPUT_BINARY;
				break;
			case 'U':	/* -U */
				opt.output_format = OUTPUT_UNIX;
				opt._auto = false;
				break;
			case OPT_NO_AUTO:	/* --no-auto */
				opt._auto = false;
				break;
			case OPT_NO_FORCE:	/* --no-force */
				opt.include &= ~INPUT_BINARY;
				break;
			case OPT_NO_SAFE:	/* --no-safe */
				opt.include |= INPUT_BINARY;
				break;

#endif /* CONV_SUPPORT_ENABLED */
			default:
				errno = 0;
				error("invalid option: %s", argv[optind - 1]);
				usage(EINVAL);
		} /* switch (c) { */

	} /* while (optind < argc) { */

	if (opt.include == INPUT_NONE) {
		errno = 0;
		error("invalid include/exclude options");
		usage(EINVAL);
	}

	return 0;
}

int main(int argc, char **argv) {
	List *file_list	= NULL;

#ifdef SIGHUP
	if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
		signal(SIGHUP, sighandler);
#endif
#ifdef SIGINT
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, sighandler);
#endif
#ifdef SIGPIPE
	if (signal(SIGPIPE, SIG_IGN) != SIG_IGN)
		signal(SIGPIPE, sighandler);
#endif
#ifdef SIGQUIT
	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
		signal(SIGQUIT, sighandler);
#endif
#ifdef SIGTERM
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, sighandler);
#endif

	progname = basename(argv[0]);

	if (progname) {
#ifdef __DOS__
		int len = strlen(progname);
		if (len > 4 && strcasecmp(progname + len - 4, ".exe") == 0)
			progname[len - 4] = '\0';
#endif
#ifdef CONV_SUPPORT_ENABLED
		if (strcasecmp(progname, "conv") == 0) {
			opt.conv_mode = true;
		}
#endif /* CONV_SUPPORT_ENABLED */

		if (strcasecmp(progname, "dos2unix") == 0 ||
			strcasecmp(progname, "d2u") == 0 ||
			strcasecmp(progname, "m2u") == 0) {
			opt.output_format = OUTPUT_UNIX;
#ifdef CONV_SUPPORT_ENABLED
			opt.conv_mode = true;
#endif /* CONV_SUPPORT_ENABLED */
		}

		if (strcasecmp(progname, "unix2dos") == 0 ||
			strcasecmp(progname, "u2d") == 0 ||
			strcasecmp(progname, "m2d") == 0) {
			opt.output_format = OUTPUT_DOS;
#ifdef CONV_SUPPORT_ENABLED
			opt.conv_mode = true;
#endif /* CONV_SUPPORT_ENABLED */
		}

		if (strcasecmp(progname, "unix2mac") == 0 ||
			strcasecmp(progname, "dos2mac") == 0 ||
			strcasecmp(progname, "u2m") == 0 ||
			strcasecmp(progname, "d2m") == 0) {
			opt.output_format = OUTPUT_MAC;
#ifdef CONV_SUPPORT_ENABLED
			opt.conv_mode = true;
#endif /* CONV_SUPPORT_ENABLED */
		}
 	}

	process_envvar(&file_list, EOLFIX_ENVVAR);

	process_options(&file_list, argc, argv);

	if (file_list == NULL && !isatty(fileno(stdin))) {
		opt.std_in = true;
	}

	/* user specified - on the command line, so let's process stdin */
	if (opt.std_in) {
		int fh;
		input_t input_format = INPUT_NONE;
		bool rewrite;
		bool binary;

		fh = fileno(stdin);

		input_filename = "<standard input>";

		if (*opt.output_filename) {
			strcpy(temp_filename, opt.output_filename);
			output_handle = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC | O_BINARY);
			if (output_handle == -1) {
				error("cannot open %s", temp_filename);
				close(output_handle);
				return errno;
			}
		} else {
			opt.std_out = true;
			output_handle = fileno(stdout);
			strcpy(temp_filename, "<standard ouput>");
		}

		/* since we're processing standard input we should not abort if there
		   is binary data in the stream */
		convert(fh, output_handle, &input_format, &rewrite, &binary, INPUT_BINARY, opt.output_format);

		if (close(fh) == -1)
			error("cannot close %s", input_filename);

		if (close(output_handle) == -1)
			error("cannot close %s", temp_filename);

		return exit_value;
	}

	if (file_list == NULL) {
		filespec_append(&file_list, "*", opt.output_format, opt.include, false);
	}

	if (opt.wildcards_used) {
		_DBG("Calling process_directory(\".\", file_list)");
		process_directory(".", file_list);
	} else {
		List *node;

		node = file_list;
		while (node) {
			bool is_directory = false;
			File *file = (File *) node->data;

			process_file(file->filespec, file->output_format, file->include, true, &is_directory);

			node = node->next;
		}

		if (opt.recursive)
			process_directory(".", file_list);
	}

	filespec_free(file_list);

	_DBG("returning %d", exit_value);
	return exit_value;
}
