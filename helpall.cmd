@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set OUTPUT=help\

echo.>empty.mak

mkdir %OUTPUT% >nul 2>nul

set path=%bcc_home%\bin;%path_save%
set cpp_exe=bcc32
set link_exe=ilink32
set make=make

set compiler=bcc55
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

set path=%cygwin_home%\bin;%path_save%
set cpp_exe=gcc --help
set link_exe=gcc --help
set make=make

set compiler=cygwin
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

set path=%djgpp_home%\bin;%path%
set cpp_exe=gcc --help
set link_exe=gcc --help
set make=make

set compiler=djgpp
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

set path=%dm_home%\bin;%path%
set cpp_exe=dmc
set link_exe=link -?
set make=make

set compiler=dm
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
::%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
make -l -d -f dm.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

call "%intel_home%\Bin\iclvars.bat"
set cpp_exe=icl /?
set link_exe=xilink /?
set make=nmake

set compiler=intel
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

set path=%lcc_home%\bin;%path%
set cpp_exe=lcc.exe
set link_exe=lcclnk
set make=make

set compiler=lcc
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

set path=%mingw_home%\bin;%msys_home%\bin;%path%
set cpp_exe=gcc --help
set link_exe=gcc --help
set make=make

set compiler=mingw
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

call "%msvc6_home%\Bin\VCVARS32.BAT"
set cpp_exe=cl /?
set link_exe=link /?
set make=nmake

set compiler=msvc6
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

call "%msvc7_home%\vcvars32.bat"
set cpp_exe=cl /?
set link_exe=link /?
set make=nmake

set compiler=msvc7
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

call "%VS80COMNTOOLS%\vsvars32.bat"
set cpp_exe=cl /?
set link_exe=link /?
set make=nmake

set compiler=msvc8
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

call %watcom_home%\setvars.bat
set cpp_exe=cl /?
set link_exe=link
set make=nmake

set compiler=watcom
echo make: %make% -? help\%compiler%_make.txt
%make% -? >help\%compiler%_make.txt 2>&1
echo make: %make% -p -f empty.mak help\%compiler%_makerules.txt
%make% -p -f empty.mak >help\%compiler%_makerules.txt 2>&1
echo cpp : %cpp_exe% help\%compiler%_cpp.txt
%cpp_exe% >help\%compiler%_cpp.txt 2>&1
echo link: %link_exe% help\%compiler%_link.txt
%link_exe% >help\%compiler%_link.txt 2>&1

:eof
del empty.mak
set path=%path_save%
set include=%include_save%
set lib=%lib_save%
