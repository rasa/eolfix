@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all
	
title %~n0

set path=%mingw_home%\bin;%msys_home%\bin;%path%

make -f mingw.mak %targets%

set path=%path_save%
