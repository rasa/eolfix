# Intel(R) C++ Compiler for 32-bit applications, Version 9.0

# $Id$

OBJS=eolfix.obj fnmatch.obj getopt.obj
EXE=eolfix.exe

CC=icl.exe 
CPP=icl.exe 
LINK32=xilink.exe 

CFLAGS=/nologo /W3 /GX /YX /FD /c /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Wall

LINK32_FLAGS=/nologo /subsystem:console /machine:I386 # setargv.obj

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
	-del $(EXE) $(OBJS) *.pdb *.ilk >nul 2>nul
