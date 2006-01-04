#!/bin/sh

# $Id$

VERSION=0.1.0

FILES="\
AUTHORS \
COPYING \
ChangeLog \
NEWS \
README \
Release/eolfix.exe \
doc/*.1 \
doc/*.chm \
doc/*.pdf \
doc/*.txt \
doc/eolfix*.html \
"

SRC_FILES="\
${FILES} \
*.c \
*.cmd \
*.dsp \
*.dsw \
*.h \
*.mak \
makedist-win32.sh \
"

# upx Release/eolfix.exe
zip -j -9 eolfix-${VERSION}-win32.zip ${FILES}

rm -f config.h

zip -j -9 eolfix-${VERSION}-src-win32.zip ${SRC_FILES}
