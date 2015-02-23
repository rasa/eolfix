@echo off

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

set path=%djgpp_home%\bin;%path%
set djgpp=%djgpp_home%\djgpp.env

make -f djgpp.mak %targets%

