#
# Makefile for testing the libmusicxml library
#

system	 := $(shell uname -s)

version	 ?= $(shell cat libmusicxmlversion.txt)
xmlfiles  = $(shell find ../files -name "*.xml")

readout	:= $(patsubst ../%.xml, $(version)/read/%.xml, $(xmlfiles))
gmnout	:= $(patsubst ../%.xml, $(version)/gmn/%.gmn, $(xmlfiles))
svgout	:= $(patsubst ../%.xml, $(version)/svg/%.svg, $(gmnout))

.PHONY: read guido

all:
	make read
	make guido

test:
	echo $(gmnout)

help:
	@echo "Makefile for testing the library output."
	@echo "Testing is based on the xml files located in the 'files' folder." 
	@echo "Available targets are:"
	@echo " 'all' (default): call the read and guido targets."
	@echo " 'read'     : reads the set of xml files and writes the corresponding output"
	@echo " 'guido'    : converts the set of xml files to guido"
	@echo " 'svg'      : converts the output of guido target to svg"
	@echo "Output files are written to a VERSION folder, "
	@echo "where VERSION is taken from the libmusicxmlversion.txt file"

#########################################################################
read: 
	@which xmlread || (echo "### xmlread (part of samples) must be available from your PATH."; false;)
	make doread

doread: $(readout)

#########################################################################
guido: 
	@which xml2guido || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	make doguido

doguido: $(gmnout)

#########################################################################
svg: 
	@which guido2svg || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	make guido
	make dosvg

dosvg: $(gmnout)

#########################################################################
# rules for xmlread
$(version)/read/%.xml: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	-xmlread $<  > $@ || rm $@

#########################################################################
# rules for xml2guido
$(version)/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	-xml2guido $<  > $@ || rm $@

#########################################################################
# rules for guido2svg
$(version)/svg/%.svg: $(version)/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	-guido2svg $<  > $@ || rm $@

