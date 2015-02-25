@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

set debug=%1

call config.cmd

if not exist "%EOLFIX_OUTPUT_DIR%" mkdir "%EOLFIX_OUTPUT_DIR%"

for %%d in (%COMPILERS%) do if not exist "%EOLFIX_OUTPUT_DIR%\%%d" mkdir "%EOLFIX_OUTPUT_DIR%\%%d"

for %%d in (%COMPILERS%) do if exist "%EOLFIX_OUTPUT_DIR%\%%d\eolfix.exe" del "%EOLFIX_OUTPUT_DIR%\%%d\eolfix.exe"

for %%d in (%COMPILERS%) do call %%d.cmd clean

for %%d in (%COMPILERS%) do (
	if exist eolfix.exe del eolfix.exe
	call %%d.cmd clean
	call %%d.cmd
	if not exist eolfix.exe exit /b
	copy /y eolfix.exe "%EOLFIX_OUTPUT_DIR%\%%d"
	del eolfix.exe
)
