
MAKE ?= make

xmlschema 	?= MusicXML/3.1/musicxml.xsd
meischema 	?= MEI/4.0.1/mei-CMN.rng
bmmlschema 	?= BMML/BMML.dtd
scripts		:= scripts
template	:= $(spec)/templates
dst     	:= ../src/elements/$(PREFIX)


elts  	:= $(PREFIX)Elements.txt
eltname := '[-a-zA-Z0-9_.:]*'

HEADER  := $(PREFIX)Elements.h
TYPES   := $(PREFIX)Typedefs.h
FACTORY := $(PREFIX)Factory.cpp

.PHONY: mei bmml

all :
	$(MAKE) xml
	$(MAKE) mei
	$(MAKE) bmml


help :
	@echo "########################## libmusicxml project ##########################"
	@echo "libmusicxml makefile - Targets are :"
	@echo "This Makefile is intended to generate code from xml schemas"
	@echo "Available targets are :"
	@echo "   all (default): generates code from MusicXML, MEI and BMML schemas"
	@echo "   xml          : generates code from MusicXML schema"
	@echo "   mei          : generates code from MEI schema"
	@echo "   bmml         : generates code from BMML schema"
	@echo "   install      : install the generated code to the src tree"



##############################
xml :
	$(MAKE) _xml PREFIX=xml schema=$(xmlschema) spec=MusicXML
_xml : $(elts) $(HEADER) $(TYPES) $(FACTORY)

##############################
mei :
	$(MAKE) _mei PREFIX=mei schema=$(meischema) spec=MEI
_mei : $(elts) $(HEADER) $(TYPES) $(FACTORY)

##############################
bmml :
	$(MAKE) _bmml PREFIX=bmml schema=$(bmmlschema) spec=BMML
_bmml : $(elts) $(HEADER) $(TYPES) $(FACTORY)

##############################
install:
	$(MAKE) _install PREFIX=xml
	$(MAKE) _install PREFIX=mei
	$(MAKE) _install PREFIX=bmml

_install:
	mkdir -p $(dst)
	cp $(HEADER) $(TYPES) $(FACTORY) $(dst)

##############################
$(HEADER) : $(template)/elements.txt $(elts)
	$(scripts)/elements.bash $(elts) $(template) constants > $(HEADER)  || rm -f $(HEADER)

$(FACTORY) : $(template)/factory.txt $(elts)
	$(scripts)/elements.bash $(elts) $(template) map > $(FACTORY) || rm -f $(FACTORY)

$(TYPES) :  $(template)/typedefs.txt $(elts)
	$(scripts)/elements.bash $(elts) $(template) types > $(TYPES)  || rm -f $(TYPES)

clean :
	rm -f *$(elts) *$(HEADER) *$(TYPES) *$(FACTORY)



##############################
# rules for xsd and rnd files
xmlElements.txt: $(schema)
	grep "<xs:element" $< | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $@

meiElements.txt: $(schema)
	grep "<element name" $< | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $@

bmmlElements.txt: $(schema)
	grep "<\!ELEMENT" $< | sed -e 's/^<\!ELEMENT //' | sed -e 's/ .*//' | sort -u > $@
