@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all
	
title %~n0

set path=%lcc_home%\bin;%path%

set _cflags=-A -O -DNDEBUG
if not "%debug%" == "" set _cflags=-A -g5 -D_DEBUG

make -f lcc.mak CFLAGS="%_cflags%" %targets%

set path=%path_save%
