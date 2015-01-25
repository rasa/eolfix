@echo off

:: $Id$

if "%bcc_home%" == ""		set bcc_home=c:\bcc55
if "%cygwin_home%" == ""	set cygwin_home=c:\cygwin
if "%djgpp_home%" == ""		set djgpp_home=c:\djgpp
if "%dm_home%" == ""		set dm_home=c:\dm
if "%intel_home%" == ""		set intel_home=C:\Program Files\Intel\Compiler\C++\9.0\IA32
if "%lcc_home%" == ""		set lcc_home=c:\lcc
if "%mingw_home%" == ""		set mingw_home=c:\mingw
if "%msys_home%" == ""		set msys_home=c:\msys\1.0
if "%msvc6_home%" == ""		set msvc6_home=C:\Program Files\Microsoft Visual Studio\VC98
if "%msvc7_home%" == ""		set msvc7_home=C:\Program Files\Microsoft Visual C++ Toolkit 2003
if "%VS80COMNTOOLS%" == ""	set VS80COMNTOOLS==C:\Program Files\Microsoft SDK
if "%watcom_home%" == ""	set watcom_home=c:\watcom

set COMPILERS=

if exist "%bcc_home%"		set COMPILERS=%COMPILERS% bcc55
if exist "%cygwin_home%"	set COMPILERS=%COMPILERS% cygwin
if exist "%djgpp_home%"		set COMPILERS=%COMPILERS% djgpp
if exist "%dm_home%"		set COMPILERS=%COMPILERS% dm
if exist "%intel_home%"		set COMPILERS=%COMPILERS% intel
if exist "%lcc_home%"		set COMPILERS=%COMPILERS% lcc
if exist "%mingw_home%"		set COMPILERS=%COMPILERS% mingw
if exist "%msvc6_home%"		set COMPILERS=%COMPILERS% msvc6
if exist "%msvc7_home%"		set COMPILERS=%COMPILERS% msvc7
if exist "%VS80COMNTOOLS%"	set COMPILERS=%COMPILERS% msvc8
if exist "%watcom_home%"	set COMPILERS=%COMPILERS% watcom

::if "%1" == "dontsave" goto :eof

set path_save=%path%
set include_save=%include%
set lib_save=%lib%
