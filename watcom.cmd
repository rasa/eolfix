@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

if exist "%watcom_home%\setvars.bat" call "%watcom_home%\setvars.bat"
if exist "%watcom_home%\owsetenv.bat" call "%watcom_home%\owsetenv.bat"

nmake -f watcom.mak %targets%

