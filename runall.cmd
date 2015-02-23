@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set OUTPUT=output

for %%d in (%COMPILERS%) do (
	if exist %OUTPUT%\%%d\eolfix.exe %OUTPUT%\%%d\eolfix.exe --version
)

