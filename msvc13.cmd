@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

call "%VS130COMNTOOLS%\vsvars32.bat"

nmake -f msvc13.mak %targets%

