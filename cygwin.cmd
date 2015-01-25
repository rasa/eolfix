@echo off

:: $Id$

call config.cmd

set targets=%*
if "%targets%" == "" set targets=eolfix.exe

title %~n0

set path=%cygwin_home%\bin;%path%

::for %%i in (%targets%) do bash -c "make %%i"

if not "%debug%" == "" goto debug

if not exist cygwin.debug goto make
del cygwin.debug 
goto configure

:debug
if exist cygwin.debug goto make
set configure_opts=--enable-debug
echo %configure_opts% >cygwin.debug
goto configure

:configure
bash -c "./configure %configure_opts%"

:make
bash -c "make %targets%"

set path=%path_save%
