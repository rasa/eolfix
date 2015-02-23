@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

call "%VS80COMNTOOLS%\vsvars32.bat"

set include=%include%;%_ProgramFiles%\Microsoft SDKs\Windows\v7.1A\Include
set include=%include%;%_ProgramFiles%\Microsoft SDKs\Windows\v7.0A\Include
set lib=%lib%;%_ProgramFiles%\Microsoft SDKs\Windows\v7.1A\Lib
set lib=%lib%;%_ProgramFiles%\Microsoft SDKs\Windows\v7.0A\Lib

nmake -f msvc8.mak %targets%

