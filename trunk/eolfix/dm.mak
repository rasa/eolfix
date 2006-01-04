# Digital Mars Compiler Version 8.42n

# $Id$

OBJS=eolfix.obj fnmatch.obj getopt.obj
EXE=eolfix.exe

CC=dmc.exe
# debug mode:
#CFLAGS=-c -D_DEBUG -g
# release mode:
#CFLAGS=-c -DNDEBUG -o

.c.obj:
	$(CC) $< -c $(CFLAGS)

.PHONY: all clean

all:	$(EXE)

$(EXE): $(OBJS)
	$(CC) $** -o$@

eolfix.obj:	eolfix.c compiler.h version.h

fnmatch.obj:	fnmatch.c fnmatch.h

getopt.obj:	getopt.c getopt.h

clean:
	-del $(EXE) $(OBJS) *.map >nul 2>nul
