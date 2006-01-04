@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

set include=
set lib=
call "%watcom_home%\setvars.bat"
echo include=%include%
echo lib=%lib%

nmake -f watcom.mak %targets%

set path=%path_save%
set include=%include_save%
set lib=%lib_save%
