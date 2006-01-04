@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

set path=%intel_home%\bin;%path%
set include=
set lib=
call "%msvc6_home%\Bin\VCVARS32.BAT"
call "%intel_home%\Bin\iclvars.bat"

nmake -f intel.mak %targets%

set path=%path_save%
set include=%include_save%
set lib=%lib_save%
