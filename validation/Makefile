#
# Makefile for testing the libmusicxml library
#

system	 := $(shell uname -s)

version	 ?= $(shell cat libmusicxmlversion.txt)
xmlfiles  = $(shell find ../files -name "*.xml")

readout	:= $(patsubst ../%.xml, $(version)/read/%.xml, $(xmlfiles))
gmnout	:= $(patsubst ../%.xml, $(version)/gmn/%.gmn, $(xmlfiles))
svgout	:= $(patsubst ../%.xml, $(version)/svg/%.svg, $(xmlfiles))

validxml 	    = $(patsubst %.xml, %.outxml, $(readout))
validgmn 	    = $(patsubst %.gmn, %.outgmn, $(gmnout))
validsvg 	    = $(patsubst %.svg, %.outsvg, $(svgout))

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
	@echo " 'svg'      : converts the output of guido target to svg"
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
	@which xml2guido || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	make doguido

doguido: $(gmnout)

#########################################################################
svg: 
	@which guido2svg || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	make guido
	make dosvg

dosvg: $(svgout)

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
	xmlread $<  > $@ || (rm $@ ; false; )

#########################################################################
# rules for xml2guido
$(version)/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	xml2guido $<  > $@ || (rm $@ ; false; )

#########################################################################
# rules for guido2svg
$(version)/svg/%.svg: $(version)/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	guido2svg $<  > $@ || (rm $@ ; false; )

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
