
schema 		?= 3.1/musicxml.xsd
template	:= ../src/elements/templates

ifeq ($(MEI), on)
	PREFIX  := mei-
else
	PREFIX  := 
endif

elts  	:= $(PREFIX)elements.txt
eltname := '[-a-zA-Z0-9_.:]*'

HEADER  := $(PREFIX)elements.h 
TYPES   := $(PREFIX)typedefs.h 
FACTORY := $(PREFIX)factory.cpp

all : $(elts) $(HEADER) $(TYPES) $(FACTORY)

#$(elts) : $(schema)
#	grep "<xs:element" $(schema) | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $(elts)

$(HEADER) : $(template)/elements.txt $(elts)
	$(template)/elements.bash $(elts) $(template) constants > $(HEADER)  || rm -f $(HEADER)

$(FACTORY) : $(template)/factory.txt $(elts)
	$(template)/elements.bash $(elts) $(template) map > $(FACTORY) || rm -f $(FACTORY)

$(TYPES) :  $(template)/typedefs.txt $(elts)
	$(template)/elements.bash $(elts) $(template) types > $(TYPES)  || rm -f $(TYPES)

clean :
	rm -f $(elts) $(HEADER) $(TYPES) $(FACTORY)

test:
	@echo elts $(elts) $(schema)
	
##############################
# rules for xsd and rnd files
elements.txt: $(schema)
	grep "<xs:element" $< | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $@

mei-elements.txt: $(schema)
	grep "<element" $< | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $@
