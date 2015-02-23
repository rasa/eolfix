# Open Watcom C/C++ CL Clone for 386  Version 1.3

OBJS=eolfix.obj fnmatch.obj getopt.obj # wildargv.obj
EXE=eolfix.exe
LINK=link.exe

CFLAGS=/nologo /W3 /GX /YX /FD /c /D "WIN32" /D "_CONSOLE" /D "_MBCS"
LINK32_FLAGS=/nologo /subsystem:console

!if defined(debug) || defined(DEBUG)
CFLAGS=$(CFLAGS) /MLd /Od /D "_DEBUG"
LINK32_FLAGS=$(LINK32_FLAGS) /incremental:yes /debug
!else
CFLAGS=$(CFLAGS) /ML  /O2 /D "NDEBUG"                                                                   
LINK32_FLAGS=$(LINK32_FLAGS) /incremental:no 
!endif

all:	$(EXE) clean

$(EXE):	$(OBJS)
	$(LINK) /out:$@ $** $(LINK32_FLAGS)

eolfix.obj: eolfix.c compiler.h version.h 

fnmatch.obj: fnmatch.c fnmatch.h

getopt.obj: getopt.c getopt.h

#wildargv.obj: wildargv.c
#
#wildargv.c:
#	copy \watcom\src\startup\wildargv.c .
	
clean:
	-del $(EXE) $(OBJS) *.ilk *.pch wildargv.c >nul 2>nul
