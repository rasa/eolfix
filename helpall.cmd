@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set OUTPUT=help\

echo.>empty.mak

mkdir %OUTPUT% >nul 2>nul

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set path=%bcc_home%\bin;%original_path%
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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set path=%cygwin_home%\bin;%original_path%
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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if defined PROCESSOR_ARCHITECTURE if not "%PROCESSOR_ARCHITECTURE%" == "x86" goto :no_djgpp

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

:no_djgpp

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist "%VS140COMNTOOLS%\vsvars32.bat" call "%VS140COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2013
if not defined TARGET_VS if exist "%VS120COMNTOOLS%\vsvars32.bat" call "%VS120COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2013
if not defined TARGET_VS if exist "%VS110COMNTOOLS%\vsvars32.bat" call "%VS110COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2012shell
if not defined TARGET_VS if exist "%VS100COMNTOOLS%\vsvars32.bat" call "%VS100COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2010shell
if not defined TARGET_VS if exist "%VS90COMNTOOLS%\vsvars32.bat"  call "%VS90COMNTOOLS%\vsvars32.bat"  & set TARGET_VS=vs2008shell
if not defined TARGET_VS if exist "%VS80COMNTOOLS%\vsvars32.bat"  call "%VS80COMNTOOLS%\vsvars32.bat"  & set TARGET_VS=vs2005shell
if not defined TARGET_VS if exist "%msvc7_home%\Bin\VCVARS32.BAT" call "%msvc7_home%\Bin\VCVARS32.BAT" & set TARGET_VS=vs2002shell
if not defined TARGET_VS if exist "%msvc6_home%\Bin\VCVARS32.BAT" call "%msvc6_home%\Bin\VCVARS32.BAT" & set TARGET_VS=vs6shell

set TARGET_ARCH=ia32
::set TARGET_ARCH=ia32_intel64
::set TARGET_ARCH=intel64

call "%intel_home%\Bin\iclvars.bat" %TARGET_ARCH% %TARGET_VS%

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist "%watcom_home%\setvars.bat" call "%watcom_home%\setvars.bat"
if exist "%watcom_home%\owsetenv.bat" call "%watcom_home%\owsetenv.bat"

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set cpp_exe=cl /?
set link_exe=link /?
set make=nmake
set vsvars32=

for %%i in (8 9 10 11 12 14) do (
	if "%%i" ==  "8" set vsvars32="%VS80COMNTOOLS%\vsvars32.bat"
	if "%%i" ==  "9" set vsvars32="%VS90COMNTOOLS%\vsvars32.bat"
	if "%%i" == "10" set vsvars32="%VS100COMNTOOLS%\vsvars32.bat"
	if "%%i" == "11" set vsvars32="%VS110COMNTOOLS%\vsvars32.bat"
	if "%%i" == "12" set vsvars32="%VS120COMNTOOLS%\vsvars32.bat"
	if "%%i" == "14" set vsvars32="%VS140COMNTOOLS%\vsvars32.bat"

	if exist !vsvars32! call !vsvars32!

	set compiler=msvc%%i

	echo make: %make% -? help\!compiler!_make.txt
	%make% -? >help\!compiler!_make.txt 2>&1
	echo make: %make% -p -f empty.mak help\!compiler!_makerules.txt
	%make% -p -f empty.mak >help\!compiler!_makerules.txt 2>&1
	echo cpp : %cpp_exe% help\!compiler!_cpp.txt
	echo.|%cpp_exe% >help\!compiler!_cpp.txt 2>&1
	echo link: %link_exe% help\!compiler!_link.txt
	%link_exe% >help\!compiler!_link.txt 2>&1
)

set | findstr COMNTOOLS

:eof
del empty.mak
set path=%original_path%
