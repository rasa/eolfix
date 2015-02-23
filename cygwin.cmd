@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=eolfix.exe

title %~n0

set path=%cygwin_home%\bin;%path%

if defined debug set configure_opts=--enable-debug

if exist config.h goto make

if exist configure goto configure

:autogen
bash -c "./autogen.sh"

:configure
bash -c "./configure %configure_opts%"

:make
bash -c "make %targets%"

