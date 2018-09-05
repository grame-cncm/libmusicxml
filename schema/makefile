
schema 		:= current/musicxml.xsd
template	:= ../src/elements/templates

elts  	:= elements.txt
eltname := '[-a-zA-Z0-9_.:]*'

SRC := $(wildcard *.cpp) 
OBJ = $(patsubst %.cpp, %.o, $(SRC))

.PHONY: current

all : current $(elts) elements.h typedefs.h factory.cpp

current:
	@[ -d current ] || (echo "Error: you must create an alias named 'current' that links to the version you want to use."; false)

$(elts) : $(schema)
	grep "<xs:element" $(schema) | sed -e 's/^.*name="//' | sed -e 's/"..*//' | sort -u > $(elts)

elements.h : $(template)/elements.txt $(elts)
	$(template)/elements $(elts) $(template) constants > elements.h  || rm -f elements.h

factory.cpp : $(template)/factory.txt $(elts)
	$(template)/elements $(elts) $(template) map > factory.cpp || rm -f factory.cpp

typedefs.h :  $(template)/typedefs.txt $(elts)
	$(template)/elements $(elts) $(template) types > typedefs.h  || rm -f typedefs.h

clean :
	rm -f $(elts) elements.h factory.cpp typedefs.h
