# Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 12.00.8804 for 80x86
# aka Visual Studio 6.0

OBJS=eolfix.obj fnmatch.obj getopt.obj
EXE=eolfix.exe

LINK32=link.exe
CFLAGS=/nologo /W4 /GX /YX /FD /c /D "WIN32" /D "_CONSOLE" /D "_MBCS"
LINK32_FLAGS=/nologo /subsystem:console /machine:I386 binmode.obj # setargv.obj

!if defined(debug) || defined(DEBUG)
CFLAGS=$(CFLAGS) /MLd /Od /D "_DEBUG" /Gm /ZI /GZ
LINK32_FLAGS=$(LINK32_FLAGS) /incremental:yes /debug
!else
CFLAGS=$(CFLAGS) /ML  /O2 /D "NDEBUG"                                                  
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
