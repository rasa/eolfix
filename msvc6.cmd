@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

call "%msvc6_home%\Bin\VCVARS32.BAT"

nmake -f msvc6.mak %targets%

