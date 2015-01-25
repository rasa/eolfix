# Process this file with automake to produce Makefile.in

XSLTPROC	?=xsltproc --xinclude --nonet

FOP_HOME	?=c:/fop-0.20.5
HHC_HOME	?=c:/progra~1/htmlhe~1
JAVA_HOME	?=c:/j2sdk1.4.2_08

XSL_DIR		?=/usr/share/xml/docbook/stylesheet/docbook-xsl

# this works for Cygwin
# todo: need to support other platforms
#XSL_DIR		?=/usr/share/docbook-xsl
MANPAGES_XSL	?=$(XSL_DIR)/manpages/docbook.xsl
HTML_XSL	?=$(XSL_DIR)/html/docbook.xsl
HTMLHELP_XSL ?=$(XSL_DIR)/htmlhelp/htmlhelp.xsl
FO_XSL		?=$(XSL_DIR)/fo/docbook.xsl
IMAGES_DIR	?=$(XSL_DIR)/images

HHC		?="$(HHC_HOME)/hhc.exe"
FOP		?="$(FOP_HOME)/fop.sh"

FOP=/usr/bin/fop

PACKAGE_UG	=$(PACKAGE)-ug

man_MANS	= $(PACKAGE).1 $(PACKAGE_UG).html $(PACKAGE_UG).txt $(PACKAGE_UG).pdf $(PACKAGE_UG).chm

EXTRA_DIST	= $(man_MANS) *.xml *.xsl

all: $(man_MANS)

gfx:
	mkdir -p $@
	cp -pr $(IMAGES_DIR)/* $@
	touch $@

$(PACKAGE_UG).html:	$(PACKAGE_UG).xml global.ent help.xml gfx
	-$(XSLTPROC) --output $@ html1.xsl $<
	perl -pi.bak -e 's|<head>|<head><link rel="stylesheet" href="$(PACKAGE).css" type="text/css">|i' $@

$(PACKAGE).1:	$(PACKAGE)-man.xml global.ent help.xml gfx
	 -$(XSLTPROC) --output $@ $(MANPAGES_XSL) $<

man:	$(PACKAGE).1

EOLFIX ?= $(shell which $(PACKAGE) 2>/dev/null || test -x ../$(PACKAGE) && echo ../$(PACKAGE))

$(if $(EOLFIX),,$(error $(EOLFIX)$(EXEEXT) not found, type 'cd doc/.. && make $(PACKAGE)$(EXEEXT)' to generate))

$(PACKAGE).out:	$(EOLFIX)
	-$(EOLFIX) --help >$@.tmp
	mv -f $@.tmp $@

help.xml:	$(PACKAGE).out
	echo "<![CDATA[" >$@
	cat $< >>$@
	echo >>$@
	echo "]]>" >>$@

htmlhelp.hhp:	$(PACKAGE_UG).xml global.ent help.xml gfx
	$(XSLTPROC) $(HTMLHELP_XSL) $<

$(PACKAGE_UG).chm:	htmlhelp.hhp
	test ! -f $@ || rm $@
	-$(HHC) $<
	test -s $@

chm:	$(PACKAGE_UG).chm

html:	$(PACKAGE_UG).html

$(PACKAGE_UG).fo:	$(PACKAGE_UG).xml global.ent help.xml gfx
	$(XSLTPROC) --output $@ $(FO_XSL) $<

$(PACKAGE_UG).pdf:	$(PACKAGE_UG).fo
	$(FOP) -fo $< -pdf $@

pdf:	$(PACKAGE_UG).pdf

$(PACKAGE_UG).txt:	html
	lynx -nolist -dump "file:`pwd`/$(PACKAGE_UG).html" >$@

txt:	$(PACKAGE_UG).txt

text:	txt

clean:
	rm -fr \
	gfx \
	help.xml \
	*.chm \
	*.fo \
	*.hhc \
	*.hhp \
	*.html \
	*.out \
	*.pdf \
	*.tmp \
	*.txt

.PHONY:	all chm clean html man pdf txt text
