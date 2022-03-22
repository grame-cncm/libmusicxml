#
# libmusicxml Javascript
# This makefile is intended to compile the libmusicxml library as a javascript library
# It is based on emscript that must be available from the command line
# see http://kripken.github.io/emscripten-site/ for more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#
# The TOTAL_MEMORY is the maximum memory allocated by emscripten. This value is specific (100663296) to be compliant with the asm.js and prevent warning at execution.
# It can be increased if error occured.
# 

LILY    ?= no
srcdir  := ../src
binddir := ./src
npmdir  := ./npm

folders := factory guido lib parser elements files interface operations	visitors
sources := $(wildcard $(srcdir)/factory/*.cpp  $(srcdir)/guido/*.cpp $(srcdir)/lib/*.cpp  $(srcdir)/parser/*.cpp  $(srcdir)/elements/*.cpp  $(srcdir)/files/*.cpp $(srcdir)/operations/*.cpp $(srcdir)/visitors/*.cpp )
sources := $(sources) $(srcdir)/interface/libmusicxml.cpp $(srcdir)/interface/musicxml2guido.cpp

subprojects :=  $(patsubst %, $(srcdir)/%, $(folders))
bindsrc = $(wildcard $(binddir)/*.cpp) 
objects = $(patsubst $(srcdir)%, obj%, $(patsubst %.cpp,%.o,$(sources)) $(patsubst $(binddir)%.cpp, obj/binding%.o,$(bindsrc)))

GCC = emcc
CXXFLAGS += -std=c++11  $(addprefix -I, $(subprojects)) -O3

library = libmusicxml

OPTIONS :=  -s ALLOW_MEMORY_GROWTH=0 -s TOTAL_MEMORY=134217728 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0 -s MODULARIZE=1 -s EXPORT_NAME="'MusicXMLModule'"

target = $(library).js

.PHONY: npm

all :
	make $(target)

help :
	@echo "libmusicxml wasm library generation"
	@echo "   all (default): build the wasm library,"	
	@echo "   clean        : remove output and object files,"	
	@echo "   depend       : recompute dependencies,"	
	@echo "   npmlib       : update lib in npm"	
	@echo "   test         : run npm test"	
	@echo "   npm          : publish to npm (use with caution),"	
	@echo "                  see the npm folder,"	
	@echo 	
	@echo "Options:"
	@echo "   LILY   include lilypond part when set to 'yes' (default is $(LILY))"
	
##############################################################
# publishing to npm
##############################################################
npm: $(npmdir)/$(library).js
	npm publish $(npmdir) --access public

test: $(npmdir)/$(library).js
	cd npm && npm run test

npmlib: $(npmdir)/$(library).js

$(npmdir)/$(library).js : $(library).js
	cp $(library).js   $(npmdir)
	cp $(library).wasm $(npmdir)


##############################################################
# building the library as web assembly
##############################################################
$(library).js : $(objects) Makefile
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $@ $(OPTIONS) -s WASM=1

clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

##############################################################
# rules for building the library
##############################################################
obj/binding/%.o: $(binddir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) -I../../src/interface $< -o $@

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@


# DO NOT DELETE

../src/factory/musicxmlfactory.o: ../src/interface/libmusicxml.h
../src/factory/musicxmlfactory.o: ../src/elements/exports.h
../src/factory/musicxmlfactory.o: ../src/factory/musicxmlfactory.h
../src/factory/musicxmlfactory.o: ../src/files/xmlfile.h
../src/factory/musicxmlfactory.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/factory/musicxmlfactory.o: ../src/lib/smartpointer.h
../src/factory/musicxmlfactory.o: ../src/visitors/visitable.h
../src/factory/musicxmlfactory.o: ../src/visitors/basevisitor.h
../src/factory/musicxmlfactory.o: ../src/elements/elements.h
../src/factory/musicxmlfactory.o: ../src/elements/factory.h
../src/factory/musicxmlfactory.o: ../src/lib/functor.h ../src/lib/singleton.h
../src/factory/musicxmlfactory.o: ../src/factory/sortvisitor.h
../src/factory/musicxmlfactory.o: ../src/visitors/visitor.h
../src/factory/musicxmlfactory.o: ../src/elements/typedefs.h
../src/factory/musicxmlfactory.o: ../src/elements/types.h
../src/factory/musicxmlfactory.o: ../src/elements/xml_tree_browser.h
../src/factory/musicxmlfactory.o: ../src/lib/tree_browser.h
../src/factory/musicxmlfactory.o: ../src/lib/browser.h
../src/factory/musicxmlfactory.o: ../src/elements/versions.h
../src/factory/sortvisitor.o: ../src/factory/sortvisitor.h
../src/factory/sortvisitor.o: ../src/visitors/visitor.h
../src/factory/sortvisitor.o: ../src/visitors/basevisitor.h
../src/factory/sortvisitor.o: ../src/elements/typedefs.h
../src/factory/sortvisitor.o: ../src/elements/elements.h
../src/factory/sortvisitor.o: ../src/elements/types.h
../src/factory/sortvisitor.o: ../src/elements/exports.h ../src/elements/xml.h
../src/factory/sortvisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/factory/sortvisitor.o: ../src/visitors/visitable.h
../src/guido/guido.o: ../src/guido/guido.h ../src/elements/exports.h
../src/guido/guido.o: ../src/lib/smartpointer.h
../src/guido/xml2guidovisitor.o: ../src/visitors/partsummary.h
../src/guido/xml2guidovisitor.o: ../src/visitors/notevisitor.h
../src/guido/xml2guidovisitor.o: ../src/lib/conversions.h
../src/guido/xml2guidovisitor.o: ../src/elements/exports.h
../src/guido/xml2guidovisitor.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/guido/xml2guidovisitor.o: ../src/elements/typedefs.h
../src/guido/xml2guidovisitor.o: ../src/elements/elements.h
../src/guido/xml2guidovisitor.o: ../src/elements/types.h
../src/guido/xml2guidovisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/guido/xml2guidovisitor.o: ../src/lib/smartpointer.h
../src/guido/xml2guidovisitor.o: ../src/visitors/visitable.h
../src/guido/xml2guidovisitor.o: ../src/visitors/basevisitor.h
../src/guido/xml2guidovisitor.o: ../src/visitors/visitor.h
../src/guido/xml2guidovisitor.o: ../src/lib/smartlist.h
../src/guido/xml2guidovisitor.o: ../src/elements/xml_tree_browser.h
../src/guido/xml2guidovisitor.o: ../src/lib/tree_browser.h
../src/guido/xml2guidovisitor.o: ../src/lib/browser.h
../src/guido/xml2guidovisitor.o: ../src/guido/xml2guidovisitor.h
../src/guido/xml2guidovisitor.o: ../src/guido/guido.h
../src/guido/xml2guidovisitor.o: ../src/visitors/partlistvisitor.h
../src/guido/xml2guidovisitor.o: ../src/guido/xmlpart2guido.h
../src/guido/xml2guidovisitor.o: ../src/visitors/clefvisitor.h
../src/guido/xml2guidovisitor.o: ../src/visitors/keysignvisitor.h
../src/guido/xml2guidovisitor.o: ../src/visitors/metronomevisitor.h
../src/guido/xml2guidovisitor.o: ../src/visitors/timesignvisitor.h
../src/guido/xmlpart2guido.o: ../src/lib/conversions.h
../src/guido/xmlpart2guido.o: ../src/elements/exports.h ../src/lib/rational.h
../src/guido/xmlpart2guido.o: ../src/lib/bimap.h
../src/guido/xmlpart2guido.o: ../src/visitors/partsummary.h
../src/guido/xmlpart2guido.o: ../src/visitors/notevisitor.h
../src/guido/xmlpart2guido.o: ../src/elements/typedefs.h
../src/guido/xmlpart2guido.o: ../src/elements/elements.h
../src/guido/xmlpart2guido.o: ../src/elements/types.h ../src/elements/xml.h
../src/guido/xmlpart2guido.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/guido/xmlpart2guido.o: ../src/visitors/visitable.h
../src/guido/xmlpart2guido.o: ../src/visitors/basevisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/visitor.h
../src/guido/xmlpart2guido.o: ../src/lib/smartlist.h
../src/guido/xmlpart2guido.o: ../src/guido/xml2guidovisitor.h
../src/guido/xmlpart2guido.o: ../src/guido/guido.h
../src/guido/xmlpart2guido.o: ../src/visitors/partlistvisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/metronomevisitor.h
../src/guido/xmlpart2guido.o: ../src/guido/xmlpart2guido.h
../src/guido/xmlpart2guido.o: ../src/visitors/clefvisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/keysignvisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/timesignvisitor.h
../src/guido/xmlpart2guido.o: ../src/elements/xml_tree_browser.h
../src/guido/xmlpart2guido.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lib/conversions.o: ../src/lib/conversions.h ../src/elements/exports.h
../src/lib/conversions.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/lib/rational.o: ../src/lib/rational.h ../src/elements/exports.h
../src/parser/xmlparse.o: ../src/parser/reader.h ../src/parser/xmllex.c++
../src/parser/xmlparse.o: ../src/parser/xmlparse.hpp
../src/elements/factory.o: ../src/elements/elements.h
../src/elements/factory.o: ../src/elements/factory.h ../src/lib/functor.h
../src/elements/factory.o: ../src/lib/singleton.h ../src/elements/xml.h
../src/elements/factory.o: ../src/elements/exports.h ../src/lib/ctree.h
../src/elements/factory.o: ../src/lib/smartpointer.h
../src/elements/factory.o: ../src/visitors/visitable.h
../src/elements/factory.o: ../src/visitors/basevisitor.h
../src/elements/factory.o: ../src/elements/types.h ../src/visitors/visitor.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/unrolled_xml_tree_browser.h
../src/elements/unrolled_xml_tree_browser.o: ../src/visitors/visitor.h
../src/elements/unrolled_xml_tree_browser.o: ../src/visitors/basevisitor.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/xml.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/exports.h
../src/elements/unrolled_xml_tree_browser.o: ../src/lib/ctree.h
../src/elements/unrolled_xml_tree_browser.o: ../src/lib/smartpointer.h
../src/elements/unrolled_xml_tree_browser.o: ../src/visitors/visitable.h
../src/elements/unrolled_xml_tree_browser.o: ../src/lib/browser.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/typedefs.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/elements.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/types.h
../src/elements/versions.o: ../src/elements/versions.h
../src/elements/versions.o: ../src/elements/exports.h
../src/elements/xml.o: ../src/elements/xml.h ../src/elements/exports.h
../src/elements/xml.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/elements/xml.o: ../src/visitors/visitable.h
../src/elements/xml.o: ../src/visitors/basevisitor.h
../src/elements/xml.o: ../src/visitors/visitor.h
../src/elements/xml_tree_browser.o: ../src/elements/xml_tree_browser.h
../src/elements/xml_tree_browser.o: ../src/lib/tree_browser.h
../src/elements/xml_tree_browser.o: ../src/elements/exports.h
../src/elements/xml_tree_browser.o: ../src/visitors/basevisitor.h
../src/elements/xml_tree_browser.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/elements/xml_tree_browser.o: ../src/lib/smartpointer.h
../src/elements/xml_tree_browser.o: ../src/visitors/visitable.h
../src/elements/xml_tree_browser.o: ../src/elements/xml.h
../src/files/xmlfile.o: ../src/files/xmlfile.h ../src/elements/exports.h
../src/files/xmlfile.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/files/xmlfile.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/files/xmlfile.o: ../src/visitors/basevisitor.h
../src/files/xmlfile.o: ../src/visitors/xmlvisitor.h
../src/files/xmlfile.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/files/xmlfile.o: ../src/elements/typedefs.h ../src/elements/elements.h
../src/files/xmlfile.o: ../src/elements/types.h ../src/visitors/visitor.h
../src/files/xmlreader.o: ../src/files/xmlreader.h ../src/elements/exports.h
../src/files/xmlreader.o: ../src/files/xmlfile.h ../src/elements/xml.h
../src/files/xmlreader.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/files/xmlreader.o: ../src/visitors/visitable.h
../src/files/xmlreader.o: ../src/visitors/basevisitor.h
../src/files/xmlreader.o: ../src/parser/reader.h ../src/elements/factory.h
../src/files/xmlreader.o: ../src/lib/functor.h ../src/lib/singleton.h
../src/operations/musicxmlquery.o: ../src/operations/musicxmlquery.h
../src/operations/musicxmlquery.o: ../src/elements/exports.h
../src/operations/musicxmlquery.o: ../src/elements/typedefs.h
../src/operations/musicxmlquery.o: ../src/elements/elements.h
../src/operations/musicxmlquery.o: ../src/elements/types.h
../src/operations/musicxmlquery.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/operations/musicxmlquery.o: ../src/lib/smartpointer.h
../src/operations/musicxmlquery.o: ../src/visitors/visitable.h
../src/operations/musicxmlquery.o: ../src/visitors/basevisitor.h
../src/operations/musicxmlquery.o: ../src/visitors/visitor.h
../src/operations/musicxmlquery.o: ../src/visitors/notevisitor.h
../src/operations/musicxmlquery.o: ../src/lib/conversions.h
../src/operations/musicxmlquery.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/operations/musicxmlquery.o: ../src/visitors/transposevisitor.h
../src/operations/musicxmlquery.o: ../src/visitors/partlistvisitor.h
../src/operations/musicxmlquery.o: ../src/guido/guido.h
../src/operations/musicxmlquery.o: ../src/lib/smartlist.h
../src/operations/musicxmlquery.o: ../src/visitors/partsummary.h
../src/operations/musicxmlquery.o: ../src/visitors/clefvisitor.h
../src/operations/musicxmlquery.o: ../src/interface/libmusicxml.h
../src/operations/musicxmlquery.o: ../src/files/xmlfile.h
../src/operations/musicxmlquery.o: ../src/files/xmlreader.h
../src/operations/musicxmlquery.o: ../src/parser/reader.h
../src/operations/musicxmlquery.o: ../src/elements/xml_tree_browser.h
../src/operations/musicxmlquery.o: ../src/lib/tree_browser.h
../src/operations/musicxmlquery.o: ../src/lib/browser.h
../src/operations/transposition.o: ../src/elements/factory.h
../src/operations/transposition.o: ../src/lib/functor.h
../src/operations/transposition.o: ../src/lib/singleton.h
../src/operations/transposition.o: ../src/elements/xml.h
../src/operations/transposition.o: ../src/elements/exports.h
../src/operations/transposition.o: ../src/lib/ctree.h
../src/operations/transposition.o: ../src/lib/smartpointer.h
../src/operations/transposition.o: ../src/visitors/visitable.h
../src/operations/transposition.o: ../src/visitors/basevisitor.h
../src/operations/transposition.o: ../src/operations/transposition.h
../src/operations/transposition.o: ../src/elements/typedefs.h
../src/operations/transposition.o: ../src/elements/elements.h
../src/operations/transposition.o: ../src/elements/types.h
../src/operations/transposition.o: ../src/visitors/visitor.h
../src/operations/transposition.o: ../src/visitors/transposevisitor.h
../src/operations/transposition.o: ../src/visitors/notevisitor.h
../src/operations/transposition.o: ../src/lib/conversions.h
../src/operations/transposition.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/operations/transposition.o: ../src/elements/xml_tree_browser.h
../src/operations/transposition.o: ../src/lib/tree_browser.h
../src/operations/transposition.o: ../src/lib/browser.h
../src/visitors/clefvisitor.o: ../src/visitors/clefvisitor.h
../src/visitors/clefvisitor.o: ../src/elements/typedefs.h
../src/visitors/clefvisitor.o: ../src/elements/elements.h
../src/visitors/clefvisitor.o: ../src/elements/types.h
../src/visitors/clefvisitor.o: ../src/elements/exports.h
../src/visitors/clefvisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/clefvisitor.o: ../src/lib/smartpointer.h
../src/visitors/clefvisitor.o: ../src/visitors/visitable.h
../src/visitors/clefvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/clefvisitor.o: ../src/visitors/visitor.h
../src/visitors/clonevisitor.o: ../src/visitors/clonevisitor.h
../src/visitors/clonevisitor.o: ../src/visitors/visitor.h
../src/visitors/clonevisitor.o: ../src/visitors/basevisitor.h
../src/visitors/clonevisitor.o: ../src/elements/xml.h
../src/visitors/clonevisitor.o: ../src/elements/exports.h ../src/lib/ctree.h
../src/visitors/clonevisitor.o: ../src/lib/smartpointer.h
../src/visitors/clonevisitor.o: ../src/visitors/visitable.h
../src/visitors/keysignvisitor.o: ../src/visitors/keysignvisitor.h
../src/visitors/keysignvisitor.o: ../src/elements/typedefs.h
../src/visitors/keysignvisitor.o: ../src/elements/elements.h
../src/visitors/keysignvisitor.o: ../src/elements/types.h
../src/visitors/keysignvisitor.o: ../src/elements/exports.h
../src/visitors/keysignvisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/keysignvisitor.o: ../src/lib/smartpointer.h
../src/visitors/keysignvisitor.o: ../src/visitors/visitable.h
../src/visitors/keysignvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/keysignvisitor.o: ../src/visitors/visitor.h
../src/visitors/keyvisitor.o: ../src/visitors/keyvisitor.h
../src/visitors/keyvisitor.o: ../src/elements/typedefs.h
../src/visitors/keyvisitor.o: ../src/elements/elements.h
../src/visitors/keyvisitor.o: ../src/elements/types.h
../src/visitors/keyvisitor.o: ../src/elements/exports.h ../src/elements/xml.h
../src/visitors/keyvisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/visitors/keyvisitor.o: ../src/visitors/visitable.h
../src/visitors/keyvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/keyvisitor.o: ../src/visitors/visitor.h
../src/visitors/metronomevisitor.o: ../src/visitors/metronomevisitor.h
../src/visitors/metronomevisitor.o: ../src/elements/typedefs.h
../src/visitors/metronomevisitor.o: ../src/elements/elements.h
../src/visitors/metronomevisitor.o: ../src/elements/types.h
../src/visitors/metronomevisitor.o: ../src/elements/exports.h
../src/visitors/metronomevisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/metronomevisitor.o: ../src/lib/smartpointer.h
../src/visitors/metronomevisitor.o: ../src/visitors/visitable.h
../src/visitors/metronomevisitor.o: ../src/visitors/basevisitor.h
../src/visitors/metronomevisitor.o: ../src/visitors/visitor.h
../src/visitors/midicontextvisitor.o: ../src/visitors/midicontextvisitor.h
../src/visitors/midicontextvisitor.o: ../src/elements/xml.h
../src/visitors/midicontextvisitor.o: ../src/elements/exports.h
../src/visitors/midicontextvisitor.o: ../src/lib/ctree.h
../src/visitors/midicontextvisitor.o: ../src/lib/smartpointer.h
../src/visitors/midicontextvisitor.o: ../src/visitors/visitable.h
../src/visitors/midicontextvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/midicontextvisitor.o: ../src/elements/typedefs.h
../src/visitors/midicontextvisitor.o: ../src/elements/elements.h
../src/visitors/midicontextvisitor.o: ../src/elements/types.h
../src/visitors/midicontextvisitor.o: ../src/visitors/visitor.h
../src/visitors/midicontextvisitor.o: ../src/visitors/notevisitor.h
../src/visitors/midicontextvisitor.o: ../src/lib/conversions.h
../src/visitors/midicontextvisitor.o: ../src/lib/rational.h
../src/visitors/midicontextvisitor.o: ../src/lib/bimap.h
../src/visitors/midicontextvisitor.o: ../src/visitors/transposevisitor.h
../src/visitors/notevisitor.o: ../src/visitors/notevisitor.h
../src/visitors/notevisitor.o: ../src/lib/conversions.h
../src/visitors/notevisitor.o: ../src/elements/exports.h
../src/visitors/notevisitor.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/visitors/notevisitor.o: ../src/elements/typedefs.h
../src/visitors/notevisitor.o: ../src/elements/elements.h
../src/visitors/notevisitor.o: ../src/elements/types.h ../src/elements/xml.h
../src/visitors/notevisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/visitors/notevisitor.o: ../src/visitors/visitable.h
../src/visitors/notevisitor.o: ../src/visitors/basevisitor.h
../src/visitors/notevisitor.o: ../src/visitors/visitor.h
../src/visitors/partlistvisitor.o: ../src/visitors/partlistvisitor.h
../src/visitors/partlistvisitor.o: ../src/elements/exports.h
../src/visitors/partlistvisitor.o: ../src/guido/guido.h
../src/visitors/partlistvisitor.o: ../src/lib/smartpointer.h
../src/visitors/partlistvisitor.o: ../src/elements/typedefs.h
../src/visitors/partlistvisitor.o: ../src/elements/elements.h
../src/visitors/partlistvisitor.o: ../src/elements/types.h
../src/visitors/partlistvisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/partlistvisitor.o: ../src/visitors/visitable.h
../src/visitors/partlistvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/partlistvisitor.o: ../src/visitors/visitor.h
../src/visitors/partlistvisitor.o: ../src/lib/smartlist.h
../src/visitors/partlistvisitor.o: ../src/lib/rational.h
../src/visitors/partlistvisitor.o: ../src/elements/xml_tree_browser.h
../src/visitors/partlistvisitor.o: ../src/lib/tree_browser.h
../src/visitors/partlistvisitor.o: ../src/lib/browser.h
../src/visitors/partsummary.o: ../src/visitors/partsummary.h
../src/visitors/partsummary.o: ../src/visitors/notevisitor.h
../src/visitors/partsummary.o: ../src/lib/conversions.h
../src/visitors/partsummary.o: ../src/elements/exports.h
../src/visitors/partsummary.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/visitors/partsummary.o: ../src/elements/typedefs.h
../src/visitors/partsummary.o: ../src/elements/elements.h
../src/visitors/partsummary.o: ../src/elements/types.h ../src/elements/xml.h
../src/visitors/partsummary.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/visitors/partsummary.o: ../src/visitors/visitable.h
../src/visitors/partsummary.o: ../src/visitors/basevisitor.h
../src/visitors/partsummary.o: ../src/visitors/visitor.h
../src/visitors/partsummary.o: ../src/lib/smartlist.h
../src/visitors/timesignvisitor.o: ../src/visitors/timesignvisitor.h
../src/visitors/timesignvisitor.o: ../src/elements/typedefs.h
../src/visitors/timesignvisitor.o: ../src/elements/elements.h
../src/visitors/timesignvisitor.o: ../src/elements/types.h
../src/visitors/timesignvisitor.o: ../src/elements/exports.h
../src/visitors/timesignvisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/timesignvisitor.o: ../src/lib/smartpointer.h
../src/visitors/timesignvisitor.o: ../src/visitors/visitable.h
../src/visitors/timesignvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/timesignvisitor.o: ../src/visitors/visitor.h
../src/visitors/timesignvisitor.o: ../src/lib/rational.h
../src/visitors/transposevisitor.o: ../src/visitors/transposevisitor.h
../src/visitors/transposevisitor.o: ../src/elements/typedefs.h
../src/visitors/transposevisitor.o: ../src/elements/elements.h
../src/visitors/transposevisitor.o: ../src/elements/types.h
../src/visitors/transposevisitor.o: ../src/elements/exports.h
../src/visitors/transposevisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/transposevisitor.o: ../src/lib/smartpointer.h
../src/visitors/transposevisitor.o: ../src/visitors/visitable.h
../src/visitors/transposevisitor.o: ../src/visitors/basevisitor.h
../src/visitors/transposevisitor.o: ../src/visitors/visitor.h
../src/visitors/unrolled_clonevisitor.o: ../src/visitors/unrolled_clonevisitor.h
../src/visitors/unrolled_clonevisitor.o: ../src/visitors/visitor.h
../src/visitors/unrolled_clonevisitor.o: ../src/visitors/basevisitor.h
../src/visitors/unrolled_clonevisitor.o: ../src/visitors/clonevisitor.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/xml.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/exports.h
../src/visitors/unrolled_clonevisitor.o: ../src/lib/ctree.h
../src/visitors/unrolled_clonevisitor.o: ../src/lib/smartpointer.h
../src/visitors/unrolled_clonevisitor.o: ../src/visitors/visitable.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/typedefs.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/elements.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/types.h
../src/visitors/xmlvisitor.o: ../src/visitors/xmlvisitor.h
../src/visitors/xmlvisitor.o: ../src/lib/tree_browser.h
../src/visitors/xmlvisitor.o: ../src/elements/exports.h
../src/visitors/xmlvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/xmlvisitor.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/visitors/xmlvisitor.o: ../src/lib/smartpointer.h
../src/visitors/xmlvisitor.o: ../src/visitors/visitable.h
../src/visitors/xmlvisitor.o: ../src/elements/typedefs.h
../src/visitors/xmlvisitor.o: ../src/elements/elements.h
../src/visitors/xmlvisitor.o: ../src/elements/types.h ../src/elements/xml.h
../src/visitors/xmlvisitor.o: ../src/visitors/visitor.h
../src/interface/libmusicxml.o: ../src/interface/libmusicxml.h
../src/interface/libmusicxml.o: ../src/elements/exports.h
../src/interface/libmusicxml.o: ../src/factory/musicxmlfactory.h
../src/interface/libmusicxml.o: ../src/files/xmlfile.h ../src/elements/xml.h
../src/interface/libmusicxml.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/interface/libmusicxml.o: ../src/visitors/visitable.h
../src/interface/libmusicxml.o: ../src/visitors/basevisitor.h
../src/interface/libmusicxml.o: ../src/elements/elements.h
../src/interface/libmusicxml.o: ../src/elements/versions.h
../src/interface/musicxml2guido.o: ../src/interface/libmusicxml.h
../src/interface/musicxml2guido.o: ../src/elements/exports.h
../src/interface/musicxml2guido.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/interface/musicxml2guido.o: ../src/lib/smartpointer.h
../src/interface/musicxml2guido.o: ../src/visitors/visitable.h
../src/interface/musicxml2guido.o: ../src/visitors/basevisitor.h
../src/interface/musicxml2guido.o: ../src/files/xmlfile.h
../src/interface/musicxml2guido.o: ../src/files/xmlreader.h
../src/interface/musicxml2guido.o: ../src/parser/reader.h
../src/interface/musicxml2guido.o: ../src/guido/xml2guidovisitor.h
../src/interface/musicxml2guido.o: ../src/guido/guido.h
../src/interface/musicxml2guido.o: ../src/elements/typedefs.h
../src/interface/musicxml2guido.o: ../src/elements/elements.h
../src/interface/musicxml2guido.o: ../src/elements/types.h
../src/interface/musicxml2guido.o: ../src/visitors/visitor.h
../src/interface/musicxml2guido.o: ../src/lib/rational.h
../src/interface/musicxml2guido.o: ../src/visitors/partlistvisitor.h
../src/interface/musicxml2guido.o: ../src/lib/smartlist.h
