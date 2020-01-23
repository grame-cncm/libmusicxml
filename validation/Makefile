#
# Makefile for testing the libmusicxml library
#

system := $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))

version ?= $(shell cat libmusicxmlversion.txt)

xmlfiles                  = $(shell find ../files -name "*.xml")
xmlfilesforlilyignored = $(shell find ../files -name "*.xml" | grep lilypond-ignored)
xmlfilesforlilyjianpu     = $(shell find ../files -name "*.xml" | grep jianpu)
jianpuincludefilename     = "jianpu10a.ly"
jianpuinclude             = $(shell find ../files -name $(jianpuincludefilename)")
xmlfilesforbrl     		  = $(shell find ../files -name "*.xml" | grep braille)

readout           := $(patsubst ../%.xml, $(version)/read/%.xml, $(xmlfiles))
gmnout            := $(patsubst ../%.xml, $(version)/gmn/%.gmn, $(xmlfiles))
guidosvg          := $(patsubst ../%.xml, $(version)/guidosvg/%.svg, $(xmlfiles))
lilyignoredout    := $(patsubst ../%.xml, $(version)/lilyignored/%.ly, $(xmlfilesforlilyignored))
lilyout           := $(patsubst ../%.xml, $(version)/lilypond/%.ly, $(xmlfiles))
lilypdf           := $(patsubst ../%.xml, $(version)/lilypdf/%.pdf, $(xmlfiles))
lilypng           := $(patsubst ../%.xml, $(version)/lilypng/%.png, $(xmlfiles))
lilymidi          := $(patsubst ../%.xml, $(version)/lilypdf/%.midi, $(xmlfiles))
lilyjianpuout     := $(patsubst ../%.xml, $(version)/lilyjianpu/%.ly, $(xmlfilesforlilyjianpu))
lilyjianpupdf     := $(patsubst ../%.xml, $(version)/lilyjianpupdf/%.pdf, $(xmlfilesforlilyjianpu))
lilyjianpumidi    := $(patsubst ../%.xml, $(version)/lilyjianpupdf/%.midi, $(xmlfilesforlilyjianpu))
brlout            := $(patsubst ../%.xml, $(version)/braille/%.brl, $(xmlfilesforbrl))

validxml 	          = $(patsubst %.xml, %.outxml, $(readout))
validgmn 	          = $(patsubst %.gmn, %.outgmn, $(gmnout))
validlilyignored    = $(patsubst %.ly, %.outlilyignored, $(lilyignoredout))
validlily 	        = $(patsubst %.ly, %.outly, $(lilyout))
validguidosvg       = $(patsubst %.svg, %.outsvg, $(guidosvg))
validlilypdf      	= $(patsubst %.pdf, %.outpdf, $(lilypdf))
validlilypng      	= $(patsubst %.png, %.outpng, $(lilypng))
validlilyjianpu     = $(patsubst %.ly, %.outlilyjianpu, $(lilyjianpuout))
validlilyjianpupdf  = $(patsubst %.ly, %.outlilyjianpupdf, $(lilyjianpupdf))
validbrl            = $(patsubst %.brl, %.outbrl, $(brlout))

XML2GUIDO ?= xml2guido
XML2LY    ?= xml2ly
XML2BRL   ?= xml2brl
XMLREAD   ?= xmlread
MAKE      ?= make

.PHONY: read guido lilyignored lily lilyjianpu brl
.DELETE_ON_ERROR:

all:
	$(MAKE) read
	$(MAKE) guido
	$(MAKE) lilyignored
	$(MAKE) lily
	$(MAKE) lilypdf
	$(MAKE) lilypng
	$(MAKE) lilyjianpu
	$(MAKE) lilyjianpupdf
#	$(MAKE) brl

clean:
	$(MAKE) lilyignoredclean
	$(MAKE) lilyclean
	$(MAKE) lilypdfclean
	$(MAKE) lilypngclean
	$(MAKE) lilyjianpuclean
	$(MAKE) lilyjianpupdfclean
#	$(MAKE) brlclean

travis:
	$(MAKE) read
	$(MAKE) guido

trace:
	@echo $(validxml)

help:
	@echo "Makefile for testing the libmusicxml library output."
	@echo "Testing is based on the xml files located in the 'files' folder."
	@echo "Available targets are:"
	@echo "  'all' (default)  : call the 'read', 'guido', 'lilyignored', 'lily' and 'lilyjianpu' targets."
	@echo "  'read'           : reads the set of xml files and writes the corresponding output"
	@echo "  'guido'          : converts the set of xml files to guido"
	@echo "  'guidosvg'       : converts the output of 'guido' target to svg"
	@echo "  'lilyignored'    : converts a subset of xml files to lilypond, but fails as expected"
	@echo "  'lily'           : converts the set of xml files to lilypond"
	@echo "  'lilypng'        : converts the output of 'lily' target to png"
	@echo "  'lilypdf'        : converts the output of 'lily' target to pdf"
	@echo "  'lilyjianpu'     : converts the a subset of xml files to lilypond code using $(jianpuinclude)"
	@echo "  'lilyjianpupdf ' : converts the output of 'lilyjianpu' target to pdf"
	@echo "  'lilyjianpupng'  : converts the output of 'lilyjianpu' target to png"
#	@echo "  'brl'            : converts a subset of xml files to BRF (Braille Ready Format) music"
	@echo "Output files are written to the $(version) folder, "
	@echo "where $(version) is taken from the libmusicxmlversion.txt file that you can freely change."
	@echo
	@echo "Validation target:"
	@echo "  'validate VERSION=another_version': compares the current version output (xml and gmn files) with the one in 'another_version' folder"
#	@echo "  (on Windows, Firefox has to be installed (and opened) in C:/Program Files (x86)/Mozilla Firefox/firefox, otherwise please change the Makefile)"
	@echo
	@echo "Options:"
	@echo "  'XML2GUIDO=/path/to/xml2guido"
	@echo "  'XML2LY=/path/to/xml2ly"
#	@echo "  'XML2BRL=/path/to/xml2brl"
	@echo "  'XMLREAD=/path/to/xmlread"
	@echo "when the above variables are not specified, the tools are taken from the current PATH env."

#########################################################################
read:
	@which $(XMLREAD) > /dev/null || (echo "### xmlread (part of samples) must be available from your PATH."; false;)
	$(MAKE) doread

doread: $(readout)

#########################################################################
guido:
	@which $(XML2GUIDO) > /dev/null || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	$(MAKE) doguido

doguido: $(gmnout)

#########################################################################
guidosvg:
	@which guido2svg || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	$(MAKE) guido
	$(MAKE) doguidosvg

doguidosvg: $(guidosvg)

#########################################################################
lilyignored:
	@echo "xmlfilesforlilyignored: " $(xmlfilesforlilyignored)
	@echo "lilyignoredout: " $(lilyignoredout)
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dolilyignored

dolilyignored: $(lilyignoredout)

#########################################################################
lilyignoredclean:
	rm $(lilyignoredout)

#########################################################################
lily:
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dolily

dolily: $(lilyout)

#########################################################################
lilyclean:
	@echo "lilyout = " $(lilyout) ", " $(shell wc < echo $(lilyout))
	rm $(lilyout)

#########################################################################
lilypdf:
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) lily
	$(MAKE) dolilypdf

