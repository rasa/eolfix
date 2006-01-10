# gcc version 3.4.3 (DJGPP version)

# $Id$

ifneq ('$(DEBUG)', '')
CFLAGS+=-O0 -g -D_DEBUG
else
CFLAGS+=-O2 -DNDEBUG
endif

OBJS=eolfix.o fnmatch.o getopt.o 
EXE=eolfix.exe

.PHONY: all clean

all:	$(EXE)

$(EXE): $(OBJS)
	$(LINK.o) -o $@ $^

eolfix.obj:	eolfix.c compiler.h version.h

fnmatch.obj:	fnmatch.c fnmatch.h

getopt.obj:	getopt.c getopt.h

clean:
	rm -f $(EXE) $(OBJS)
