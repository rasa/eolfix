# Borland C++ 5.5.1 for Win32

OBJS=eolfix.obj fnmatch.obj getopt.obj 
EXE=eolfix.exe

CFLAGS	=-I$(MAKEDIR)\..\Include
LDFLAGS	=-L$(MAKEDIR)\..\Lib # $(MAKEDIR)\..\Lib\wildargs.obj

!if '$(windir)' != ''
CC	=bcc32
CFLAGS	=$(CFLAGS) -tWC -WC
LDFLAGS	=$(LDFLAGS) -tWC -WC
!else
CC	=bcc
CFLAGS	=$(CFLAGS) -ms
LDFLAGS	=$(LDFLAGS) -ms
!endif

!if '$(DEBUG)' != ''
CFLAGS	=$(CFLAGS) -D_DEBUG -Ox -Oa 
!else
CFLAGS	=$(CFLAGS) -DNDEBUG
!endif

all:	$(EXE)

$(EXE):	$(OBJS)
	$(CC) -e$@ $(LDFLAGS) $**

eolfix.obj: eolfix.c compiler.h version.h

fnmatch.obj: fnmatch.c fnmatch.h

getopt.obj: getopt.c getopt.h

clean:
	-del $(EXE) $(OBJS) *.tds >nul 2>nul
