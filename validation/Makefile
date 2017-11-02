#
# Makefile for testing the libmusicxml library
#

system	 := $(shell uname -s)

version	 ?= $(shell cat libmusicxmlversion.txt)
xmlfiles  = $(shell find ../files -name "*.xml")

readout	:= $(patsubst ../%.xml, $(version)/read/%.xml, $(xmlfiles))
gmnout	:= $(patsubst ../%.xml, $(version)/gmn/%.gmn, $(xmlfiles))
lilyout	:= $(patsubst ../%.xml, $(version)/lilypond/%.ly, $(xmlfiles))
guidosvg := $(patsubst ../%.xml, $(version)/guidosvg/%.svg, $(xmlfiles))
lilysvg  := $(patsubst ../%.xml, $(version)/lilysvg/%.svg, $(xmlfiles))

validxml 	    = $(patsubst %.xml, %.outxml, $(readout))
validgmn 	    = $(patsubst %.gmn, %.outgmn, $(gmnout))
validguidosvg 	= $(patsubst %.svg, %.outsvg, $(guidosvg))
validlilysvg 	= $(patsubst %.svg, %.outsvg, $(guidosvg))

XML2GUIDO ?= xml2guido
XML2LILY  ?= ../cmake/macos/Release/xml2lilypond

.PHONY: read guido

all:
	make read
	make guido

trace:
	@echo $(validxml)

help:
	@echo "Makefile for testing the library output."
	@echo "Testing is based on the xml files located in the 'files' folder." 
	@echo "Available targets are:"
	@echo " 'all' (default): call the read and guido targets."
	@echo " 'read'     : reads the set of xml files and writes the corresponding output"
	@echo " 'guido'    : converts the set of xml files to guido"
	@echo " 'lily'     : converts the set of xml files to lilypond"
	@echo " 'guidosvg' : converts the output of guido target to svg"
	@echo " 'lilysvg'  : converts the output of lily target to svg"
	@echo "Output files are written to a VERSION folder, "
	@echo "where VERSION is taken from the libmusicxmlversion.txt file"
	@echo " 'validate VERSION=another_version': compares the current version output (xml and gmn files) with the one in VERSION folder (on Windows, Firefox has to be installed (and opened) in C:/Program Files (x86)/Mozilla Firefox/firefox, otherwise please change Makefile)"

#########################################################################
read: 
	@which xmlread || (echo "### xmlread (part of samples) must be available from your PATH."; false;)
	make doread

doread: $(readout)

#########################################################################
guido: 
	@which $(XML2GUIDO) || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	make doguido

doguido: $(gmnout)

#########################################################################
lily: 
	@which $(XML2LILY) || (echo "### xml2lilypond (part of samples) must be available from your PATH."; false;)
	make dolily

dolily: $(lilyout)

#########################################################################
guidosvg: 
	@which guido2svg || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	make guido
	make doguidosvg

doguidosvg: $(guidosvg)

#########################################################################
lilysvg: 
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	make lily
	make dolilysvg

dolilysvg: $(lilysvg)

#########################################################################
validate: 
	@echo Validating version $(version) with $(VERSION) $(tmp)
	make validxml
	make validgmn

validxml: $(validxml)

validgmn: $(validgmn)


#########################################################################
# rules for xmlread
$(version)/read/%.xml: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	-xmlread $<  > $@ || rm $@

#########################################################################
# rules for xml2guido
$(version)/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	-$(XML2GUIDO) $<  > $@ || rm $@

#########################################################################
# rules for guido2svg
$(version)/guidosvg/%.svg: $(version)/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	-guido2svg $<  > $@ || rm $@

#########################################################################
# rules for xml2lilypond
$(version)/lilypond/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	-$(XML2LILY) $<  > $@ || rm $@

#########################################################################
# rules for lilypond with svg backend
$(version)/lilysvg/%.svg: $(version)/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	-lilypond -dbackend=svg $<  > $@ || rm $@

#########################################################################
# rules for xml output validation
%.outxml: %.xml
	$(eval tmp := $(patsubst $(version)/%, $(VERSION)/%, $<))	
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(version)/%, $(VERSION)/%, $<) # to check changes"; true)

%.outgmn: %.gmn
	$(eval tmp := $(patsubst $(version)/%, $(VERSION)/%, $<))	
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(version)/%, $(VERSION)/%, $<) # to check changes"; true)
