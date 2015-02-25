@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

set EOLFIX_OUTPUT_DIR=output

set _ProgramFiles=%ProgramFiles(x86)%
if not defined _ProgramFiles set _ProgramFiles=%ProgramFiles%
if not exist "%_ProgramFiles%" set _ProgramFiles=%ProgramFiles%

set original_path=%path%

if not defined bcc_home		set bcc_home=c:\bcc55
if not defined cygwin_home	set cygwin_home=c:\cygwin
if not defined djgpp_home	set djgpp_home=c:\djgpp
if not defined dm_home		set dm_home=c:\dm
if not defined intel_home	if exist      "%_ProgramFiles%\Intel\Compiler\C++\9.0\IA32" set intel_home=%_ProgramFiles%\Intel\Compiler\C++\9.0\IA32
if not defined intel_home	if exist      "%_ProgramFiles%\Intel\Composer XE 2015"      set intel_home=%_ProgramFiles%\Intel\Composer XE 2015
if not defined lcc_home		set lcc_home=c:\lcc
if not defined mingw_home	set mingw_home=c:\mingw
if not defined msys_home	if exist c:\msys\1.0 set msys_home=c:\msys\1.0
if not defined msys_home	if exist %mingw_home%\msys\1.0 set msys_home=%mingw_home%\msys\1.0
if not defined watcom_home	set watcom_home=c:\watcom

if not defined VS80COMNTOOLS	if exist "%_ProgramFiles%\Microsoft SDK" set VS80COMNTOOLS=%_ProgramFiles%\Microsoft SDK
if not defined VS80COMNTOOLS	set VS80COMNTOOLS =%_ProgramFiles%\Microsoft Visual Studio 8\Common7\Tools\
if not defined VS90COMNTOOLS	set VS90COMNTOOLS =%_ProgramFiles%\Microsoft Visual Studio 9.0\Common7\Tools\
if not defined VS100COMNTOOLS	set VS100COMNTOOLS=%_ProgramFiles%\Microsoft Visual Studio 10.0\Common7\Tools
if not defined VS110COMNTOOLS	set VS110COMNTOOLS=%_ProgramFiles%\Microsoft Visual Studio 11.0\Common7\Tools
if not defined VS120COMNTOOLS	set VS120COMNTOOLS=%_ProgramFiles%\Microsoft Visual Studio 12.0\Common7\Tools
if not defined VS130COMNTOOLS	set VS130COMNTOOLS=%_ProgramFiles%\Microsoft Visual Studio 13.0\Common7\Tools

:: Visual Studio 6 / CL version 12.0:
if not defined msvc6_home	set msvc6_home=%_ProgramFiles%\Microsoft Visual Studio\VC98
:: Visual Studio .NET (2002) / CL Version 13.0:
if not defined msvc7_home	set msvc7_home=%_ProgramFiles%\Microsoft Visual C++ Toolkit 2003
:: Visual Studio 2005 / aka VS 8 / CL version 14.0:
if not defined msvc8_home	set msvc8_home=%_ProgramFiles%\Microsoft Visual Studio 8\VC
:: Visual Studio 2008 / aka VS 9.0 / CL version 15.0:
if not defined msvc9_home	set msvc9_home=%_ProgramFiles%\Microsoft Visual Studio 9.0\VC
:: Visual Studio 2010 / aka VS 10.0 / CL version 16.0:
if not defined msvc10_home	set msvc10_home=%_ProgramFiles%\Microsoft Visual Studio 10.0\VC
:: Visual Studio 2012 / aka VS 11.0 / CL version 17.0:
if not defined msvc11_home	set msvc11_home=%_ProgramFiles%\Microsoft Visual Studio 11.0\VC
:: Visual Studio 2013 / aka VS 12.0 / CL version 18.0:
if not defined msvc12_home	set msvc12_home=%_ProgramFiles%\Microsoft Visual Studio 12.0\VC
:: Visual Studio 2015 / aka VS 13.0 / CL version 19.0:
if not defined msvc13_home	set msvc13_home=%_ProgramFiles%\Microsoft Visual Studio 13.0\VC

set COMPILERS=

if exist "%bcc_home%"		set COMPILERS=%COMPILERS% bcc55
if exist "%cygwin_home%"	set COMPILERS=%COMPILERS% cygwin
if not defined PROCESSOR_ARCHITECTURE (
if exist "%djgpp_home%"		set COMPILERS=%COMPILERS% djgpp
)
if "%PROCESSOR_ARCHITECTURE%" == "x86" (
if exist "%djgpp_home%"		set COMPILERS=%COMPILERS% djgpp
)
if exist "%dm_home%"		set COMPILERS=%COMPILERS% dm
::if exist "%intel_home%"		set COMPILERS=%COMPILERS% intel
if exist "%lcc_home%"		set COMPILERS=%COMPILERS% lcc
if exist "%mingw_home%"		set COMPILERS=%COMPILERS% mingw
if exist "%watcom_home%"	set COMPILERS=%COMPILERS% watcom

if exist "%msvc6_home%"		set COMPILERS=%COMPILERS% msvc6
if exist "%msvc7_home%"		set COMPILERS=%COMPILERS% msvc7
if exist "%VS80COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc8
if exist "%VS90COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc9
if exist "%VS100COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc10
if exist "%VS110COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc11
if exist "%VS120COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc12
if exist "%VS140COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc14

::if "%1" == "dontsave" goto :eof

