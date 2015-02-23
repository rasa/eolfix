@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

set debug=%1

call config.cmd

set OUTPUT=output

if not exist "%OUTPUT%" mkdir "%OUTPUT%"

for %%d in (%COMPILERS%) do if not exist "%OUTPUT%\%%d" mkdir "%OUTPUT%\%%d"

for %%d in (%COMPILERS%) do if exist "%OUTPUT%\%%d\eolfix.exe" del "%OUTPUT%\%%d\eolfix.exe"

for %%d in (%COMPILERS%) do call %%d.cmd clean

for %%d in (%COMPILERS%) do (
	if exist eolfix.exe del eolfix.exe
	call %%d.cmd clean
	call %%d.cmd
	if not exist eolfix.exe exit /b
	copy /y eolfix.exe %output%\%%d
	del eolfix.exe
)
