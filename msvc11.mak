# Microsoft (R) C/C++ Optimizing Compiler Version 17.00.50727.1 for x86
# aka Visual Studio 2012

OBJS=eolfix.obj fnmatch.obj getopt.obj
EXE=eolfix.exe

LINK32=link.exe
CFLAGS=/nologo /W3 /EHsc /FD /c /D "WIN32" /D "_CONSOLE" /D "_MBCS" /wd4996 /wd4710 /wd4711
LINK32_FLAGS=/nologo /subsystem:console /machine:I386 # setargv.obj

!if defined(debug) || defined(DEBUG)
CFLAGS=$(CFLAGS) /Od /D "_DEBUG" /Gm /ZI /RTC1
LINK32_FLAGS=$(LINK32_FLAGS) /incremental:yes /debug
!else
CFLAGS=$(CFLAGS) /O2 /D "NDEBUG"                                                  
LINK32_FLAGS=$(LINK32_FLAGS) /incremental:no
!endif

.PHONY:	all clean

all:	$(EXE)

$(EXE):	$(OBJS)
	$(LINK32) /out:$@ $** $(LINK32_FLAGS)

eolfix.obj: eolfix.c compiler.h version.h

fnmatch.obj: fnmatch.c fnmatch.h

getopt.obj: getopt.c getopt.h

clean:
	-del $(EXE) $(OBJS) *.idb *.ilk *.ncb *.opt *.pch *.pdb *.plg >nul 2>nul
