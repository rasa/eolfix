@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

set path=%dm_home%\bin;%path%

set _cflags=-DNDEBUG -o
if defined debug set _cflags=-D_DEBUG -g

make -f dm.mk CFLAGS="%_cflags%" %targets%

