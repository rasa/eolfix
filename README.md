# eolfix

eolfix is a command line utility to report, and optionally convert, 
the line endings of text files between MS-DOS, Unix and Macintosh formats.
It detects and ignores binary files unless instructed to include them.
It does not modify files if the file is already in the requested format.
It properly handles "mixed" format files (files having more than one type of line ending format).

## Usage

````
Usage: eolfix [options] [files...]
Options:
-u | --unix    [files] Convert text files to have UNIX line endings
-d | --dos     [files] Convert text files to have DOS/Windows line endings
-m | --mac     [files] Convert text files to have Macintosh line endings
-n | --native  [files] Convert text files to have DOS/Windows line endings
-l | --leave   [files] Report on the type of file, but do not convert (default)
-s | --skip    [files] Skip these files entirely
-i | --include type    Include types: unix, dos, mac, varied, binary and all
                       Default is to include unix, dos, mac and varied files
-e | --exclude type    Exclude types: unix, dos, mac, varied, binary and all
-a | --abort           Abort processing on the first error encountered
-b | --backup          Backup each file before converting it
-k | --backup-ext .ext Use <.ext> for backup file extension (default is .bak)
-c | --compress        Compress doubled-spaced files to be single-spaced (todo)
-g | --config file     Process options in <file>
-y | --dry-run         Skip conversion, but show what would have been converted
-f | --force           Convert every file, even if no change would result
-z | --ignore-case     Perform case-insensitive filename matching
-j | --in-place        Convert text files in-place (slower)
-o | --output file     Send output to <file> (use - for stdout)
-p | --preserve        Preserve file ownership and times
-r | --recursive       Recurse into directories
-t | --temp-dir dir    Use <dir> for temporary files (default is file's dir)
-v | --verbose         Report on every file, even if it would not be changed
-q | --quiet           Only report errors
-? | --help            Show this help message and quit
     --version         Display version information and quit
     --license         Display licensing information
     --                Stop processing options
````

## Contributing

To contribute to this project, please see [CONTRIBUTING.md](CONTRIBUTING.md).

## Bugs

To view existing bugs, or report a new bug, please see the [issues](/issues) page for this project.

## Supported Platforms

* FreeBSD
* Linux
* MS-DOS
* NetBSD
* OpenBSD
* Solaris
* Windows (Cygwin)
* Windows (Native)

## Supported Compilers

* Borland C++ 5.5.1 for Win32
* Digital Mars Compiler Version 8.42n
* Digital Mars Compiler Version 8.57
* gcc version 3.4.3 (DJGPP version)
* gcc version 3.4.2 (mingw-special)
* gcc (Cygwin)
* gcc (Linux)
* Intel(R) C++ Compiler for 32-bit applications, Version 9.0
* Intel(R) Parallel Studio XE 2015 Update 2 Composer Edition (package 179)
* Logiciels/Informatique lcc-win32 version 3.8
* Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 12.00.8804 for 80x86 (aka MSVC 6)
* Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 13.10.3077 for 80x86 (aka MSVC 7 / Visual Studio .NET 2002/3)
* Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 14.00.50727.762 for 80x86 (aka MSVC 8 / Visual Studio 2005)
* Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 15.00.30729.01 for 80x86 (aka MSVC 9 / Visual Studio 2008)
* Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 16.00.30319.01 for 80x86 (aka MSVC 10 / Visual Studio 2010)
* Microsoft (R) C/C++ Optimizing Compiler Version 17.00.50727.1 for x86 (aka MSVC 11 / Visual Studio 2012)
* Microsoft (R) C/C++ Optimizing Compiler Version 18.00.31101 for x86 (aka MSVC 12 / Visual Studio 2013)
* Watcom C/C++ CL Clone for 386  Version 1.3
* Open Watcom C/C++ NMAKE Clone for 386  Version 1.9

## License

This project is [MIT licensed](LICENSE).

## Changelog

Please see [CHANGELOG.md](CHANGELOG.md) for the version history for this project.

## Contact

This project was originally developed by [Ross Smith II](mailto:ross@smithii.com).
Any enhancements and suggestions are welcome.

