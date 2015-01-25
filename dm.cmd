@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

set path=%dm_home%\bin;%path%
set lib=

set _cflags=-DNDEBUG -o
if not "%debug%" == "" set _cflags=-D_DEBUG -g

make -f dm.mak CFLAGS="%_cflags%" %targets%

set path=%path_save%
set lib=%lib_save%
