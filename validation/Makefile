#
# Makefile for testing the libmusicxml library
#

system := $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))

version	 ?= $(shell cat libmusicxmlversion.txt)
xmlfiles  = $(shell find ../files -name "*.xml")

readout	:= $(patsubst ../%.xml, $(version)/read/%.xml, $(xmlfiles))
gmnout	:= $(patsubst ../%.xml, $(version)/gmn/%.gmn, $(xmlfiles))
svgout	:= $(patsubst ../%.xml, $(version)/svg/%.svg, $(xmlfiles))

validxml 	    = $(patsubst %.xml, %.outxml, $(readout))
validgmn 	    = $(patsubst %.gmn, %.outgmn, $(gmnout))
validsvg 	    = $(patsubst %.svg, %.outsvg, $(svgout))

ifeq ($(system), MINGW)
 XML2GUIDO ?= ./xml2guido.exe
 XMLREAD   ?= ./xmlread.exe
else
 XML2GUIDO ?= xml2guido
 XMLREAD   ?= xmlread
endif
WINTOOLS := xml2guido.exe xmlread.exe
WINPATH  := ../build/win64/release

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
	@echo " 'gmn2svg'  : converts the output of guido target to svg"
	@echo "            Output files are written to a VERSION folder, "
	@echo "            where VERSION is taken from the libmusicxmlversion.txt file"
	@echo " 'validate VERSION=another_version': compares the current version output (xml and gmn files)" 
	@echo "              with the one in VERSION folder"
	@echo "             (on Windows, Firefox has to be installed (and opened) in C:/Program Files (x86)/Mozilla Firefox/firefox,"
	@echo "              otherwise please change the Makefile)"
	@echo " 'win64'    : windows specific target: copy the exe and dll files to the validation folder"

#########################################################################
read: 
	@which $(XMLREAD) > /dev/null || (echo "### xmlread (part of samples) must be available from your PATH."; false;)
	make doread

doread: $(readout)

#########################################################################
guido: 
	@which $(XML2GUIDO) > /dev/null || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	make doguido

doguido: $(gmnout)

#########################################################################
gmn2svg: 
	@which guido2svg > /dev/null || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	make guido
	make dogmn2svg

dogmn2svg: $(svgout)

#########################################################################
validate: 
	@echo Validating version $(version) with $(VERSION)
	make validxml
	make validgmn

validxml: $(validxml)

validgmn: $(validgmn)


#########################################################################
# rules for xmlread
$(version)/read/%.xml: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XMLREAD) $<  > $@ || (rm $@ ; false; )

#########################################################################
# rules for xml2guido
$(version)/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2GUIDO) $<  > $@ || (rm $@ ; false; )

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

#########################################################################
# windows specific stuff
win64: $(WINTOOLS) libmusicxml2.dll

xml2guido.exe: $(WINPATH)/xml2guido.exe
	cp $(WINPATH)/xml2guido.exe .
	
xmlread.exe: $(WINPATH)/xmlread.exe
	cp $(WINPATH)/xmlread.exe .

libmusicxml2.dll: $(WINPATH)/libmusicxml2.dll
	cp $(WINPATH)/libmusicxml2.dll .