dolilypdf: $(lilypdf)

#########################################################################
lilypdfclean:
	@echo "lilypdf = " $(lilypdf) ", " $(shell wc < echo $(lilypdf))
	@echo "lilymidi = " $(lilymidi) ", " $(shell wc < echo $(lilymidi))
	rm $(lilypdf)
	rm $(lilymidi)

#########################################################################
lilypng:
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) lily
	$(MAKE) dolilypng

dolilypng: $(lilypng)

#########################################################################
lilypngclean:
	@echo "lilypng = " $(lilypng) ", " $(shell wc < echo $(lilypng))
	rm $(lilypng)
	rm $(lilymidi)

#########################################################################
lilyjianpu:
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dolilyjianpu
	@ls -sal $(lilyjianpuout)

dolilyjianpu: $(lilyjianpuout)

#########################################################################
lilyjianpuclean:
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@echo "lilyjianpumidi = " $(lilyjianpumidi) ", " $(shell wc < echo $(lilyjianpumidi))
	rm $(lilyjianpuout)

#########################################################################
lilyjianpupdf:
	$(MAKE) lilyjianpu
	@echo "jianpuinclude: " $(jianpuinclude)
	@test "z${jianpuinclude}" != "z" || (echo "### $(jianpuincludefilename) not found in the validation directory."; false;)
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) dolilyjianpupdf

dolilyjianpupdf: $(lilyjianpupdf)

#########################################################################
lilyjianpupdfclean:
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@echo "lilyjianpumidi = " $(lilyjianpumidi) ", " $(shell wc < echo $(lilyjianpumidi))
	rm $(lilyjianpupdf)
	rm $(lilyjianpumidi)

#########################################################################
brl:
	@echo "xmlfilesforbrl: " $(xmlfilesforbrl)
	@echo "brlout: " $(brlout)
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dobrl

dobrl: $(brlout)

#########################################################################
brlclean:
	@echo "brlout = " $(brlout) ", " $(shell wc < echo $(brlout))
	rm $(brlout)

#########################################################################
validate:
	@echo Validating version $(version) with $(VERSION) $(tmp)
	$(MAKE) validxml
	$(MAKE) validgmn
	$(MAKE) validlilyignored
	$(MAKE) validlily
	$(MAKE) validlilyjianpu
#	$(MAKE) validbrl

validxml: $(validxml)

validgmn: $(validgmn)

validlilyignored: $(validlilyignored)

validlily: $(validlily)

validlilyjianpu: $(validlilyjianpu)

validbrl: $(validbrl)

#########################################################################
# rules for xmlread
$(version)/read/%.xml: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XMLREAD) -o $@ $<

#########################################################################
# rules for xml2guido
$(version)/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2GUIDO) -o $@ $<

#########################################################################
# rules for guido2svg
$(version)/guidosvg/%.svg: $(version)/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	guido2svg  -o $@ $<

#########################################################################
# rules for xml2ly
$(version)/lilypond/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -o $@ $<

$(version)/lilyignored/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -o $@ $<  || true

#########################################################################
# rules for lilypond with pdf output
$(version)/lilypdf/%.pdf: $(version)/lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	lilypond --pdf -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with png output
$(version)/lilypng/%.png: $(version)/lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	lilypond --png -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with jianpu inclusion in output
$(version)/lilyjianpu/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -jianpu -o $@ $<

#########################################################################
# rules for lilypond pdf output in jianpu notation
$(version)/lilyjianpupdf/%.pdf: $(version)/lilyjianpu/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	cp -p $(jianpuinclude) $(@D)
	lilypond --pdf -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for xml2brl
$(version)/braille/%.brl: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2BRL) -q  -o $@ $<

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

%.outly: %.ly
	$(eval tmp := $(patsubst $(version)/%, $(VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(version)/%, $(VERSION)/%, $<) # to check changes"; true)

%.outlilyjianpu: %.ly
	$(eval tmp := $(patsubst $(version)/%, $(VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(version)/%, $(VERSION)/%, $<) # to check changes"; true)

%.outbrl: %.brl
	$(eval tmp := $(patsubst $(version)/%, $(VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(version)/%, $(VERSION)/%, $<) # to check changes"; true)
