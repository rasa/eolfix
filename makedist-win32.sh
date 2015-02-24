#!/usr/bin/env bash

# Copyright (c) 2005-2015 Ross Smith II. MIT Licensed.

# Cygwin shell script to create Win32 binary & source distributions

VERSION="$(grep PACKAGE_VERSION version.h | head -n 1 | cut -d'"' -f 2)"

FILES="\
AUTHORS \
LICENSE \
CHANGELOG.md \
CONTRIBUTING.md \
README.md \
eolfix.exe \
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
-x config.h \
"

ZIP="zip -j -9"

EOLFIX_ZIP="eolfix-${VERSION}-win32.zip"
EOLFIX_SRC_ZIP="eolfix-${VERSION}-src-win32.zip"

rm -f "${EOLFIX_ZIP}" "${EOLFIX_SRC_ZIP}"

# upx Release/eolfix.exe
${ZIP} "${EOLFIX_ZIP}" ${FILES}

# test -f config.h && mv -f config.h config.h.sav

${ZIP} "${EOLFIX_SRC_ZIP}" ${SRC_FILES}

# test -f config.h.sav && mv -f config.h.sav config.h
