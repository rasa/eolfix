@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=clean all

title %~n0

set path=%bcc_home%\bin;%path%

make -f bcc55.mak %targets% 

set path=%path_save%
