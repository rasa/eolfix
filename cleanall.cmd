@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set OUTPUT=output\

for %%d in (%COMPILERS%) do (
	echo %%d
	call %%d.cmd clean
)
