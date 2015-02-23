@setlocal EnableDelayedExpansion EnableExtensions
@if defined EOLFIX_DEBUG (@echo on) else (@echo off)

call config.cmd

set targets=%*
if not defined targets set targets=clean all

title %~n0

set path=%intel_home%\bin;%path%

if exist "%VS130COMNTOOLS%\vsvars32.bat" call "%VS130COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2015
if not defined TARGET_VS if exist "%VS120COMNTOOLS%\vsvars32.bat" call "%VS120COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2013
if not defined TARGET_VS if exist "%VS110COMNTOOLS%\vsvars32.bat" call "%VS110COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2012shell
if not defined TARGET_VS if exist "%VS100COMNTOOLS%\vsvars32.bat" call "%VS100COMNTOOLS%\vsvars32.bat" & set TARGET_VS=vs2010shell
if not defined TARGET_VS if exist "%VS90COMNTOOLS%\vsvars32.bat"  call "%VS90COMNTOOLS%\vsvars32.bat"  & set TARGET_VS=vs2008shell
if not defined TARGET_VS if exist "%VS80COMNTOOLS%\vsvars32.bat"  call "%VS80COMNTOOLS%\vsvars32.bat"  & set TARGET_VS=vs2005shell
if not defined TARGET_VS if exist "%msvc6_home%\Bin\VCVARS32.BAT" call "%msvc6_home%\Bin\VCVARS32.BAT" & set TARGET_VS=vs6shell

set TARGET_ARCH=ia32
::set TARGET_ARCH=ia32_intel64
::set TARGET_ARCH=intel64

call "%intel_home%\Bin\iclvars.bat" %TARGET_ARCH% %TARGET_VS%

nmake -f intel.mak %targets%

