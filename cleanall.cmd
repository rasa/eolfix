@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

for %%d in (%COMPILERS%) do (
	echo %%d
	call %%d.cmd clean
	if exist "%EOLFIX_OUTPUT_DIR%\%%d" rmdir /s /q "%EOLFIX_OUTPUT_DIR%\%%d"
)
