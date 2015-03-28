@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all
	
title %~n0

set path=%mingw_home%\bin;%msys_home%\bin;%path%

make -f mingw.mk %targets%

