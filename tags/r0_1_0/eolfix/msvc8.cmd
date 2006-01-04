@echo off
echo on

:: $Id$

call config.cmd
echo on

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

set include=
set lib=
call "%VS80COMNTOOLS%\vsvars32.bat"

nmake -f msvc8.mak %targets%

set path=%path_save%
set include=%include_save%
set lib=%lib_save%
