@echo off

:: $Id$

call config.cmd

set OUTPUT=output\

for %%d in (%COMPILERS%) do (
	if exist %OUTPUT%%%d\eolfix.exe %OUTPUT%%%d\eolfix.exe --version
)

