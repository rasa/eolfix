@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

for %%d in (%COMPILERS%) do (
	if exist "%EOLFIX_OUTPUT_DIR%\%%d\eolfix.exe" "%EOLFIX_OUTPUT_DIR%\%%d\eolfix.exe" --version
)

