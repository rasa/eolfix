@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

set path=%bcc_home%\bin;%path%

make -f bcc55.mak %targets% 

