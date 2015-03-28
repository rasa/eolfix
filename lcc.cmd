@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all
	
title %~n0

set path=%lcc_home%\bin;%path%

set _cflags=-A -O -DNDEBUG
if defined debug set _cflags=-A -g5 -D_DEBUG

make -f lcc.mk CFLAGS="%_cflags%" %targets%

