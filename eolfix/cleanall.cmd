@echo off

:: $Id$

call config.cmd

set OUTPUT=output\

del cygwin.debug 

for %%d in (%COMPILERS%) do (
	echo %%d
	call %%d.cmd clean
)
