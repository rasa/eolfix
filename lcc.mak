# Logiciels/Informatique lcc-win32 version 3.8. Compilation date: Aug 14 2005 09:28:06

# $Id$

OBJS=eolfix.obj fnmatch.obj # getopt.obj 
EXE=eolfix.exe

LINK=$(LCCROOT)\bin\lcclnk.exe
# debug mode:
#CFLAGS=-A -g5 -D_DEBUG
# release mode:
#CFLAGS=-A -O -DNDEBUG
LDFLAGS=-subsystem console -s
LINK.o=$(LINK) $(LDFLAGS)

.PHONY: all clean

all:	$(EXE)

$(EXE):	$(OBJS)
	$(LINK.o) -o $@ $(OBJS)

eolfix.obj: eolfix.c compiler.h version.h

fnmatch.obj: fnmatch.c fnmatch.h

#getopt.obj: getopt.c getopt.h

clean:
	-del $(EXE) $(OBJS) >nul 2>nul
