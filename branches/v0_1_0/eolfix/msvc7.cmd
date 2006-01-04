@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

::set include=
::set lib=
call "%msvc7_home%\vcvars32.bat"

nmake -f msvc7.mak %targets%

set path=%path_save%
set include=%include_save%
set lib=%lib_save%
