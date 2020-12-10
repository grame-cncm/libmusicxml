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
# Compilation option -Oz is discouraged, it provoke undefined results and assertion errors at runtime.
#

LILY     ?= yes
BRAILLE  ?= yes
GUIDO    ?= yes
MSR     := no
srcdir  := ../src
binddir := ./src
npmdir  := ./npm
distdir := $(npmdir)/dist

ifeq ($(GUIDO), yes)
	DEFS = $(DEFS) -D GUIDO
endif
ifeq ($(LILY), yes)
	MSR = yes
	DEFS = $(DEFS) -D LILY
endif
ifeq ($(BRAILLE), yes)
	MSR = yes
	DEFS = $(DEFS) -D BRAILLE
endif

folders := factory lib parser elements elements/xml files interface operations visitors

ifeq ($(GUIDO), yes)
	folders := $(folders) guido
endif

ifeq ($(MSR), yes)
	folders := $(folders) utilities musicxml msr oah mxmltree
endif

ifeq ($(LILY), yes)
	folders := $(folders) lpsr lilypond lpsr2lilypond
endif
ifeq ($(BRAILLE), yes)
	folders := $(folders) bsr braille bsr2braille msr2bsr musicxml2braille mxml2mxmltree mxmltree2msr oah2manpage msr2mxmltree bsr2bsr msr2msr msr2lpsr musicxml2lilypond musicxml2musicxml
endif

sources := $(foreach dir,$(folders), $(wildcard $(srcdir)/$(dir)/*.cpp))
subprojects :=  $(patsubst %, $(srcdir)/%, $(folders))
bindsrc = $(wildcard $(binddir)/*.cpp) 
objects = $(patsubst $(srcdir)%, obj%, $(patsubst %.cpp,%.o,$(sources)) $(patsubst $(binddir)%.cpp, obj/binding%.o,$(bindsrc)))

GCC = emcc
CXXFLAGS += -std=c++11  -DXML $(addprefix -I, $(subprojects)) -O3

library = libmusicxml

#OPTIONS :=  -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_MEMORY=134283264 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0 -s MODULARIZE=1 -s EXPORT_NAME="'MusicXMLModule'"
OPTIONS :=  -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0 -s MODULARIZE=1 -s EXPORT_NAME="'MusicXMLModule'"

target = $(library).js

##############################################################
all :
	make $(target)

##############################################################
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
	@echo "   LILY      include lilypond part when set to 'yes' (default is $(LILY))"
	@echo "   BRAILLE   include braille part when set to 'yes' (default is $(BRAILLE))"

dbg:
	@echo $(objects)

##############################################################
# publishing to npm
##############################################################
npm: $(npmdir)/$(library).js
	npm publish $(npmdir) --access public

test: $(npmdir)/$(library).js
	cd npm && npm run test

npmlib: $(distdir)/$(library).js

$(distdir)/$(library).js : $(library).js
	cp $(library).js   $(distdir)
	cp $(library).wasm $(distdir)


##############################################################
# building the library as web assembly
##############################################################
$(library).js : $(objects)
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
../src/factory/musicxmlfactory.o: ../src/utilities/utilities.h
../src/factory/musicxmlfactory.o: ../src/lib/smartpointer.h
../src/factory/musicxmlfactory.o: ../src/visitors/basevisitor.h
../src/factory/musicxmlfactory.o: ../src/factory/musicxmlfactory.h
../src/factory/musicxmlfactory.o: ../src/files/xmlfile.h
../src/factory/musicxmlfactory.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/factory/musicxmlfactory.o: ../src/visitors/visitable.h
../src/factory/musicxmlfactory.o: ../src/elements/elements.h
../src/factory/musicxmlfactory.o: ../src/elements/xml/xmlElements.h
../src/factory/musicxmlfactory.o: ../src/elements/factory.h
../src/factory/musicxmlfactory.o: ../src/lib/functor.h ../src/lib/singleton.h
../src/factory/musicxmlfactory.o: ../src/factory/sortvisitor.h
../src/factory/musicxmlfactory.o: ../src/visitors/visitor.h
../src/factory/musicxmlfactory.o: ../src/elements/typedefs.h
../src/factory/musicxmlfactory.o: ../src/elements/xml/xmlTypedefs.h
../src/factory/musicxmlfactory.o: ../src/elements/types.h
../src/factory/musicxmlfactory.o: ../src/elements/xml_tree_browser.h
../src/factory/musicxmlfactory.o: ../src/lib/tree_browser.h
../src/factory/musicxmlfactory.o: ../src/lib/browser.h
../src/factory/musicxmlfactory.o: ../src/elements/versions.h
../src/factory/sortvisitor.o: ../src/factory/sortvisitor.h
../src/factory/sortvisitor.o: ../src/visitors/visitor.h
../src/factory/sortvisitor.o: ../src/visitors/basevisitor.h
../src/factory/sortvisitor.o: ../src/elements/typedefs.h
../src/factory/sortvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/factory/sortvisitor.o: ../src/elements/elements.h
../src/factory/sortvisitor.o: ../src/elements/xml/xmlElements.h
../src/factory/sortvisitor.o: ../src/elements/types.h
../src/factory/sortvisitor.o: ../src/elements/exports.h ../src/elements/xml.h
../src/factory/sortvisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/factory/sortvisitor.o: ../src/visitors/visitable.h
../src/guido/guido.o: ../src/guido/guido.h ../src/elements/exports.h
../src/guido/guido.o: ../src/lib/smartpointer.h
../src/guido/musicxml2guido.o: ../src/interface/libmusicxml.h
../src/guido/musicxml2guido.o: ../src/elements/exports.h
../src/guido/musicxml2guido.o: ../src/utilities/utilities.h
../src/guido/musicxml2guido.o: ../src/lib/smartpointer.h
../src/guido/musicxml2guido.o: ../src/visitors/basevisitor.h
../src/guido/musicxml2guido.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/guido/musicxml2guido.o: ../src/visitors/visitable.h
../src/guido/musicxml2guido.o: ../src/files/xmlfile.h
../src/guido/musicxml2guido.o: ../src/files/xmlreader.h
../src/guido/musicxml2guido.o: ../src/parser/reader.h
../src/guido/musicxml2guido.o: ../src/guido/xml2guidovisitor.h
../src/guido/musicxml2guido.o: ../src/guido/guido.h
../src/guido/musicxml2guido.o: ../src/elements/typedefs.h
../src/guido/musicxml2guido.o: ../src/elements/xml/xmlTypedefs.h
../src/guido/musicxml2guido.o: ../src/elements/elements.h
../src/guido/musicxml2guido.o: ../src/elements/xml/xmlElements.h
../src/guido/musicxml2guido.o: ../src/elements/types.h
../src/guido/musicxml2guido.o: ../src/visitors/visitor.h
../src/guido/musicxml2guido.o: ../src/lib/rational.h
../src/guido/musicxml2guido.o: ../src/visitors/partlistvisitor.h
../src/guido/musicxml2guido.o: ../src/lib/smartlist.h
../src/guido/xml2guidovisitor.o: ../src/visitors/partsummary.h
../src/guido/xml2guidovisitor.o: ../src/visitors/notevisitor.h
../src/guido/xml2guidovisitor.o: ../src/lib/conversions.h
../src/guido/xml2guidovisitor.o: ../src/elements/exports.h
../src/guido/xml2guidovisitor.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/guido/xml2guidovisitor.o: ../src/elements/typedefs.h
../src/guido/xml2guidovisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/guido/xml2guidovisitor.o: ../src/elements/elements.h
../src/guido/xml2guidovisitor.o: ../src/elements/xml/xmlElements.h
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
../src/guido/xmlpart2guido.o: ../src/elements/xml/xmlTypedefs.h
../src/guido/xmlpart2guido.o: ../src/elements/elements.h
../src/guido/xmlpart2guido.o: ../src/elements/xml/xmlElements.h
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
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/xml/xmlTypedefs.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/elements.h
../src/elements/unrolled_xml_tree_browser.o: ../src/elements/xml/xmlElements.h
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
../src/elements/xml/xmlFactory.o: ../src/elements/elements.h
../src/elements/xml/xmlFactory.o: ../src/elements/xml/xmlElements.h
../src/elements/xml/xmlFactory.o: ../src/elements/factory.h
../src/elements/xml/xmlFactory.o: ../src/lib/functor.h ../src/lib/singleton.h
../src/elements/xml/xmlFactory.o: ../src/elements/xml.h
../src/elements/xml/xmlFactory.o: ../src/elements/exports.h
../src/elements/xml/xmlFactory.o: ../src/lib/ctree.h
../src/elements/xml/xmlFactory.o: ../src/lib/smartpointer.h
../src/elements/xml/xmlFactory.o: ../src/visitors/visitable.h
../src/elements/xml/xmlFactory.o: ../src/visitors/basevisitor.h
../src/elements/xml/xmlFactory.o: ../src/elements/types.h
../src/elements/xml/xmlFactory.o: ../src/visitors/visitor.h
../src/files/xmlfile.o: ../src/files/xmlfile.h ../src/elements/exports.h
../src/files/xmlfile.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/files/xmlfile.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/files/xmlfile.o: ../src/visitors/basevisitor.h
../src/files/xmlfile.o: ../src/visitors/xmlvisitor.h
../src/files/xmlfile.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/files/xmlfile.o: ../src/elements/typedefs.h
../src/files/xmlfile.o: ../src/elements/xml/xmlTypedefs.h
../src/files/xmlfile.o: ../src/elements/elements.h
../src/files/xmlfile.o: ../src/elements/xml/xmlElements.h
../src/files/xmlfile.o: ../src/elements/types.h ../src/visitors/visitor.h
../src/files/xmlreader.o: ../src/files/xmlreader.h ../src/elements/exports.h
../src/files/xmlreader.o: ../src/files/xmlfile.h ../src/elements/xml.h
../src/files/xmlreader.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/files/xmlreader.o: ../src/visitors/visitable.h
../src/files/xmlreader.o: ../src/visitors/basevisitor.h
../src/files/xmlreader.o: ../src/parser/reader.h ../src/elements/factory.h
../src/files/xmlreader.o: ../src/lib/functor.h ../src/lib/singleton.h
../src/interface/libmusicxml.o: ../src/interface/libmusicxml.h
../src/interface/libmusicxml.o: ../src/elements/exports.h
../src/interface/libmusicxml.o: ../src/utilities/utilities.h
../src/interface/libmusicxml.o: ../src/lib/smartpointer.h
../src/interface/libmusicxml.o: ../src/visitors/basevisitor.h
../src/interface/libmusicxml.o: ../src/factory/musicxmlfactory.h
../src/interface/libmusicxml.o: ../src/files/xmlfile.h ../src/elements/xml.h
../src/interface/libmusicxml.o: ../src/lib/ctree.h
../src/interface/libmusicxml.o: ../src/visitors/visitable.h
../src/interface/libmusicxml.o: ../src/elements/elements.h
../src/interface/libmusicxml.o: ../src/elements/xml/xmlElements.h
../src/interface/libmusicxml.o: ../src/elements/versions.h
../src/operations/musicxmlquery.o: ../src/operations/musicxmlquery.h
../src/operations/musicxmlquery.o: ../src/elements/exports.h
../src/operations/musicxmlquery.o: ../src/elements/typedefs.h
../src/operations/musicxmlquery.o: ../src/elements/xml/xmlTypedefs.h
../src/operations/musicxmlquery.o: ../src/elements/elements.h
../src/operations/musicxmlquery.o: ../src/elements/xml/xmlElements.h
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
../src/operations/musicxmlquery.o: ../src/utilities/utilities.h
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
../src/operations/transposition.o: ../src/elements/xml/xmlTypedefs.h
../src/operations/transposition.o: ../src/elements/elements.h
../src/operations/transposition.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/clefvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/clefvisitor.o: ../src/elements/elements.h
../src/visitors/clefvisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/keysignvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/keysignvisitor.o: ../src/elements/elements.h
../src/visitors/keysignvisitor.o: ../src/elements/xml/xmlElements.h
../src/visitors/keysignvisitor.o: ../src/elements/types.h
../src/visitors/keysignvisitor.o: ../src/elements/exports.h
../src/visitors/keysignvisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/visitors/keysignvisitor.o: ../src/lib/smartpointer.h
../src/visitors/keysignvisitor.o: ../src/visitors/visitable.h
../src/visitors/keysignvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/keysignvisitor.o: ../src/visitors/visitor.h
../src/visitors/keyvisitor.o: ../src/visitors/keyvisitor.h
../src/visitors/keyvisitor.o: ../src/elements/typedefs.h
../src/visitors/keyvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/keyvisitor.o: ../src/elements/elements.h
../src/visitors/keyvisitor.o: ../src/elements/xml/xmlElements.h
../src/visitors/keyvisitor.o: ../src/elements/types.h
../src/visitors/keyvisitor.o: ../src/elements/exports.h ../src/elements/xml.h
../src/visitors/keyvisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/visitors/keyvisitor.o: ../src/visitors/visitable.h
../src/visitors/keyvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/keyvisitor.o: ../src/visitors/visitor.h
../src/visitors/metronomevisitor.o: ../src/visitors/metronomevisitor.h
../src/visitors/metronomevisitor.o: ../src/elements/typedefs.h
../src/visitors/metronomevisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/metronomevisitor.o: ../src/elements/elements.h
../src/visitors/metronomevisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/midicontextvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/midicontextvisitor.o: ../src/elements/elements.h
../src/visitors/midicontextvisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/notevisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/notevisitor.o: ../src/elements/elements.h
../src/visitors/notevisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/partlistvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/partlistvisitor.o: ../src/elements/elements.h
../src/visitors/partlistvisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/partsummary.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/partsummary.o: ../src/elements/elements.h
../src/visitors/partsummary.o: ../src/elements/xml/xmlElements.h
../src/visitors/partsummary.o: ../src/elements/types.h ../src/elements/xml.h
../src/visitors/partsummary.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/visitors/partsummary.o: ../src/visitors/visitable.h
../src/visitors/partsummary.o: ../src/visitors/basevisitor.h
../src/visitors/partsummary.o: ../src/visitors/visitor.h
../src/visitors/partsummary.o: ../src/lib/smartlist.h
../src/visitors/timesignvisitor.o: ../src/visitors/timesignvisitor.h
../src/visitors/timesignvisitor.o: ../src/elements/typedefs.h
../src/visitors/timesignvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/timesignvisitor.o: ../src/elements/elements.h
../src/visitors/timesignvisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/transposevisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/transposevisitor.o: ../src/elements/elements.h
../src/visitors/transposevisitor.o: ../src/elements/xml/xmlElements.h
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
../src/visitors/unrolled_clonevisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/elements.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/xml/xmlElements.h
../src/visitors/unrolled_clonevisitor.o: ../src/elements/types.h
../src/visitors/xmlvisitor.o: ../src/visitors/xmlvisitor.h
../src/visitors/xmlvisitor.o: ../src/lib/tree_browser.h
../src/visitors/xmlvisitor.o: ../src/elements/exports.h
../src/visitors/xmlvisitor.o: ../src/visitors/basevisitor.h
../src/visitors/xmlvisitor.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/visitors/xmlvisitor.o: ../src/lib/smartpointer.h
../src/visitors/xmlvisitor.o: ../src/visitors/visitable.h
../src/visitors/xmlvisitor.o: ../src/elements/typedefs.h
../src/visitors/xmlvisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/visitors/xmlvisitor.o: ../src/elements/elements.h
../src/visitors/xmlvisitor.o: ../src/elements/xml/xmlElements.h
../src/visitors/xmlvisitor.o: ../src/elements/types.h ../src/elements/xml.h
../src/visitors/xmlvisitor.o: ../src/visitors/visitor.h
../src/utilities/messagesHandling.o: ../src/utilities/utilities.h
../src/utilities/messagesHandling.o: ../src/lib/smartpointer.h
../src/utilities/messagesHandling.o: ../src/elements/exports.h
../src/utilities/messagesHandling.o: ../src/visitors/basevisitor.h
../src/utilities/messagesHandling.o: ../src/oah/enableAbortOnInternalErrors.h
../src/utilities/messagesHandling.o: ../src/utilities/messagesHandling.h
../src/utilities/messagesHandling.o: ../src/utilities/exceptions.h
../src/utilities/messagesHandling.o: ../src/oah/generalOah.h
../src/utilities/messagesHandling.o: ../src/oah/oahBasicTypes.h
../src/utilities/messagesHandling.o: ../src/interface/libmusicxml.h
../src/utilities/messagesHandling.o: ../src/oah/oahElements.h
../src/utilities/messagesHandling.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/utilities/messagesHandling.o: ../src/visitors/visitable.h
../src/utilities/utilities.o: ../src/lib/rational.h ../src/elements/exports.h
../src/utilities/utilities.o: ../src/utilities/utilities.h
../src/utilities/utilities.o: ../src/lib/smartpointer.h
../src/utilities/utilities.o: ../src/visitors/basevisitor.h
../src/utilities/utilities.o: ../src/oah/oahOah.h
../src/utilities/utilities.o: ../src/oah/oahAtomsCollection.h
../src/utilities/utilities.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/utilities/utilities.o: ../src/elements/typedefs.h
../src/utilities/utilities.o: ../src/elements/xml/xmlTypedefs.h
../src/utilities/utilities.o: ../src/elements/elements.h
../src/utilities/utilities.o: ../src/elements/xml/xmlElements.h
../src/utilities/utilities.o: ../src/elements/types.h ../src/elements/xml.h
../src/utilities/utilities.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/utilities/utilities.o: ../src/visitors/visitor.h
../src/utilities/utilities.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/utilities/utilities.o: ../src/msr/msrBasicTypes.h
../src/utilities/utilities.o: ../src/oah/oahBasicTypes.h
../src/utilities/utilities.o: ../src/interface/libmusicxml.h
../src/utilities/utilities.o: ../src/oah/oahElements.h
../src/utilities/version.o: ../src/utilities/version.h
../src/utilities/version.o: ../src/elements/exports.h
../src/utilities/version.o: ../src/utilities/utilities.h
../src/utilities/version.o: ../src/lib/smartpointer.h
../src/utilities/version.o: ../src/visitors/basevisitor.h
../src/musicxml/musicxmlOah.o: ../src/utilities/version.h
../src/musicxml/musicxmlOah.o: ../src/elements/exports.h
../src/musicxml/musicxmlOah.o: ../src/utilities/utilities.h
../src/musicxml/musicxmlOah.o: ../src/lib/smartpointer.h
../src/musicxml/musicxmlOah.o: ../src/visitors/basevisitor.h
../src/musicxml/musicxmlOah.o: ../src/oah/enableTracingIfDesired.h
../src/musicxml/musicxmlOah.o: ../src/oah/traceOah.h
../src/musicxml/musicxmlOah.o: ../src/oah/oahBasicTypes.h
../src/musicxml/musicxmlOah.o: ../src/interface/libmusicxml.h
../src/musicxml/musicxmlOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/musicxml/musicxmlOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/musicxml/musicxmlOah.o: ../src/utilities/messagesHandling.h
../src/musicxml/musicxmlOah.o: ../src/utilities/exceptions.h
../src/musicxml/musicxmlOah.o: ../src/oah/oahOah.h
../src/musicxml/musicxmlOah.o: ../src/oah/oahAtomsCollection.h
../src/musicxml/musicxmlOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/musicxml/musicxmlOah.o: ../src/msr/msrElements.h
../src/musicxml/musicxmlOah.o: ../src/elements/typedefs.h
../src/musicxml/musicxmlOah.o: ../src/elements/xml/xmlTypedefs.h
../src/musicxml/musicxmlOah.o: ../src/elements/elements.h
../src/musicxml/musicxmlOah.o: ../src/elements/xml/xmlElements.h
../src/musicxml/musicxmlOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/musicxml/musicxmlOah.o: ../src/visitors/visitor.h
../src/musicxml/musicxmlOah.o: ../src/lib/tree_browser.h
../src/musicxml/musicxmlOah.o: ../src/msr/msrBasicTypes.h
../src/musicxml/musicxmlOah.o: ../src/oah/generalOah.h
../src/musicxml/musicxmlOah.o: ../src/musicxml/musicxmlOah.h
../src/msr/msr.o: ../src/msr/msr.h ../src/elements/typedefs.h
../src/msr/msr.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msr.o: ../src/elements/elements.h
../src/msr/msr.o: ../src/elements/xml/xmlElements.h ../src/elements/types.h
../src/msr/msr.o: ../src/elements/exports.h ../src/elements/xml.h
../src/msr/msr.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msr.o: ../src/visitors/visitable.h ../src/visitors/basevisitor.h
../src/msr/msr.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msr.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msr.o: ../src/utilities/messagesHandling.h
../src/msr/msr.o: ../src/utilities/exceptions.h ../src/msr/msrBasicTypes.h
../src/msr/msr.o: ../src/lib/rational.h ../src/msr/msrElements.h
../src/msr/msr.o: ../src/msr/msrVarValAssocs.h ../src/msr/msrIdentification.h
../src/msr/msr.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msr.o: ../src/msr/msrMeasureElements.h ../src/msr/msrScaling.h
../src/msr/msr.o: ../src/msr/msrCredits.h ../src/msr/msrDivisions.h
../src/msr/msr.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msr.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msr.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msr.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msr.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msr.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msr.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msr.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msr.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msr.o: ../src/msr/msrArticulations.h ../src/msr/msrTablatures.h
../src/msr/msr.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msr.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msr.o: ../src/msr/msrSingleTremolos.h ../src/msr/msrSlashes.h
../src/msr/msr.o: ../src/msr/msrScores.h ../src/msr/msrPartGroups.h
../src/msr/msr.o: ../src/msr/msrPartGroupElements.h
../src/msr/msr.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msr.o: ../src/msr/msrTupletElements.h
../src/msr/msr.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/msr/msr.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msr.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msr.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msr.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/oah/oahOah.h ../src/elements/exports.h
../src/msr/msr2namesVisitor.o: ../src/oah/oahAtomsCollection.h
../src/msr/msr2namesVisitor.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrElements.h
../src/msr/msr2namesVisitor.o: ../src/elements/typedefs.h
../src/msr/msr2namesVisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msr2namesVisitor.o: ../src/elements/elements.h
../src/msr/msr2namesVisitor.o: ../src/elements/xml/xmlElements.h
../src/msr/msr2namesVisitor.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msr2namesVisitor.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msr2namesVisitor.o: ../src/visitors/visitable.h
../src/msr/msr2namesVisitor.o: ../src/visitors/basevisitor.h
../src/msr/msr2namesVisitor.o: ../src/visitors/visitor.h
../src/msr/msr2namesVisitor.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrBasicTypes.h
../src/msr/msr2namesVisitor.o: ../src/oah/oahBasicTypes.h
../src/msr/msr2namesVisitor.o: ../src/interface/libmusicxml.h
../src/msr/msr2namesVisitor.o: ../src/utilities/utilities.h
../src/msr/msr2namesVisitor.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msr2namesVisitor.o: ../src/msr/msr2namesVisitor.h ../src/msr/msr.h
../src/msr/msr2namesVisitor.o: ../src/utilities/messagesHandling.h
../src/msr/msr2namesVisitor.o: ../src/utilities/exceptions.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrVarValAssocs.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrIdentification.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrMeasureElements.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrScaling.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrCredits.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrDivisions.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrStavesDetails.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrRehearsals.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTranspositions.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTechnicals.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrArticulations.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTablatures.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrInstruments.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrLigatures.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSingleTremolos.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrPartGroups.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrPartGroupElements.h
../src/msr/msr2namesVisitor.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTupletElements.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrVoiceElements.h
../src/msr/msr2namesVisitor.o: ../src/oah/generalOah.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msr2namesVisitor.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/oah/oahOah.h
../src/msr/msr2summaryVisitor.o: ../src/elements/exports.h
../src/msr/msr2summaryVisitor.o: ../src/oah/oahAtomsCollection.h
../src/msr/msr2summaryVisitor.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrElements.h
../src/msr/msr2summaryVisitor.o: ../src/elements/typedefs.h
../src/msr/msr2summaryVisitor.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msr2summaryVisitor.o: ../src/elements/elements.h
../src/msr/msr2summaryVisitor.o: ../src/elements/xml/xmlElements.h
../src/msr/msr2summaryVisitor.o: ../src/elements/types.h
../src/msr/msr2summaryVisitor.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msr2summaryVisitor.o: ../src/lib/smartpointer.h
../src/msr/msr2summaryVisitor.o: ../src/visitors/visitable.h
../src/msr/msr2summaryVisitor.o: ../src/visitors/basevisitor.h
../src/msr/msr2summaryVisitor.o: ../src/visitors/visitor.h
../src/msr/msr2summaryVisitor.o: ../src/lib/tree_browser.h
../src/msr/msr2summaryVisitor.o: ../src/lib/browser.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrBasicTypes.h
../src/msr/msr2summaryVisitor.o: ../src/oah/oahBasicTypes.h
../src/msr/msr2summaryVisitor.o: ../src/interface/libmusicxml.h
../src/msr/msr2summaryVisitor.o: ../src/utilities/utilities.h
../src/msr/msr2summaryVisitor.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msr2summaryVisitor.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msr.h
../src/msr/msr2summaryVisitor.o: ../src/utilities/messagesHandling.h
../src/msr/msr2summaryVisitor.o: ../src/utilities/exceptions.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrVarValAssocs.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrIdentification.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrMeasureElements.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrScaling.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrCredits.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrDivisions.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrStavesDetails.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrRehearsals.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTranspositions.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTechnicals.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrBars.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrDynamics.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrArticulations.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTablatures.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrInstruments.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrLigatures.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSingleTremolos.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSlashes.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrScores.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrPartGroups.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrPartGroupElements.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTupletElements.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrVoiceElements.h
../src/msr/msr2summaryVisitor.o: ../src/oah/generalOah.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msr2summaryVisitor.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/typedefs.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/elements.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/types.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/exports.h
../src/msr/msrAfterGraceNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrAfterGraceNotes.o: ../src/lib/smartpointer.h
../src/msr/msrAfterGraceNotes.o: ../src/visitors/visitable.h
../src/msr/msrAfterGraceNotes.o: ../src/visitors/basevisitor.h
../src/msr/msrAfterGraceNotes.o: ../src/visitors/visitor.h
../src/msr/msrAfterGraceNotes.o: ../src/lib/tree_browser.h
../src/msr/msrAfterGraceNotes.o: ../src/lib/browser.h
../src/msr/msrAfterGraceNotes.o: ../src/utilities/utilities.h
../src/msr/msrAfterGraceNotes.o: ../src/utilities/messagesHandling.h
../src/msr/msrAfterGraceNotes.o: ../src/utilities/exceptions.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrElements.h
../src/msr/msrAfterGraceNotes.o: ../src/lib/rational.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTupletElements.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrMeasureElements.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrBasicTypes.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrVoiceElements.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrIdentification.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrScaling.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrCredits.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrDivisions.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrStavesDetails.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrRehearsals.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTranspositions.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTechnicals.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrBars.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrDynamics.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrArticulations.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTablatures.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrInstruments.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrLigatures.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSlashes.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrPartGroups.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/generalOah.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/oahBasicTypes.h
../src/msr/msrAfterGraceNotes.o: ../src/interface/libmusicxml.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/oahElements.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrAfterGraceNotes.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrAfterGraceNotes.o: ../src/msr/msrOah.h
../src/msr/msrArpeggiatos.o: ../src/msr/msrArpeggiatos.h
../src/msr/msrArpeggiatos.o: ../src/msr/msrElements.h
../src/msr/msrArpeggiatos.o: ../src/elements/typedefs.h
../src/msr/msrArpeggiatos.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrArpeggiatos.o: ../src/elements/elements.h
../src/msr/msrArpeggiatos.o: ../src/elements/xml/xmlElements.h
../src/msr/msrArpeggiatos.o: ../src/elements/types.h
../src/msr/msrArpeggiatos.o: ../src/elements/exports.h ../src/elements/xml.h
../src/msr/msrArpeggiatos.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msrArpeggiatos.o: ../src/visitors/visitable.h
../src/msr/msrArpeggiatos.o: ../src/visitors/basevisitor.h
../src/msr/msrArpeggiatos.o: ../src/visitors/visitor.h
../src/msr/msrArpeggiatos.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrArpeggiatos.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrArpeggiatos.o: ../src/msr/msrOah.h
../src/msr/msrArpeggiatos.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrArpeggiatos.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrArpeggiatos.o: ../src/interface/libmusicxml.h
../src/msr/msrArpeggiatos.o: ../src/utilities/utilities.h
../src/msr/msrArpeggiatos.o: ../src/oah/oahElements.h
../src/msr/msrArticulations.o: ../src/utilities/utilities.h
../src/msr/msrArticulations.o: ../src/lib/smartpointer.h
../src/msr/msrArticulations.o: ../src/elements/exports.h
../src/msr/msrArticulations.o: ../src/visitors/basevisitor.h
../src/msr/msrArticulations.o: ../src/msr/msrArticulations.h
../src/msr/msrArticulations.o: ../src/msr/msrElements.h
../src/msr/msrArticulations.o: ../src/elements/typedefs.h
../src/msr/msrArticulations.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrArticulations.o: ../src/elements/elements.h
../src/msr/msrArticulations.o: ../src/elements/xml/xmlElements.h
../src/msr/msrArticulations.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrArticulations.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrArticulations.o: ../src/visitors/visitor.h
../src/msr/msrArticulations.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrArticulations.o: ../src/lib/rational.h
../src/msr/msrArticulations.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrArticulations.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrArticulations.o: ../src/msr/msrMidi.h
../src/msr/msrArticulations.o: ../src/oah/oahBasicTypes.h
../src/msr/msrArticulations.o: ../src/interface/libmusicxml.h
../src/msr/msrArticulations.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrBars.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrBars.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/msr/msrBars.o: ../src/msr/msrBars.h ../src/msr/msrSegnosAndCodas.h
../src/msr/msrBars.o: ../src/msr/msrMeasureElements.h
../src/msr/msrBars.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrBars.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrBars.o: ../src/elements/elements.h
../src/msr/msrBars.o: ../src/elements/xml/xmlElements.h
../src/msr/msrBars.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrBars.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrBars.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrBars.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrBars.o: ../src/msr/msrBasicTypes.h
../src/msr/msrBars.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrBars.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrBars.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msrBars.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/msr/msrBars.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrBasicTypes.o: ../src/lib/rational.h ../src/elements/exports.h
../src/msr/msrBasicTypes.o: ../src/utilities/utilities.h
../src/msr/msrBasicTypes.o: ../src/lib/smartpointer.h
../src/msr/msrBasicTypes.o: ../src/visitors/basevisitor.h
../src/msr/msrBasicTypes.o: ../src/utilities/messagesHandling.h
../src/msr/msrBasicTypes.o: ../src/utilities/exceptions.h
../src/msr/msrBasicTypes.o: ../src/msr/msrBasicTypes.h
../src/msr/msrBasicTypes.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrBasicTypes.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrBasicTypes.o: ../src/interface/libmusicxml.h
../src/msr/msrBasicTypes.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/msr/msrBasicTypes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrBasicTypes.o: ../src/oah/oahOah.h
../src/msr/msrBasicTypes.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrBasicTypes.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/msr/msrBasicTypes.o: ../src/elements/typedefs.h
../src/msr/msrBasicTypes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrBasicTypes.o: ../src/elements/elements.h
../src/msr/msrBasicTypes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrBasicTypes.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrBasicTypes.o: ../src/visitors/visitor.h
../src/msr/msrBasicTypes.o: ../src/lib/tree_browser.h ../src/oah/generalOah.h
../src/msr/msrBasicTypes.o: ../src/msr/msrOah.h ../src/lpsr/lpsrOah.h
../src/msr/msrBasicTypes.o: ../src/lpsr/lpsrBasicTypes.h
../src/msr/msrBeams.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrBeams.o: ../src/elements/exports.h
../src/msr/msrBeams.o: ../src/visitors/basevisitor.h ../src/msr/msrBeams.h
../src/msr/msrBeams.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrBeams.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrBeams.o: ../src/elements/elements.h
../src/msr/msrBeams.o: ../src/elements/xml/xmlElements.h
../src/msr/msrBeams.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrBeams.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrBeams.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrBeams.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrBeams.o: ../src/msr/msrBasicTypes.h
../src/msr/msrBeams.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrBeams.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrBeams.o: ../src/interface/libmusicxml.h
../src/msr/msrBeams.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrBeams.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrBeams.o: ../src/msr/msrOah.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrBeatsRepeats.o: ../src/elements/typedefs.h
../src/msr/msrBeatsRepeats.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrBeatsRepeats.o: ../src/elements/elements.h
../src/msr/msrBeatsRepeats.o: ../src/elements/xml/xmlElements.h
../src/msr/msrBeatsRepeats.o: ../src/elements/types.h
../src/msr/msrBeatsRepeats.o: ../src/elements/exports.h ../src/elements/xml.h
../src/msr/msrBeatsRepeats.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msrBeatsRepeats.o: ../src/visitors/visitable.h
../src/msr/msrBeatsRepeats.o: ../src/visitors/basevisitor.h
../src/msr/msrBeatsRepeats.o: ../src/visitors/visitor.h
../src/msr/msrBeatsRepeats.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrBeatsRepeats.o: ../src/utilities/utilities.h
../src/msr/msrBeatsRepeats.o: ../src/utilities/messagesHandling.h
../src/msr/msrBeatsRepeats.o: ../src/utilities/exceptions.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTupletElements.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrMeasureElements.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrBasicTypes.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrVoiceElements.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrIdentification.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrDivisions.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrStavesDetails.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrRehearsals.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTranspositions.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrDynamics.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrArticulations.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTablatures.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrInstruments.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrLigatures.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSlashes.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrPartGroups.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrBeatsRepeats.o: ../src/oah/generalOah.h
../src/msr/msrBeatsRepeats.o: ../src/oah/oahBasicTypes.h
../src/msr/msrBeatsRepeats.o: ../src/interface/libmusicxml.h
../src/msr/msrBeatsRepeats.o: ../src/oah/oahElements.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrBeatsRepeats.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrBeatsRepeats.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrBeatsRepeats.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrBeatsRepeats.o: ../src/msr/msrOah.h
../src/msr/msrBreaks.o: ../src/utilities/utilities.h
../src/msr/msrBreaks.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrBreaks.o: ../src/visitors/basevisitor.h ../src/msr/msrBreaks.h
../src/msr/msrBreaks.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrBreaks.o: ../src/msr/msrMeasureElements.h
../src/msr/msrBreaks.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrBreaks.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrBreaks.o: ../src/elements/elements.h
../src/msr/msrBreaks.o: ../src/elements/xml/xmlElements.h
../src/msr/msrBreaks.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrBreaks.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrBreaks.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrBreaks.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrBreaks.o: ../src/msr/msrBasicTypes.h
../src/msr/msrBreaks.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrBreaks.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrBreaks.o: ../src/interface/libmusicxml.h
../src/msr/msrBreaks.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrBreaks.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrBreaks.o: ../src/msr/msrOah.h
../src/msr/msrChords.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrChords.o: ../src/elements/typedefs.h
../src/msr/msrChords.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrChords.o: ../src/elements/elements.h
../src/msr/msrChords.o: ../src/elements/xml/xmlElements.h
../src/msr/msrChords.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrChords.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrChords.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrChords.o: ../src/visitors/basevisitor.h
../src/msr/msrChords.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrChords.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrChords.o: ../src/utilities/messagesHandling.h
../src/msr/msrChords.o: ../src/utilities/exceptions.h
../src/msr/msrChords.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrChords.o: ../src/msr/msrTupletElements.h
../src/msr/msrChords.o: ../src/msr/msrMeasureElements.h
../src/msr/msrChords.o: ../src/msr/msrBasicTypes.h
../src/msr/msrChords.o: ../src/msr/msrVoiceElements.h
../src/msr/msrChords.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrChords.o: ../src/msr/msrIdentification.h
../src/msr/msrChords.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrChords.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrChords.o: ../src/msr/msrDivisions.h
../src/msr/msrChords.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrChords.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrChords.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrChords.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrChords.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrChords.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrChords.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrChords.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrChords.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrChords.o: ../src/msr/msrArticulations.h
../src/msr/msrChords.o: ../src/msr/msrTablatures.h
../src/msr/msrChords.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrChords.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrChords.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrChords.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrChords.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrChords.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrChords.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrChords.o: ../src/interface/libmusicxml.h
../src/msr/msrChords.o: ../src/oah/oahElements.h
../src/msr/msrChords.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrChords.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrChords.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrChords.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrChords.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrClefs.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrClefs.o: ../src/elements/exports.h
../src/msr/msrClefs.o: ../src/visitors/basevisitor.h ../src/msr/msrClefs.h
../src/msr/msrClefs.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrClefs.o: ../src/msr/msrMeasureElements.h
../src/msr/msrClefs.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrClefs.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrClefs.o: ../src/elements/elements.h
../src/msr/msrClefs.o: ../src/elements/xml/xmlElements.h
../src/msr/msrClefs.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrClefs.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrClefs.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrClefs.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrClefs.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrClefs.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrClefs.o: ../src/oah/oahBasicTypes.h
../src/msr/msrClefs.o: ../src/interface/libmusicxml.h
../src/msr/msrClefs.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrCredits.o: ../src/utilities/utilities.h
../src/msr/msrCredits.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrCredits.o: ../src/visitors/basevisitor.h
../src/msr/msrCredits.o: ../src/msr/msrCredits.h ../src/msr/msrElements.h
../src/msr/msrCredits.o: ../src/elements/typedefs.h
../src/msr/msrCredits.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrCredits.o: ../src/elements/elements.h
../src/msr/msrCredits.o: ../src/elements/xml/xmlElements.h
../src/msr/msrCredits.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrCredits.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrCredits.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrCredits.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrCredits.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrCredits.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrCredits.o: ../src/oah/oahBasicTypes.h
../src/msr/msrCredits.o: ../src/interface/libmusicxml.h
../src/msr/msrCredits.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrDivisions.o: ../src/utilities/utilities.h
../src/msr/msrDivisions.o: ../src/lib/smartpointer.h
../src/msr/msrDivisions.o: ../src/elements/exports.h
../src/msr/msrDivisions.o: ../src/visitors/basevisitor.h
../src/msr/msrDivisions.o: ../src/msr/msrDivisions.h ../src/msr/msrElements.h
../src/msr/msrDivisions.o: ../src/elements/typedefs.h
../src/msr/msrDivisions.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrDivisions.o: ../src/elements/elements.h
../src/msr/msrDivisions.o: ../src/elements/xml/xmlElements.h
../src/msr/msrDivisions.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrDivisions.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrDivisions.o: ../src/visitors/visitor.h
../src/msr/msrDivisions.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrDivisions.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrDivisions.o: ../src/oah/oahOah.h
../src/msr/msrDivisions.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrDivisions.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrDivisions.o: ../src/interface/libmusicxml.h
../src/msr/msrDivisions.o: ../src/oah/oahElements.h ../src/oah/generalOah.h
../src/msr/msrDivisions.o: ../src/musicxml/musicxmlOah.h ../src/msr/msrOah.h
../src/msr/msrDivisions.o: ../src/utilities/messagesHandling.h
../src/msr/msrDivisions.o: ../src/utilities/exceptions.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrDoubleTremolos.o: ../src/elements/typedefs.h
../src/msr/msrDoubleTremolos.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrDoubleTremolos.o: ../src/elements/elements.h
../src/msr/msrDoubleTremolos.o: ../src/elements/xml/xmlElements.h
../src/msr/msrDoubleTremolos.o: ../src/elements/types.h
../src/msr/msrDoubleTremolos.o: ../src/elements/exports.h
../src/msr/msrDoubleTremolos.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrDoubleTremolos.o: ../src/lib/smartpointer.h
../src/msr/msrDoubleTremolos.o: ../src/visitors/visitable.h
../src/msr/msrDoubleTremolos.o: ../src/visitors/basevisitor.h
../src/msr/msrDoubleTremolos.o: ../src/visitors/visitor.h
../src/msr/msrDoubleTremolos.o: ../src/lib/tree_browser.h
../src/msr/msrDoubleTremolos.o: ../src/lib/browser.h
../src/msr/msrDoubleTremolos.o: ../src/utilities/utilities.h
../src/msr/msrDoubleTremolos.o: ../src/utilities/messagesHandling.h
../src/msr/msrDoubleTremolos.o: ../src/utilities/exceptions.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrElements.h
../src/msr/msrDoubleTremolos.o: ../src/lib/rational.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTupletElements.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrMeasureElements.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrBasicTypes.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrVoiceElements.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrIdentification.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrScaling.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrCredits.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrDivisions.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrStavesDetails.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrRehearsals.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTranspositions.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTechnicals.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrArticulations.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTablatures.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrInstruments.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrLigatures.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSlashes.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrPartGroups.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrDoubleTremolos.o: ../src/oah/generalOah.h
../src/msr/msrDoubleTremolos.o: ../src/oah/oahBasicTypes.h
../src/msr/msrDoubleTremolos.o: ../src/interface/libmusicxml.h
../src/msr/msrDoubleTremolos.o: ../src/oah/oahElements.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrDoubleTremolos.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrDoubleTremolos.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrDoubleTremolos.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrDoubleTremolos.o: ../src/msr/msrOah.h
../src/msr/msrDynamics.o: ../src/utilities/utilities.h
../src/msr/msrDynamics.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrDynamics.o: ../src/visitors/basevisitor.h
../src/msr/msrDynamics.o: ../src/msr/msrDynamics.h ../src/msr/msrElements.h
../src/msr/msrDynamics.o: ../src/elements/typedefs.h
../src/msr/msrDynamics.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrDynamics.o: ../src/elements/elements.h
../src/msr/msrDynamics.o: ../src/elements/xml/xmlElements.h
../src/msr/msrDynamics.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrDynamics.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrDynamics.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrDynamics.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrDynamics.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrDynamics.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrDynamics.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrDynamics.o: ../src/interface/libmusicxml.h
../src/msr/msrDynamics.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrElements.o: ../src/utilities/utilities.h
../src/msr/msrElements.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrElements.o: ../src/visitors/basevisitor.h
../src/msr/msrElements.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrElements.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrElements.o: ../src/elements/elements.h
../src/msr/msrElements.o: ../src/elements/xml/xmlElements.h
../src/msr/msrElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrElements.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrElements.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrElements.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/msr/msrElements.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/msr/msrElements.o: ../src/oah/oahBasicTypes.h
../src/msr/msrElements.o: ../src/interface/libmusicxml.h
../src/msr/msrElements.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrFiguredBasses.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrFiguredBasses.o: ../src/elements/typedefs.h
../src/msr/msrFiguredBasses.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrFiguredBasses.o: ../src/elements/elements.h
../src/msr/msrFiguredBasses.o: ../src/elements/xml/xmlElements.h
../src/msr/msrFiguredBasses.o: ../src/elements/types.h
../src/msr/msrFiguredBasses.o: ../src/elements/exports.h
../src/msr/msrFiguredBasses.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrFiguredBasses.o: ../src/lib/smartpointer.h
../src/msr/msrFiguredBasses.o: ../src/visitors/visitable.h
../src/msr/msrFiguredBasses.o: ../src/visitors/basevisitor.h
../src/msr/msrFiguredBasses.o: ../src/visitors/visitor.h
../src/msr/msrFiguredBasses.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrFiguredBasses.o: ../src/utilities/utilities.h
../src/msr/msrFiguredBasses.o: ../src/utilities/messagesHandling.h
../src/msr/msrFiguredBasses.o: ../src/utilities/exceptions.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTupletElements.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrMeasureElements.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrBasicTypes.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrVoiceElements.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrIdentification.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrScaling.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrCredits.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrDivisions.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrStavesDetails.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrRehearsals.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTranspositions.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTechnicals.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrArticulations.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTablatures.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrInstruments.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrLigatures.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSlashes.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrPartGroups.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrFiguredBasses.o: ../src/oah/generalOah.h
../src/msr/msrFiguredBasses.o: ../src/oah/oahBasicTypes.h
../src/msr/msrFiguredBasses.o: ../src/interface/libmusicxml.h
../src/msr/msrFiguredBasses.o: ../src/oah/oahElements.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrFiguredBasses.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrFiguredBasses.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrFiguredBasses.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrFiguredBasses.o: ../src/msr/msrOah.h
../src/msr/msrGlissandos.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrGlissandos.o: ../src/elements/typedefs.h
../src/msr/msrGlissandos.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrGlissandos.o: ../src/elements/elements.h
../src/msr/msrGlissandos.o: ../src/elements/xml/xmlElements.h
../src/msr/msrGlissandos.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrGlissandos.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrGlissandos.o: ../src/lib/smartpointer.h
../src/msr/msrGlissandos.o: ../src/visitors/visitable.h
../src/msr/msrGlissandos.o: ../src/visitors/basevisitor.h
../src/msr/msrGlissandos.o: ../src/visitors/visitor.h
../src/msr/msrGlissandos.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrGlissandos.o: ../src/utilities/utilities.h
../src/msr/msrGlissandos.o: ../src/utilities/messagesHandling.h
../src/msr/msrGlissandos.o: ../src/utilities/exceptions.h
../src/msr/msrGlissandos.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrGlissandos.o: ../src/msr/msrTupletElements.h
../src/msr/msrGlissandos.o: ../src/msr/msrMeasureElements.h
../src/msr/msrGlissandos.o: ../src/msr/msrBasicTypes.h
../src/msr/msrGlissandos.o: ../src/msr/msrVoiceElements.h
../src/msr/msrGlissandos.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrGlissandos.o: ../src/msr/msrIdentification.h
../src/msr/msrGlissandos.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrGlissandos.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrGlissandos.o: ../src/msr/msrDivisions.h
../src/msr/msrGlissandos.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrGlissandos.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrGlissandos.o: ../src/msr/msrStavesDetails.h
../src/msr/msrGlissandos.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrGlissandos.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrGlissandos.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrGlissandos.o: ../src/msr/msrRehearsals.h
../src/msr/msrGlissandos.o: ../src/msr/msrTranspositions.h
../src/msr/msrGlissandos.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrGlissandos.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrGlissandos.o: ../src/msr/msrDynamics.h
../src/msr/msrGlissandos.o: ../src/msr/msrArticulations.h
../src/msr/msrGlissandos.o: ../src/msr/msrTablatures.h
../src/msr/msrGlissandos.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrGlissandos.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrGlissandos.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrGlissandos.o: ../src/msr/msrSlashes.h
../src/msr/msrGlissandos.o: ../src/msr/msrPartGroups.h
../src/msr/msrGlissandos.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrGlissandos.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrGlissandos.o: ../src/oah/generalOah.h
../src/msr/msrGlissandos.o: ../src/oah/oahBasicTypes.h
../src/msr/msrGlissandos.o: ../src/interface/libmusicxml.h
../src/msr/msrGlissandos.o: ../src/oah/oahElements.h
../src/msr/msrGlissandos.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrGlissandos.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrGlissandos.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrGlissandos.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrGlissandos.o: ../src/msr/msrOah.h
../src/msr/msrGraceNotes.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrGraceNotes.o: ../src/elements/typedefs.h
../src/msr/msrGraceNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrGraceNotes.o: ../src/elements/elements.h
../src/msr/msrGraceNotes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrGraceNotes.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrGraceNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrGraceNotes.o: ../src/lib/smartpointer.h
../src/msr/msrGraceNotes.o: ../src/visitors/visitable.h
../src/msr/msrGraceNotes.o: ../src/visitors/basevisitor.h
../src/msr/msrGraceNotes.o: ../src/visitors/visitor.h
../src/msr/msrGraceNotes.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrGraceNotes.o: ../src/utilities/utilities.h
../src/msr/msrGraceNotes.o: ../src/utilities/messagesHandling.h
../src/msr/msrGraceNotes.o: ../src/utilities/exceptions.h
../src/msr/msrGraceNotes.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTupletElements.h
../src/msr/msrGraceNotes.o: ../src/msr/msrMeasureElements.h
../src/msr/msrGraceNotes.o: ../src/msr/msrBasicTypes.h
../src/msr/msrGraceNotes.o: ../src/msr/msrVoiceElements.h
../src/msr/msrGraceNotes.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrGraceNotes.o: ../src/msr/msrIdentification.h
../src/msr/msrGraceNotes.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrGraceNotes.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrGraceNotes.o: ../src/msr/msrDivisions.h
../src/msr/msrGraceNotes.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrGraceNotes.o: ../src/msr/msrStavesDetails.h
../src/msr/msrGraceNotes.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrGraceNotes.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrGraceNotes.o: ../src/msr/msrRehearsals.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTranspositions.h
../src/msr/msrGraceNotes.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrGraceNotes.o: ../src/msr/msrDynamics.h
../src/msr/msrGraceNotes.o: ../src/msr/msrArticulations.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTablatures.h
../src/msr/msrGraceNotes.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSlashes.h
../src/msr/msrGraceNotes.o: ../src/msr/msrPartGroups.h
../src/msr/msrGraceNotes.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrGraceNotes.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrGraceNotes.o: ../src/oah/generalOah.h
../src/msr/msrGraceNotes.o: ../src/oah/oahBasicTypes.h
../src/msr/msrGraceNotes.o: ../src/interface/libmusicxml.h
../src/msr/msrGraceNotes.o: ../src/oah/oahElements.h
../src/msr/msrGraceNotes.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrGraceNotes.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrGraceNotes.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrGraceNotes.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrGraceNotes.o: ../src/msr/msrOah.h
../src/msr/msrHarmonies.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrHarmonies.o: ../src/elements/typedefs.h
../src/msr/msrHarmonies.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrHarmonies.o: ../src/elements/elements.h
../src/msr/msrHarmonies.o: ../src/elements/xml/xmlElements.h
../src/msr/msrHarmonies.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrHarmonies.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrHarmonies.o: ../src/lib/smartpointer.h
../src/msr/msrHarmonies.o: ../src/visitors/visitable.h
../src/msr/msrHarmonies.o: ../src/visitors/basevisitor.h
../src/msr/msrHarmonies.o: ../src/visitors/visitor.h
../src/msr/msrHarmonies.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrHarmonies.o: ../src/utilities/utilities.h
../src/msr/msrHarmonies.o: ../src/utilities/messagesHandling.h
../src/msr/msrHarmonies.o: ../src/utilities/exceptions.h
../src/msr/msrHarmonies.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrHarmonies.o: ../src/msr/msrTupletElements.h
../src/msr/msrHarmonies.o: ../src/msr/msrMeasureElements.h
../src/msr/msrHarmonies.o: ../src/msr/msrBasicTypes.h
../src/msr/msrHarmonies.o: ../src/msr/msrVoiceElements.h
../src/msr/msrHarmonies.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrHarmonies.o: ../src/msr/msrIdentification.h
../src/msr/msrHarmonies.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrHarmonies.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrHarmonies.o: ../src/msr/msrDivisions.h
../src/msr/msrHarmonies.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrHarmonies.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrHarmonies.o: ../src/msr/msrStavesDetails.h
../src/msr/msrHarmonies.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrHarmonies.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrHarmonies.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrHarmonies.o: ../src/msr/msrRehearsals.h
../src/msr/msrHarmonies.o: ../src/msr/msrTranspositions.h
../src/msr/msrHarmonies.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrHarmonies.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrHarmonies.o: ../src/msr/msrDynamics.h
../src/msr/msrHarmonies.o: ../src/msr/msrArticulations.h
../src/msr/msrHarmonies.o: ../src/msr/msrTablatures.h
../src/msr/msrHarmonies.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrHarmonies.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrHarmonies.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrHarmonies.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrHarmonies.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrHarmonies.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrHarmonies.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrHarmonies.o: ../src/interface/libmusicxml.h
../src/msr/msrHarmonies.o: ../src/oah/oahElements.h
../src/msr/msrHarmonies.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrHarmonies.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrHarmonies.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrHarmonies.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrHarmonies.o: ../src/msr/msrOah.h
../src/msr/msrIdentification.o: ../src/msr/msrIdentification.h
../src/msr/msrIdentification.o: ../src/msr/msrElements.h
../src/msr/msrIdentification.o: ../src/elements/typedefs.h
../src/msr/msrIdentification.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrIdentification.o: ../src/elements/elements.h
../src/msr/msrIdentification.o: ../src/elements/xml/xmlElements.h
../src/msr/msrIdentification.o: ../src/elements/types.h
../src/msr/msrIdentification.o: ../src/elements/exports.h
../src/msr/msrIdentification.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrIdentification.o: ../src/lib/smartpointer.h
../src/msr/msrIdentification.o: ../src/visitors/visitable.h
../src/msr/msrIdentification.o: ../src/visitors/basevisitor.h
../src/msr/msrIdentification.o: ../src/visitors/visitor.h
../src/msr/msrIdentification.o: ../src/lib/tree_browser.h
../src/msr/msrIdentification.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrIdentification.o: ../src/msr/msrBasicTypes.h
../src/msr/msrIdentification.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrIdentification.o: ../src/utilities/utilities.h
../src/msr/msrIdentification.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrIdentification.o: ../src/oah/traceOah.h
../src/msr/msrIdentification.o: ../src/oah/oahBasicTypes.h
../src/msr/msrIdentification.o: ../src/interface/libmusicxml.h
../src/msr/msrIdentification.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrIdentification.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrIdentification.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrInstruments.o: ../src/utilities/utilities.h
../src/msr/msrInstruments.o: ../src/lib/smartpointer.h
../src/msr/msrInstruments.o: ../src/elements/exports.h
../src/msr/msrInstruments.o: ../src/visitors/basevisitor.h
../src/msr/msrInstruments.o: ../src/utilities/messagesHandling.h
../src/msr/msrInstruments.o: ../src/utilities/exceptions.h
../src/msr/msrInstruments.o: ../src/msr/msrInstruments.h
../src/msr/msrInstruments.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrInstruments.o: ../src/msr/msrMeasureElements.h
../src/msr/msrInstruments.o: ../src/msr/msrElements.h
../src/msr/msrInstruments.o: ../src/elements/typedefs.h
../src/msr/msrInstruments.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrInstruments.o: ../src/elements/elements.h
../src/msr/msrInstruments.o: ../src/elements/xml/xmlElements.h
../src/msr/msrInstruments.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrInstruments.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrInstruments.o: ../src/visitors/visitor.h
../src/msr/msrInstruments.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrInstruments.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrInstruments.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrInstruments.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrInstruments.o: ../src/interface/libmusicxml.h
../src/msr/msrInstruments.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrInstruments.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrInstruments.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/msr/msrInstruments.o: ../src/msr/msrOah.h
../src/msr/msrKeys.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrKeys.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/msr/msrKeys.o: ../src/utilities/messagesHandling.h
../src/msr/msrKeys.o: ../src/utilities/exceptions.h ../src/msr/msrKeys.h
../src/msr/msrKeys.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrKeys.o: ../src/msr/msrMeasureElements.h
../src/msr/msrKeys.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrKeys.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrKeys.o: ../src/elements/elements.h
../src/msr/msrKeys.o: ../src/elements/xml/xmlElements.h
../src/msr/msrKeys.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrKeys.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrKeys.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrKeys.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrKeys.o: ../src/msr/msrBasicTypes.h
../src/msr/msrKeys.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrKeys.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrKeys.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msrKeys.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/msr/msrKeys.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrLigatures.o: ../src/utilities/utilities.h
../src/msr/msrLigatures.o: ../src/lib/smartpointer.h
../src/msr/msrLigatures.o: ../src/elements/exports.h
../src/msr/msrLigatures.o: ../src/visitors/basevisitor.h
../src/msr/msrLigatures.o: ../src/utilities/messagesHandling.h
../src/msr/msrLigatures.o: ../src/utilities/exceptions.h
../src/msr/msrLigatures.o: ../src/msr/msrLigatures.h ../src/msr/msrElements.h
../src/msr/msrLigatures.o: ../src/elements/typedefs.h
../src/msr/msrLigatures.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrLigatures.o: ../src/elements/elements.h
../src/msr/msrLigatures.o: ../src/elements/xml/xmlElements.h
../src/msr/msrLigatures.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrLigatures.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrLigatures.o: ../src/visitors/visitor.h
../src/msr/msrLigatures.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrLigatures.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrLigatures.o: ../src/oah/oahOah.h
../src/msr/msrLigatures.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrLigatures.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrLigatures.o: ../src/interface/libmusicxml.h
../src/msr/msrLigatures.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrMeasureElements.o: ../src/utilities/utilities.h
../src/msr/msrMeasureElements.o: ../src/lib/smartpointer.h
../src/msr/msrMeasureElements.o: ../src/elements/exports.h
../src/msr/msrMeasureElements.o: ../src/visitors/basevisitor.h
../src/msr/msrMeasureElements.o: ../src/utilities/messagesHandling.h
../src/msr/msrMeasureElements.o: ../src/utilities/exceptions.h
../src/msr/msrMeasureElements.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrMeasureElements.o: ../src/msr/msrMeasureElements.h
../src/msr/msrMeasureElements.o: ../src/msr/msrElements.h
../src/msr/msrMeasureElements.o: ../src/elements/typedefs.h
../src/msr/msrMeasureElements.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrMeasureElements.o: ../src/elements/elements.h
../src/msr/msrMeasureElements.o: ../src/elements/xml/xmlElements.h
../src/msr/msrMeasureElements.o: ../src/elements/types.h
../src/msr/msrMeasureElements.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrMeasureElements.o: ../src/visitors/visitable.h
../src/msr/msrMeasureElements.o: ../src/visitors/visitor.h
../src/msr/msrMeasureElements.o: ../src/lib/tree_browser.h
../src/msr/msrMeasureElements.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrMeasureElements.o: ../src/msr/msrBasicTypes.h
../src/msr/msrMeasureElements.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrMeasureElements.o: ../src/oah/traceOah.h
../src/msr/msrMeasureElements.o: ../src/oah/oahBasicTypes.h
../src/msr/msrMeasureElements.o: ../src/interface/libmusicxml.h
../src/msr/msrMeasureElements.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrMeasureElements.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrMeasureElements.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrMeasures.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrMeasures.o: ../src/elements/typedefs.h
../src/msr/msrMeasures.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrMeasures.o: ../src/elements/elements.h
../src/msr/msrMeasures.o: ../src/elements/xml/xmlElements.h
../src/msr/msrMeasures.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrMeasures.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrMeasures.o: ../src/lib/smartpointer.h
../src/msr/msrMeasures.o: ../src/visitors/visitable.h
../src/msr/msrMeasures.o: ../src/visitors/basevisitor.h
../src/msr/msrMeasures.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrMeasures.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrMeasures.o: ../src/utilities/messagesHandling.h
../src/msr/msrMeasures.o: ../src/utilities/exceptions.h
../src/msr/msrMeasures.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrMeasures.o: ../src/msr/msrTupletElements.h
../src/msr/msrMeasures.o: ../src/msr/msrMeasureElements.h
../src/msr/msrMeasures.o: ../src/msr/msrBasicTypes.h
../src/msr/msrMeasures.o: ../src/msr/msrVoiceElements.h
../src/msr/msrMeasures.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrMeasures.o: ../src/msr/msrIdentification.h
../src/msr/msrMeasures.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrMeasures.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrMeasures.o: ../src/msr/msrDivisions.h
../src/msr/msrMeasures.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrMeasures.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrMeasures.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrMeasures.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrMeasures.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrMeasures.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrMeasures.o: ../src/msr/msrTranspositions.h
../src/msr/msrMeasures.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrMeasures.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrMeasures.o: ../src/msr/msrDynamics.h
../src/msr/msrMeasures.o: ../src/msr/msrArticulations.h
../src/msr/msrMeasures.o: ../src/msr/msrTablatures.h
../src/msr/msrMeasures.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrMeasures.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrMeasures.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrMeasures.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrMeasures.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrMeasures.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrMeasures.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrMeasures.o: ../src/interface/libmusicxml.h
../src/msr/msrMeasures.o: ../src/oah/oahElements.h
../src/msr/msrMeasures.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrMeasures.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrMeasures.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrMeasures.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/typedefs.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/elements.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/xml/xmlElements.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/types.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/exports.h
../src/msr/msrMeasuresRepeats.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrMeasuresRepeats.o: ../src/lib/smartpointer.h
../src/msr/msrMeasuresRepeats.o: ../src/visitors/visitable.h
../src/msr/msrMeasuresRepeats.o: ../src/visitors/basevisitor.h
../src/msr/msrMeasuresRepeats.o: ../src/visitors/visitor.h
../src/msr/msrMeasuresRepeats.o: ../src/lib/tree_browser.h
../src/msr/msrMeasuresRepeats.o: ../src/lib/browser.h
../src/msr/msrMeasuresRepeats.o: ../src/utilities/utilities.h
../src/msr/msrMeasuresRepeats.o: ../src/utilities/messagesHandling.h
../src/msr/msrMeasuresRepeats.o: ../src/utilities/exceptions.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrElements.h
../src/msr/msrMeasuresRepeats.o: ../src/lib/rational.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTupletElements.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrMeasureElements.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrBasicTypes.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrVoiceElements.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrIdentification.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrScaling.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrCredits.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrDivisions.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrStavesDetails.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrRehearsals.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTranspositions.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTechnicals.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrBars.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrDynamics.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrArticulations.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTablatures.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrInstruments.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrLigatures.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSlashes.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrPartGroups.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/generalOah.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/oahBasicTypes.h
../src/msr/msrMeasuresRepeats.o: ../src/interface/libmusicxml.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/oahElements.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrMeasuresRepeats.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrMeasuresRepeats.o: ../src/msr/msrOah.h
../src/msr/msrMidi.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrMidi.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/msr/msrMidi.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/msr/msrMidi.o: ../src/elements/typedefs.h
../src/msr/msrMidi.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrMidi.o: ../src/elements/elements.h
../src/msr/msrMidi.o: ../src/elements/xml/xmlElements.h
../src/msr/msrMidi.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrMidi.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrMidi.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrMidi.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrMidi.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrMidi.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrMidi.o: ../src/oah/oahBasicTypes.h
../src/msr/msrMidi.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msrMidi.o: ../src/msr/msrOah.h
../src/msr/msrMusicXMLSpecifics.o: ../src/utilities/utilities.h
../src/msr/msrMusicXMLSpecifics.o: ../src/lib/smartpointer.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/exports.h
../src/msr/msrMusicXMLSpecifics.o: ../src/visitors/basevisitor.h
../src/msr/msrMusicXMLSpecifics.o: ../src/utilities/messagesHandling.h
../src/msr/msrMusicXMLSpecifics.o: ../src/utilities/exceptions.h
../src/msr/msrMusicXMLSpecifics.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrMusicXMLSpecifics.o: ../src/msr/msrElements.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/typedefs.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/elements.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/xml/xmlElements.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/types.h
../src/msr/msrMusicXMLSpecifics.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrMusicXMLSpecifics.o: ../src/visitors/visitable.h
../src/msr/msrMusicXMLSpecifics.o: ../src/visitors/visitor.h
../src/msr/msrMusicXMLSpecifics.o: ../src/lib/tree_browser.h
../src/msr/msrMusicXMLSpecifics.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrMusicXMLSpecifics.o: ../src/msr/msrMeasureElements.h
../src/msr/msrMusicXMLSpecifics.o: ../src/msr/msrBasicTypes.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/traceOah.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/oahBasicTypes.h
../src/msr/msrMusicXMLSpecifics.o: ../src/interface/libmusicxml.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/oahElements.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/oahOah.h
../src/msr/msrMusicXMLSpecifics.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrMusicXMLSpecifics.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrNotes.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrNotes.o: ../src/elements/typedefs.h
../src/msr/msrNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrNotes.o: ../src/elements/elements.h
../src/msr/msrNotes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrNotes.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrNotes.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrNotes.o: ../src/visitors/basevisitor.h
../src/msr/msrNotes.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrNotes.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrNotes.o: ../src/utilities/messagesHandling.h
../src/msr/msrNotes.o: ../src/utilities/exceptions.h ../src/msr/msrElements.h
../src/msr/msrNotes.o: ../src/lib/rational.h ../src/msr/msrTupletElements.h
../src/msr/msrNotes.o: ../src/msr/msrMeasureElements.h
../src/msr/msrNotes.o: ../src/msr/msrBasicTypes.h
../src/msr/msrNotes.o: ../src/msr/msrVoiceElements.h
../src/msr/msrNotes.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrNotes.o: ../src/msr/msrIdentification.h
../src/msr/msrNotes.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrNotes.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrNotes.o: ../src/msr/msrDivisions.h
../src/msr/msrNotes.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrNotes.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrNotes.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrNotes.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrNotes.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrNotes.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrNotes.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrNotes.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrNotes.o: ../src/msr/msrArticulations.h
../src/msr/msrNotes.o: ../src/msr/msrTablatures.h ../src/msr/msrInstruments.h
../src/msr/msrNotes.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrNotes.o: ../src/msr/msrLigatures.h
../src/msr/msrNotes.o: ../src/msr/msrSingleTremolos.h ../src/msr/msrSlashes.h
../src/msr/msrNotes.o: ../src/msr/msrPartGroups.h
../src/msr/msrNotes.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrNotes.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrNotes.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrNotes.o: ../src/interface/libmusicxml.h
../src/msr/msrNotes.o: ../src/oah/oahElements.h
../src/msr/msrNotes.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrNotes.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrNotes.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrNotes.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrNotes.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrOah.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrOah.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/msr/msrOah.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrOah.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msrOah.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/msr/msrOah.o: ../src/visitors/visitable.h ../src/oah/oahOah.h
../src/msr/msrOah.o: ../src/oah/oahAtomsCollection.h ../src/lib/rational.h
../src/msr/msrOah.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/msr/msrOah.o: ../src/elements/typedefs.h
../src/msr/msrOah.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrOah.o: ../src/elements/elements.h
../src/msr/msrOah.o: ../src/elements/xml/xmlElements.h
../src/msr/msrOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrOah.o: ../src/msr/msrBasicTypes.h ../src/oah/generalOah.h
../src/msr/msrOah.o: ../src/msr/msrOah.h ../src/utilities/messagesHandling.h
../src/msr/msrOah.o: ../src/utilities/exceptions.h
../src/msr/msrOrnaments.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrOrnaments.o: ../src/elements/typedefs.h
../src/msr/msrOrnaments.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrOrnaments.o: ../src/elements/elements.h
../src/msr/msrOrnaments.o: ../src/elements/xml/xmlElements.h
../src/msr/msrOrnaments.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrOrnaments.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrOrnaments.o: ../src/lib/smartpointer.h
../src/msr/msrOrnaments.o: ../src/visitors/visitable.h
../src/msr/msrOrnaments.o: ../src/visitors/basevisitor.h
../src/msr/msrOrnaments.o: ../src/visitors/visitor.h
../src/msr/msrOrnaments.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrOrnaments.o: ../src/utilities/utilities.h
../src/msr/msrOrnaments.o: ../src/utilities/messagesHandling.h
../src/msr/msrOrnaments.o: ../src/utilities/exceptions.h
../src/msr/msrOrnaments.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrOrnaments.o: ../src/msr/msrTupletElements.h
../src/msr/msrOrnaments.o: ../src/msr/msrMeasureElements.h
../src/msr/msrOrnaments.o: ../src/msr/msrBasicTypes.h
../src/msr/msrOrnaments.o: ../src/msr/msrVoiceElements.h
../src/msr/msrOrnaments.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrOrnaments.o: ../src/msr/msrIdentification.h
../src/msr/msrOrnaments.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrOrnaments.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrOrnaments.o: ../src/msr/msrDivisions.h
../src/msr/msrOrnaments.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrOrnaments.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrOrnaments.o: ../src/msr/msrStavesDetails.h
../src/msr/msrOrnaments.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrOrnaments.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrOrnaments.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrOrnaments.o: ../src/msr/msrRehearsals.h
../src/msr/msrOrnaments.o: ../src/msr/msrTranspositions.h
../src/msr/msrOrnaments.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrOrnaments.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrOrnaments.o: ../src/msr/msrDynamics.h
../src/msr/msrOrnaments.o: ../src/msr/msrArticulations.h
../src/msr/msrOrnaments.o: ../src/msr/msrTablatures.h
../src/msr/msrOrnaments.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrOrnaments.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrOrnaments.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrOrnaments.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrOrnaments.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrOrnaments.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrOrnaments.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrOrnaments.o: ../src/interface/libmusicxml.h
../src/msr/msrOrnaments.o: ../src/oah/oahElements.h
../src/msr/msrOrnaments.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrOrnaments.o: ../src/msr/msrParts_MUT_DEP.h ../src/oah/oahOah.h
../src/msr/msrOrnaments.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrOrnaments.o: ../src/msr/msrOah.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrMeasureElements.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrElements.h
../src/msr/msrPartGroupElements.o: ../src/elements/typedefs.h
../src/msr/msrPartGroupElements.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrPartGroupElements.o: ../src/elements/elements.h
../src/msr/msrPartGroupElements.o: ../src/elements/xml/xmlElements.h
../src/msr/msrPartGroupElements.o: ../src/elements/types.h
../src/msr/msrPartGroupElements.o: ../src/elements/exports.h
../src/msr/msrPartGroupElements.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrPartGroupElements.o: ../src/lib/smartpointer.h
../src/msr/msrPartGroupElements.o: ../src/visitors/visitable.h
../src/msr/msrPartGroupElements.o: ../src/visitors/basevisitor.h
../src/msr/msrPartGroupElements.o: ../src/visitors/visitor.h
../src/msr/msrPartGroupElements.o: ../src/lib/tree_browser.h
../src/msr/msrPartGroupElements.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrBasicTypes.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrOah.h
../src/msr/msrPartGroupElements.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrPartGroupElements.o: ../src/msr/msrMidi.h
../src/msr/msrPartGroupElements.o: ../src/oah/oahBasicTypes.h
../src/msr/msrPartGroupElements.o: ../src/interface/libmusicxml.h
../src/msr/msrPartGroupElements.o: ../src/utilities/utilities.h
../src/msr/msrPartGroupElements.o: ../src/oah/oahElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrPartGroups.h
../src/msr/msrPartGroups.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrPartGroups.o: ../src/msr/msrMeasureElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrElements.h
../src/msr/msrPartGroups.o: ../src/elements/typedefs.h
../src/msr/msrPartGroups.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrPartGroups.o: ../src/elements/elements.h
../src/msr/msrPartGroups.o: ../src/elements/xml/xmlElements.h
../src/msr/msrPartGroups.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrPartGroups.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrPartGroups.o: ../src/lib/smartpointer.h
../src/msr/msrPartGroups.o: ../src/visitors/visitable.h
../src/msr/msrPartGroups.o: ../src/visitors/basevisitor.h
../src/msr/msrPartGroups.o: ../src/visitors/visitor.h
../src/msr/msrPartGroups.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrPartGroups.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrPartGroups.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrPartGroups.o: ../src/utilities/utilities.h
../src/msr/msrPartGroups.o: ../src/utilities/messagesHandling.h
../src/msr/msrPartGroups.o: ../src/utilities/exceptions.h
../src/msr/msrPartGroups.o: ../src/msr/msrTupletElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrVoiceElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrPartGroups.o: ../src/msr/msrIdentification.h
../src/msr/msrPartGroups.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrPartGroups.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrPartGroups.o: ../src/msr/msrDivisions.h
../src/msr/msrPartGroups.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrPartGroups.o: ../src/msr/msrStavesDetails.h
../src/msr/msrPartGroups.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrPartGroups.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrPartGroups.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrPartGroups.o: ../src/msr/msrRehearsals.h
../src/msr/msrPartGroups.o: ../src/msr/msrTranspositions.h
../src/msr/msrPartGroups.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrPartGroups.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrPartGroups.o: ../src/msr/msrDynamics.h
../src/msr/msrPartGroups.o: ../src/msr/msrArticulations.h
../src/msr/msrPartGroups.o: ../src/msr/msrTablatures.h
../src/msr/msrPartGroups.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrPartGroups.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrPartGroups.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrPartGroups.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/msr/msrPartGroups.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/msr/msrPartGroups.o: ../src/oah/oahBasicTypes.h
../src/msr/msrPartGroups.o: ../src/interface/libmusicxml.h
../src/msr/msrPartGroups.o: ../src/oah/oahElements.h
../src/msr/msrPartGroups.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrPartGroups.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrPartGroups.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrPartGroups.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrPartGroups.o: ../src/msr/msrOah.h
../src/msr/msrPartNamesDisplays.o: ../src/utilities/utilities.h
../src/msr/msrPartNamesDisplays.o: ../src/lib/smartpointer.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/exports.h
../src/msr/msrPartNamesDisplays.o: ../src/visitors/basevisitor.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrMeasureElements.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrElements.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/typedefs.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/elements.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/xml/xmlElements.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/types.h
../src/msr/msrPartNamesDisplays.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrPartNamesDisplays.o: ../src/visitors/visitable.h
../src/msr/msrPartNamesDisplays.o: ../src/visitors/visitor.h
../src/msr/msrPartNamesDisplays.o: ../src/lib/tree_browser.h
../src/msr/msrPartNamesDisplays.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrBasicTypes.h
../src/msr/msrPartNamesDisplays.o: ../src/oah/oahOah.h
../src/msr/msrPartNamesDisplays.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrMidi.h
../src/msr/msrPartNamesDisplays.o: ../src/oah/oahBasicTypes.h
../src/msr/msrPartNamesDisplays.o: ../src/interface/libmusicxml.h
../src/msr/msrPartNamesDisplays.o: ../src/oah/oahElements.h
../src/msr/msrPartNamesDisplays.o: ../src/msr/msrOah.h
../src/msr/msrParts.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrParts.o: ../src/elements/typedefs.h
../src/msr/msrParts.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrParts.o: ../src/elements/elements.h
../src/msr/msrParts.o: ../src/elements/xml/xmlElements.h
../src/msr/msrParts.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrParts.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrParts.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrParts.o: ../src/visitors/basevisitor.h
../src/msr/msrParts.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrParts.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrParts.o: ../src/utilities/messagesHandling.h
../src/msr/msrParts.o: ../src/utilities/exceptions.h ../src/msr/msrElements.h
../src/msr/msrParts.o: ../src/lib/rational.h ../src/msr/msrTupletElements.h
../src/msr/msrParts.o: ../src/msr/msrMeasureElements.h
../src/msr/msrParts.o: ../src/msr/msrBasicTypes.h
../src/msr/msrParts.o: ../src/msr/msrVoiceElements.h
../src/msr/msrParts.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrParts.o: ../src/msr/msrIdentification.h
../src/msr/msrParts.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrParts.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrParts.o: ../src/msr/msrDivisions.h
../src/msr/msrParts.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrParts.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrParts.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrParts.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrParts.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrParts.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrParts.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrParts.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrParts.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrParts.o: ../src/msr/msrArticulations.h
../src/msr/msrParts.o: ../src/msr/msrTablatures.h ../src/msr/msrInstruments.h
../src/msr/msrParts.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrParts.o: ../src/msr/msrLigatures.h
../src/msr/msrParts.o: ../src/msr/msrSingleTremolos.h ../src/msr/msrSlashes.h
../src/msr/msrParts.o: ../src/msr/msrPartGroups.h
../src/msr/msrParts.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrParts.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrParts.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrParts.o: ../src/interface/libmusicxml.h
../src/msr/msrParts.o: ../src/oah/oahElements.h
../src/msr/msrParts.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrParts.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrParts.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrParts.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrParts.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrRehearsals.o: ../src/utilities/utilities.h
../src/msr/msrRehearsals.o: ../src/lib/smartpointer.h
../src/msr/msrRehearsals.o: ../src/elements/exports.h
../src/msr/msrRehearsals.o: ../src/visitors/basevisitor.h
../src/msr/msrRehearsals.o: ../src/msr/msrRehearsals.h
../src/msr/msrRehearsals.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrRehearsals.o: ../src/msr/msrMeasureElements.h
../src/msr/msrRehearsals.o: ../src/msr/msrElements.h
../src/msr/msrRehearsals.o: ../src/elements/typedefs.h
../src/msr/msrRehearsals.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrRehearsals.o: ../src/elements/elements.h
../src/msr/msrRehearsals.o: ../src/elements/xml/xmlElements.h
../src/msr/msrRehearsals.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrRehearsals.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrRehearsals.o: ../src/visitors/visitor.h
../src/msr/msrRehearsals.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrRehearsals.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrRehearsals.o: ../src/oah/oahOah.h
../src/msr/msrRehearsals.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrRehearsals.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrRehearsals.o: ../src/interface/libmusicxml.h
../src/msr/msrRehearsals.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrRepeatCodas.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrRepeatCodas.o: ../src/elements/typedefs.h
../src/msr/msrRepeatCodas.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrRepeatCodas.o: ../src/elements/elements.h
../src/msr/msrRepeatCodas.o: ../src/elements/xml/xmlElements.h
../src/msr/msrRepeatCodas.o: ../src/elements/types.h
../src/msr/msrRepeatCodas.o: ../src/elements/exports.h ../src/elements/xml.h
../src/msr/msrRepeatCodas.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msrRepeatCodas.o: ../src/visitors/visitable.h
../src/msr/msrRepeatCodas.o: ../src/visitors/basevisitor.h
../src/msr/msrRepeatCodas.o: ../src/visitors/visitor.h
../src/msr/msrRepeatCodas.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrRepeatCodas.o: ../src/utilities/utilities.h
../src/msr/msrRepeatCodas.o: ../src/utilities/messagesHandling.h
../src/msr/msrRepeatCodas.o: ../src/utilities/exceptions.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTupletElements.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrMeasureElements.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrBasicTypes.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrVoiceElements.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrIdentification.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrDivisions.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrStavesDetails.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrRehearsals.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTranspositions.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrDynamics.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrArticulations.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTablatures.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSlashes.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrPartGroups.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrRepeatCodas.o: ../src/oah/generalOah.h
../src/msr/msrRepeatCodas.o: ../src/oah/oahBasicTypes.h
../src/msr/msrRepeatCodas.o: ../src/interface/libmusicxml.h
../src/msr/msrRepeatCodas.o: ../src/oah/oahElements.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrRepeatCodas.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrRepeatCodas.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrRepeatCodas.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrRepeatCodas.o: ../src/msr/msrOah.h
../src/msr/msrRepeats.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrRepeats.o: ../src/elements/typedefs.h
../src/msr/msrRepeats.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrRepeats.o: ../src/elements/elements.h
../src/msr/msrRepeats.o: ../src/elements/xml/xmlElements.h
../src/msr/msrRepeats.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrRepeats.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrRepeats.o: ../src/lib/smartpointer.h
../src/msr/msrRepeats.o: ../src/visitors/visitable.h
../src/msr/msrRepeats.o: ../src/visitors/basevisitor.h
../src/msr/msrRepeats.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrRepeats.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrRepeats.o: ../src/utilities/messagesHandling.h
../src/msr/msrRepeats.o: ../src/utilities/exceptions.h
../src/msr/msrRepeats.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrRepeats.o: ../src/msr/msrTupletElements.h
../src/msr/msrRepeats.o: ../src/msr/msrMeasureElements.h
../src/msr/msrRepeats.o: ../src/msr/msrBasicTypes.h
../src/msr/msrRepeats.o: ../src/msr/msrVoiceElements.h
../src/msr/msrRepeats.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrRepeats.o: ../src/msr/msrIdentification.h
../src/msr/msrRepeats.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrRepeats.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrRepeats.o: ../src/msr/msrDivisions.h
../src/msr/msrRepeats.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrRepeats.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrRepeats.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrRepeats.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrRepeats.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrRepeats.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrRepeats.o: ../src/msr/msrTranspositions.h
../src/msr/msrRepeats.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrRepeats.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrRepeats.o: ../src/msr/msrDynamics.h
../src/msr/msrRepeats.o: ../src/msr/msrArticulations.h
../src/msr/msrRepeats.o: ../src/msr/msrTablatures.h
../src/msr/msrRepeats.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrRepeats.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrRepeats.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrRepeats.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrRepeats.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrRepeats.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrRepeats.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrRepeats.o: ../src/interface/libmusicxml.h
../src/msr/msrRepeats.o: ../src/oah/oahElements.h
../src/msr/msrRepeats.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrRepeats.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrRepeats.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrRepeats.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrRestMeasures.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrRestMeasures.o: ../src/elements/typedefs.h
../src/msr/msrRestMeasures.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrRestMeasures.o: ../src/elements/elements.h
../src/msr/msrRestMeasures.o: ../src/elements/xml/xmlElements.h
../src/msr/msrRestMeasures.o: ../src/elements/types.h
../src/msr/msrRestMeasures.o: ../src/elements/exports.h ../src/elements/xml.h
../src/msr/msrRestMeasures.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/msr/msrRestMeasures.o: ../src/visitors/visitable.h
../src/msr/msrRestMeasures.o: ../src/visitors/basevisitor.h
../src/msr/msrRestMeasures.o: ../src/visitors/visitor.h
../src/msr/msrRestMeasures.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrRestMeasures.o: ../src/utilities/utilities.h
../src/msr/msrRestMeasures.o: ../src/utilities/messagesHandling.h
../src/msr/msrRestMeasures.o: ../src/utilities/exceptions.h
../src/msr/msrRestMeasures.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTupletElements.h
../src/msr/msrRestMeasures.o: ../src/msr/msrMeasureElements.h
../src/msr/msrRestMeasures.o: ../src/msr/msrBasicTypes.h
../src/msr/msrRestMeasures.o: ../src/msr/msrVoiceElements.h
../src/msr/msrRestMeasures.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrRestMeasures.o: ../src/msr/msrIdentification.h
../src/msr/msrRestMeasures.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrRestMeasures.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrRestMeasures.o: ../src/msr/msrDivisions.h
../src/msr/msrRestMeasures.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrRestMeasures.o: ../src/msr/msrStavesDetails.h
../src/msr/msrRestMeasures.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrRestMeasures.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrRestMeasures.o: ../src/msr/msrRehearsals.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTranspositions.h
../src/msr/msrRestMeasures.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrRestMeasures.o: ../src/msr/msrDynamics.h
../src/msr/msrRestMeasures.o: ../src/msr/msrArticulations.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTablatures.h
../src/msr/msrRestMeasures.o: ../src/msr/msrInstruments.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/msr/msrRestMeasures.o: ../src/msr/msrLigatures.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSlashes.h
../src/msr/msrRestMeasures.o: ../src/msr/msrPartGroups.h
../src/msr/msrRestMeasures.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrRestMeasures.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrRestMeasures.o: ../src/oah/generalOah.h
../src/msr/msrRestMeasures.o: ../src/oah/oahBasicTypes.h
../src/msr/msrRestMeasures.o: ../src/interface/libmusicxml.h
../src/msr/msrRestMeasures.o: ../src/oah/oahElements.h
../src/msr/msrRestMeasures.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrRestMeasures.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrRestMeasures.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrRestMeasures.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrRestMeasures.o: ../src/msr/msrOah.h
../src/msr/msrScaling.o: ../src/utilities/utilities.h
../src/msr/msrScaling.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrScaling.o: ../src/visitors/basevisitor.h
../src/msr/msrScaling.o: ../src/utilities/messagesHandling.h
../src/msr/msrScaling.o: ../src/utilities/exceptions.h
../src/msr/msrScaling.o: ../src/msr/msrScaling.h ../src/msr/msrElements.h
../src/msr/msrScaling.o: ../src/elements/typedefs.h
../src/msr/msrScaling.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrScaling.o: ../src/elements/elements.h
../src/msr/msrScaling.o: ../src/elements/xml/xmlElements.h
../src/msr/msrScaling.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrScaling.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrScaling.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrScaling.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrScaling.o: ../src/msr/msrBasicTypes.h
../src/msr/msrScaling.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrScaling.o: ../src/msr/msrMeasureElements.h
../src/msr/msrScaling.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrScaling.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrScaling.o: ../src/interface/libmusicxml.h
../src/msr/msrScaling.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrScaling.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrScaling.o: ../src/oah/generalOah.h ../src/msr/msrOah.h
../src/msr/msrScaling.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/msr/msrScores.o: ../src/msr/msrScores.h ../src/msr/msrIdentification.h
../src/msr/msrScores.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrScores.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrScores.o: ../src/elements/elements.h
../src/msr/msrScores.o: ../src/elements/xml/xmlElements.h
../src/msr/msrScores.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrScores.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrScores.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrScores.o: ../src/visitors/basevisitor.h
../src/msr/msrScores.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrScores.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrScores.o: ../src/msr/msrBasicTypes.h
../src/msr/msrScores.o: ../src/msr/msrVarValAssocs.h ../src/msr/msrScaling.h
../src/msr/msrScores.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrScores.o: ../src/msr/msrMeasureElements.h
../src/msr/msrScores.o: ../src/msr/msrCredits.h ../src/msr/msr.h
../src/msr/msrScores.o: ../src/utilities/utilities.h
../src/msr/msrScores.o: ../src/utilities/messagesHandling.h
../src/msr/msrScores.o: ../src/utilities/exceptions.h
../src/msr/msrScores.o: ../src/msr/msrDivisions.h
../src/msr/msrScores.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrScores.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrScores.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrScores.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrScores.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrScores.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrScores.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrScores.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrScores.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrScores.o: ../src/msr/msrArticulations.h
../src/msr/msrScores.o: ../src/msr/msrTablatures.h
../src/msr/msrScores.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrScores.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrScores.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrScores.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrScores.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrScores.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrScores.o: ../src/msr/msrTupletElements.h
../src/msr/msrScores.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/msr/msrScores.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrScores.o: ../src/interface/libmusicxml.h
../src/msr/msrScores.o: ../src/oah/oahElements.h
../src/msr/msrScores.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrScores.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrScores.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrScores.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrScores.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrScores.o: ../src/oah/mxmlTree2msrOah.h ../src/msr/msrOah.h
../src/msr/msrSegments.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrSegments.o: ../src/elements/typedefs.h
../src/msr/msrSegments.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSegments.o: ../src/elements/elements.h
../src/msr/msrSegments.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSegments.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrSegments.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrSegments.o: ../src/lib/smartpointer.h
../src/msr/msrSegments.o: ../src/visitors/visitable.h
../src/msr/msrSegments.o: ../src/visitors/basevisitor.h
../src/msr/msrSegments.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrSegments.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrSegments.o: ../src/utilities/messagesHandling.h
../src/msr/msrSegments.o: ../src/utilities/exceptions.h
../src/msr/msrSegments.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrSegments.o: ../src/msr/msrTupletElements.h
../src/msr/msrSegments.o: ../src/msr/msrMeasureElements.h
../src/msr/msrSegments.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSegments.o: ../src/msr/msrVoiceElements.h
../src/msr/msrSegments.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrSegments.o: ../src/msr/msrIdentification.h
../src/msr/msrSegments.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrSegments.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrSegments.o: ../src/msr/msrDivisions.h
../src/msr/msrSegments.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrSegments.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrSegments.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrSegments.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrSegments.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrSegments.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrSegments.o: ../src/msr/msrTranspositions.h
../src/msr/msrSegments.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrSegments.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrSegments.o: ../src/msr/msrDynamics.h
../src/msr/msrSegments.o: ../src/msr/msrArticulations.h
../src/msr/msrSegments.o: ../src/msr/msrTablatures.h
../src/msr/msrSegments.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrSegments.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrSegments.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrSegments.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrSegments.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrSegments.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrSegments.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrSegments.o: ../src/interface/libmusicxml.h
../src/msr/msrSegments.o: ../src/oah/oahElements.h
../src/msr/msrSegments.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrSegments.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrSegments.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrSegments.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrSegments.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrSegnosAndCodas.o: ../src/utilities/utilities.h
../src/msr/msrSegnosAndCodas.o: ../src/lib/smartpointer.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/exports.h
../src/msr/msrSegnosAndCodas.o: ../src/visitors/basevisitor.h
../src/msr/msrSegnosAndCodas.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrSegnosAndCodas.o: ../src/msr/msrMeasureElements.h
../src/msr/msrSegnosAndCodas.o: ../src/msr/msrElements.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/typedefs.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/elements.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSegnosAndCodas.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrSegnosAndCodas.o: ../src/lib/ctree.h
../src/msr/msrSegnosAndCodas.o: ../src/visitors/visitable.h
../src/msr/msrSegnosAndCodas.o: ../src/visitors/visitor.h
../src/msr/msrSegnosAndCodas.o: ../src/lib/tree_browser.h
../src/msr/msrSegnosAndCodas.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrSegnosAndCodas.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSegnosAndCodas.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrSegnosAndCodas.o: ../src/oah/traceOah.h
../src/msr/msrSegnosAndCodas.o: ../src/oah/oahBasicTypes.h
../src/msr/msrSegnosAndCodas.o: ../src/interface/libmusicxml.h
../src/msr/msrSegnosAndCodas.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrSegnosAndCodas.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrSegnosAndCodas.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrSingleTremolos.o: ../src/utilities/utilities.h
../src/msr/msrSingleTremolos.o: ../src/lib/smartpointer.h
../src/msr/msrSingleTremolos.o: ../src/elements/exports.h
../src/msr/msrSingleTremolos.o: ../src/visitors/basevisitor.h
../src/msr/msrSingleTremolos.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrSingleTremolos.o: ../src/msr/msrElements.h
../src/msr/msrSingleTremolos.o: ../src/elements/typedefs.h
../src/msr/msrSingleTremolos.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSingleTremolos.o: ../src/elements/elements.h
../src/msr/msrSingleTremolos.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSingleTremolos.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrSingleTremolos.o: ../src/lib/ctree.h
../src/msr/msrSingleTremolos.o: ../src/visitors/visitable.h
../src/msr/msrSingleTremolos.o: ../src/visitors/visitor.h
../src/msr/msrSingleTremolos.o: ../src/lib/tree_browser.h
../src/msr/msrSingleTremolos.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrSingleTremolos.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSingleTremolos.o: ../src/oah/oahOah.h
../src/msr/msrSingleTremolos.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrSingleTremolos.o: ../src/msr/msrMidi.h
../src/msr/msrSingleTremolos.o: ../src/oah/oahBasicTypes.h
../src/msr/msrSingleTremolos.o: ../src/interface/libmusicxml.h
../src/msr/msrSingleTremolos.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrSlashes.o: ../src/utilities/utilities.h
../src/msr/msrSlashes.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrSlashes.o: ../src/visitors/basevisitor.h
../src/msr/msrSlashes.o: ../src/msr/msrSlashes.h ../src/msr/msrElements.h
../src/msr/msrSlashes.o: ../src/elements/typedefs.h
../src/msr/msrSlashes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSlashes.o: ../src/elements/elements.h
../src/msr/msrSlashes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSlashes.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrSlashes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrSlashes.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrSlashes.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrSlashes.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrSlashes.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrSlashes.o: ../src/oah/oahBasicTypes.h
../src/msr/msrSlashes.o: ../src/interface/libmusicxml.h
../src/msr/msrSlashes.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrSlides.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrSlides.o: ../src/elements/typedefs.h
../src/msr/msrSlides.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSlides.o: ../src/elements/elements.h
../src/msr/msrSlides.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSlides.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrSlides.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrSlides.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrSlides.o: ../src/visitors/basevisitor.h
../src/msr/msrSlides.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrSlides.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrSlides.o: ../src/utilities/messagesHandling.h
../src/msr/msrSlides.o: ../src/utilities/exceptions.h
../src/msr/msrSlides.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrSlides.o: ../src/msr/msrTupletElements.h
../src/msr/msrSlides.o: ../src/msr/msrMeasureElements.h
../src/msr/msrSlides.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSlides.o: ../src/msr/msrVoiceElements.h
../src/msr/msrSlides.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrSlides.o: ../src/msr/msrIdentification.h
../src/msr/msrSlides.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrSlides.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrSlides.o: ../src/msr/msrDivisions.h
../src/msr/msrSlides.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrSlides.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrSlides.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrSlides.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrSlides.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrSlides.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrSlides.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrSlides.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrSlides.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrSlides.o: ../src/msr/msrArticulations.h
../src/msr/msrSlides.o: ../src/msr/msrTablatures.h
../src/msr/msrSlides.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrSlides.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrSlides.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrSlides.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrSlides.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrSlides.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrSlides.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrSlides.o: ../src/interface/libmusicxml.h
../src/msr/msrSlides.o: ../src/oah/oahElements.h
../src/msr/msrSlides.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrSlides.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrSlides.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrSlides.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrSlides.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrSlurs.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrSlurs.o: ../src/elements/exports.h
../src/msr/msrSlurs.o: ../src/visitors/basevisitor.h ../src/msr/msrSlurs.h
../src/msr/msrSlurs.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrSlurs.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSlurs.o: ../src/elements/elements.h
../src/msr/msrSlurs.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSlurs.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrSlurs.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrSlurs.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrSlurs.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrSlurs.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrSlurs.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrSlurs.o: ../src/oah/oahBasicTypes.h
../src/msr/msrSlurs.o: ../src/interface/libmusicxml.h
../src/msr/msrSlurs.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrSlurs.o: ../src/utilities/messagesHandling.h
../src/msr/msrSlurs.o: ../src/utilities/exceptions.h
../src/msr/msrSpanners.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrSpanners.o: ../src/elements/typedefs.h
../src/msr/msrSpanners.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSpanners.o: ../src/elements/elements.h
../src/msr/msrSpanners.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSpanners.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrSpanners.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrSpanners.o: ../src/lib/smartpointer.h
../src/msr/msrSpanners.o: ../src/visitors/visitable.h
../src/msr/msrSpanners.o: ../src/visitors/basevisitor.h
../src/msr/msrSpanners.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrSpanners.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrSpanners.o: ../src/utilities/messagesHandling.h
../src/msr/msrSpanners.o: ../src/utilities/exceptions.h
../src/msr/msrSpanners.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrSpanners.o: ../src/msr/msrTupletElements.h
../src/msr/msrSpanners.o: ../src/msr/msrMeasureElements.h
../src/msr/msrSpanners.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSpanners.o: ../src/msr/msrVoiceElements.h
../src/msr/msrSpanners.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrSpanners.o: ../src/msr/msrIdentification.h
../src/msr/msrSpanners.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrSpanners.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrSpanners.o: ../src/msr/msrDivisions.h
../src/msr/msrSpanners.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrSpanners.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrSpanners.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrSpanners.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrSpanners.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrSpanners.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrSpanners.o: ../src/msr/msrTranspositions.h
../src/msr/msrSpanners.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrSpanners.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrSpanners.o: ../src/msr/msrDynamics.h
../src/msr/msrSpanners.o: ../src/msr/msrArticulations.h
../src/msr/msrSpanners.o: ../src/msr/msrTablatures.h
../src/msr/msrSpanners.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrSpanners.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrSpanners.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrSpanners.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrSpanners.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrSpanners.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrSpanners.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrSpanners.o: ../src/interface/libmusicxml.h
../src/msr/msrSpanners.o: ../src/oah/oahElements.h
../src/msr/msrSpanners.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrSpanners.o: ../src/msr/msrParts_MUT_DEP.h ../src/oah/oahOah.h
../src/msr/msrSpanners.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrStanzas.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrStanzas.o: ../src/elements/typedefs.h
../src/msr/msrStanzas.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrStanzas.o: ../src/elements/elements.h
../src/msr/msrStanzas.o: ../src/elements/xml/xmlElements.h
../src/msr/msrStanzas.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrStanzas.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrStanzas.o: ../src/lib/smartpointer.h
../src/msr/msrStanzas.o: ../src/visitors/visitable.h
../src/msr/msrStanzas.o: ../src/visitors/basevisitor.h
../src/msr/msrStanzas.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrStanzas.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrStanzas.o: ../src/utilities/messagesHandling.h
../src/msr/msrStanzas.o: ../src/utilities/exceptions.h
../src/msr/msrStanzas.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrStanzas.o: ../src/msr/msrTupletElements.h
../src/msr/msrStanzas.o: ../src/msr/msrMeasureElements.h
../src/msr/msrStanzas.o: ../src/msr/msrBasicTypes.h
../src/msr/msrStanzas.o: ../src/msr/msrVoiceElements.h
../src/msr/msrStanzas.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrStanzas.o: ../src/msr/msrIdentification.h
../src/msr/msrStanzas.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrStanzas.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrStanzas.o: ../src/msr/msrDivisions.h
../src/msr/msrStanzas.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrStanzas.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrStanzas.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrStanzas.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrStanzas.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrStanzas.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrStanzas.o: ../src/msr/msrTranspositions.h
../src/msr/msrStanzas.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrStanzas.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrStanzas.o: ../src/msr/msrDynamics.h
../src/msr/msrStanzas.o: ../src/msr/msrArticulations.h
../src/msr/msrStanzas.o: ../src/msr/msrTablatures.h
../src/msr/msrStanzas.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrStanzas.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrStanzas.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrStanzas.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrStanzas.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrStanzas.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrStanzas.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrStanzas.o: ../src/interface/libmusicxml.h
../src/msr/msrStanzas.o: ../src/oah/oahElements.h
../src/msr/msrStanzas.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrStanzas.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrStanzas.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrStanzas.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrStaves.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrStaves.o: ../src/elements/typedefs.h
../src/msr/msrStaves.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrStaves.o: ../src/elements/elements.h
../src/msr/msrStaves.o: ../src/elements/xml/xmlElements.h
../src/msr/msrStaves.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrStaves.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrStaves.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrStaves.o: ../src/visitors/basevisitor.h
../src/msr/msrStaves.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrStaves.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrStaves.o: ../src/utilities/messagesHandling.h
../src/msr/msrStaves.o: ../src/utilities/exceptions.h
../src/msr/msrStaves.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrStaves.o: ../src/msr/msrTupletElements.h
../src/msr/msrStaves.o: ../src/msr/msrMeasureElements.h
../src/msr/msrStaves.o: ../src/msr/msrBasicTypes.h
../src/msr/msrStaves.o: ../src/msr/msrVoiceElements.h
../src/msr/msrStaves.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrStaves.o: ../src/msr/msrIdentification.h
../src/msr/msrStaves.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrStaves.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrStaves.o: ../src/msr/msrDivisions.h
../src/msr/msrStaves.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrStaves.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrStaves.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrStaves.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrStaves.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrStaves.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrStaves.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrStaves.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrStaves.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrStaves.o: ../src/msr/msrArticulations.h
../src/msr/msrStaves.o: ../src/msr/msrTablatures.h
../src/msr/msrStaves.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrStaves.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrStaves.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrStaves.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrStaves.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrStaves.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrStaves.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrStaves.o: ../src/interface/libmusicxml.h
../src/msr/msrStaves.o: ../src/oah/oahElements.h
../src/msr/msrStaves.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrStaves.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrStaves.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrStaves.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrStaves.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrStaves.o: ../src/oah/mxmlTree2msrOah.h
../src/msr/msrStavesDetails.o: ../src/utilities/utilities.h
../src/msr/msrStavesDetails.o: ../src/lib/smartpointer.h
../src/msr/msrStavesDetails.o: ../src/elements/exports.h
../src/msr/msrStavesDetails.o: ../src/visitors/basevisitor.h
../src/msr/msrStavesDetails.o: ../src/msr/msrStavesDetails.h
../src/msr/msrStavesDetails.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrStavesDetails.o: ../src/msr/msrMeasureElements.h
../src/msr/msrStavesDetails.o: ../src/msr/msrElements.h
../src/msr/msrStavesDetails.o: ../src/elements/typedefs.h
../src/msr/msrStavesDetails.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrStavesDetails.o: ../src/elements/elements.h
../src/msr/msrStavesDetails.o: ../src/elements/xml/xmlElements.h
../src/msr/msrStavesDetails.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrStavesDetails.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrStavesDetails.o: ../src/visitors/visitor.h
../src/msr/msrStavesDetails.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrStavesDetails.o: ../src/lib/rational.h
../src/msr/msrStavesDetails.o: ../src/msr/msrBasicTypes.h
../src/msr/msrStavesDetails.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrStavesDetails.o: ../src/oah/traceOah.h
../src/msr/msrStavesDetails.o: ../src/oah/oahBasicTypes.h
../src/msr/msrStavesDetails.o: ../src/interface/libmusicxml.h
../src/msr/msrStavesDetails.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrStavesDetails.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrStavesDetails.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrStems.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrStems.o: ../src/elements/exports.h
../src/msr/msrStems.o: ../src/visitors/basevisitor.h ../src/msr/msrStems.h
../src/msr/msrStems.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrStems.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrStems.o: ../src/elements/elements.h
../src/msr/msrStems.o: ../src/elements/xml/xmlElements.h
../src/msr/msrStems.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrStems.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrStems.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrStems.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrStems.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrStems.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrStems.o: ../src/oah/oahBasicTypes.h
../src/msr/msrStems.o: ../src/interface/libmusicxml.h
../src/msr/msrStems.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrSyllables.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrSyllables.o: ../src/elements/typedefs.h
../src/msr/msrSyllables.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrSyllables.o: ../src/elements/elements.h
../src/msr/msrSyllables.o: ../src/elements/xml/xmlElements.h
../src/msr/msrSyllables.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrSyllables.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrSyllables.o: ../src/lib/smartpointer.h
../src/msr/msrSyllables.o: ../src/visitors/visitable.h
../src/msr/msrSyllables.o: ../src/visitors/basevisitor.h
../src/msr/msrSyllables.o: ../src/visitors/visitor.h
../src/msr/msrSyllables.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrSyllables.o: ../src/utilities/utilities.h
../src/msr/msrSyllables.o: ../src/utilities/messagesHandling.h
../src/msr/msrSyllables.o: ../src/utilities/exceptions.h
../src/msr/msrSyllables.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrSyllables.o: ../src/msr/msrTupletElements.h
../src/msr/msrSyllables.o: ../src/msr/msrMeasureElements.h
../src/msr/msrSyllables.o: ../src/msr/msrBasicTypes.h
../src/msr/msrSyllables.o: ../src/msr/msrVoiceElements.h
../src/msr/msrSyllables.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrSyllables.o: ../src/msr/msrIdentification.h
../src/msr/msrSyllables.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrSyllables.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrSyllables.o: ../src/msr/msrDivisions.h
../src/msr/msrSyllables.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrSyllables.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrSyllables.o: ../src/msr/msrStavesDetails.h
../src/msr/msrSyllables.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/msr/msrSyllables.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/msr/msrSyllables.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/msr/msrSyllables.o: ../src/msr/msrRehearsals.h
../src/msr/msrSyllables.o: ../src/msr/msrTranspositions.h
../src/msr/msrSyllables.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrSyllables.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrSyllables.o: ../src/msr/msrDynamics.h
../src/msr/msrSyllables.o: ../src/msr/msrArticulations.h
../src/msr/msrSyllables.o: ../src/msr/msrTablatures.h
../src/msr/msrSyllables.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrSyllables.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrSyllables.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrSyllables.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrSyllables.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrSyllables.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrSyllables.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrSyllables.o: ../src/interface/libmusicxml.h
../src/msr/msrSyllables.o: ../src/oah/oahElements.h
../src/msr/msrSyllables.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrSyllables.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrSyllables.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrSyllables.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrSyllables.o: ../src/msr/msrOah.h
../src/msr/msrTablatures.o: ../src/utilities/utilities.h
../src/msr/msrTablatures.o: ../src/lib/smartpointer.h
../src/msr/msrTablatures.o: ../src/elements/exports.h
../src/msr/msrTablatures.o: ../src/visitors/basevisitor.h
../src/msr/msrTablatures.o: ../src/utilities/messagesHandling.h
../src/msr/msrTablatures.o: ../src/utilities/exceptions.h
../src/msr/msrTablatures.o: ../src/msr/msrTablatures.h
../src/msr/msrTablatures.o: ../src/msr/msrElements.h
../src/msr/msrTablatures.o: ../src/elements/typedefs.h
../src/msr/msrTablatures.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTablatures.o: ../src/elements/elements.h
../src/msr/msrTablatures.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTablatures.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTablatures.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrTablatures.o: ../src/visitors/visitor.h
../src/msr/msrTablatures.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrTablatures.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrTablatures.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrTablatures.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrTablatures.o: ../src/interface/libmusicxml.h
../src/msr/msrTablatures.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrTablatures.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrTablatures.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/msr/msrTablatures.o: ../src/msr/msrOah.h
../src/msr/msrTechnicals.o: ../src/utilities/utilities.h
../src/msr/msrTechnicals.o: ../src/lib/smartpointer.h
../src/msr/msrTechnicals.o: ../src/elements/exports.h
../src/msr/msrTechnicals.o: ../src/visitors/basevisitor.h
../src/msr/msrTechnicals.o: ../src/msr/msrTechnicals.h
../src/msr/msrTechnicals.o: ../src/msr/msrElements.h
../src/msr/msrTechnicals.o: ../src/elements/typedefs.h
../src/msr/msrTechnicals.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTechnicals.o: ../src/elements/elements.h
../src/msr/msrTechnicals.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTechnicals.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTechnicals.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrTechnicals.o: ../src/visitors/visitor.h
../src/msr/msrTechnicals.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrTechnicals.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/msr/msrTechnicals.o: ../src/oah/oahOah.h
../src/msr/msrTechnicals.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrTechnicals.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/msr/msrTechnicals.o: ../src/interface/libmusicxml.h
../src/msr/msrTechnicals.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/msr/msrTempos.o: ../src/utilities/utilities.h
../src/msr/msrTempos.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/msr/msrTempos.o: ../src/visitors/basevisitor.h
../src/msr/msrTempos.o: ../src/utilities/messagesHandling.h
../src/msr/msrTempos.o: ../src/utilities/exceptions.h ../src/msr/msrTempos.h
../src/msr/msrTempos.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrTempos.o: ../src/msr/msrMeasureElements.h
../src/msr/msrTempos.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrTempos.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTempos.o: ../src/elements/elements.h
../src/msr/msrTempos.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTempos.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTempos.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrTempos.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrTempos.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrTempos.o: ../src/msr/msrBasicTypes.h ../src/msr/msrBeams.h
../src/msr/msrTempos.o: ../src/msr/msrWords.h
../src/msr/msrTempos.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrTempos.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrTempos.o: ../src/interface/libmusicxml.h
../src/msr/msrTempos.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrTempos.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrTempos.o: ../src/oah/generalOah.h ../src/msr/msrOah.h
../src/msr/msrTies.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrTies.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/msr/msrTies.o: ../src/msr/msrTies.h ../src/msr/msrElements.h
../src/msr/msrTies.o: ../src/elements/typedefs.h
../src/msr/msrTies.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTies.o: ../src/elements/elements.h
../src/msr/msrTies.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTies.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTies.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrTies.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrTies.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrTies.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrTies.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrTies.o: ../src/oah/oahBasicTypes.h
../src/msr/msrTies.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/msr/msrTies.o: ../src/msr/msrOah.h
../src/msr/msrTimes.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrTimes.o: ../src/elements/exports.h
../src/msr/msrTimes.o: ../src/visitors/basevisitor.h
../src/msr/msrTimes.o: ../src/utilities/messagesHandling.h
../src/msr/msrTimes.o: ../src/utilities/exceptions.h ../src/msr/msrTimes.h
../src/msr/msrTimes.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrTimes.o: ../src/msr/msrMeasureElements.h
../src/msr/msrTimes.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrTimes.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTimes.o: ../src/elements/elements.h
../src/msr/msrTimes.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTimes.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTimes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrTimes.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrTimes.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrTimes.o: ../src/msr/msrBasicTypes.h
../src/msr/msrTimes.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrTimes.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrTimes.o: ../src/interface/libmusicxml.h
../src/msr/msrTimes.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrTimes.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrTimes.o: ../src/oah/generalOah.h ../src/msr/msrOah.h
../src/msr/msrTranspositions.o: ../src/utilities/utilities.h
../src/msr/msrTranspositions.o: ../src/lib/smartpointer.h
../src/msr/msrTranspositions.o: ../src/elements/exports.h
../src/msr/msrTranspositions.o: ../src/visitors/basevisitor.h
../src/msr/msrTranspositions.o: ../src/msr/msrTranspositions.h
../src/msr/msrTranspositions.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrTranspositions.o: ../src/msr/msrMeasureElements.h
../src/msr/msrTranspositions.o: ../src/msr/msrElements.h
../src/msr/msrTranspositions.o: ../src/elements/typedefs.h
../src/msr/msrTranspositions.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTranspositions.o: ../src/elements/elements.h
../src/msr/msrTranspositions.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTranspositions.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrTranspositions.o: ../src/lib/ctree.h
../src/msr/msrTranspositions.o: ../src/visitors/visitable.h
../src/msr/msrTranspositions.o: ../src/visitors/visitor.h
../src/msr/msrTranspositions.o: ../src/lib/tree_browser.h
../src/msr/msrTranspositions.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrTranspositions.o: ../src/msr/msrBasicTypes.h
../src/msr/msrTranspositions.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrTranspositions.o: ../src/oah/traceOah.h
../src/msr/msrTranspositions.o: ../src/oah/oahBasicTypes.h
../src/msr/msrTranspositions.o: ../src/interface/libmusicxml.h
../src/msr/msrTranspositions.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrTranspositions.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrTranspositions.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/msr/msrTranspositions.o: ../src/msr/msrOah.h
../src/msr/msrTupletElements.o: ../src/msr/msrTupletElements.h
../src/msr/msrTupletElements.o: ../src/msr/msrMeasureElements.h
../src/msr/msrTupletElements.o: ../src/msr/msrElements.h
../src/msr/msrTupletElements.o: ../src/elements/typedefs.h
../src/msr/msrTupletElements.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTupletElements.o: ../src/elements/elements.h
../src/msr/msrTupletElements.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTupletElements.o: ../src/elements/types.h
../src/msr/msrTupletElements.o: ../src/elements/exports.h
../src/msr/msrTupletElements.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrTupletElements.o: ../src/lib/smartpointer.h
../src/msr/msrTupletElements.o: ../src/visitors/visitable.h
../src/msr/msrTupletElements.o: ../src/visitors/basevisitor.h
../src/msr/msrTupletElements.o: ../src/visitors/visitor.h
../src/msr/msrTupletElements.o: ../src/lib/tree_browser.h
../src/msr/msrTupletElements.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrTupletElements.o: ../src/msr/msrBasicTypes.h
../src/msr/msrTuplets.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrTuplets.o: ../src/elements/typedefs.h
../src/msr/msrTuplets.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrTuplets.o: ../src/elements/elements.h
../src/msr/msrTuplets.o: ../src/elements/xml/xmlElements.h
../src/msr/msrTuplets.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrTuplets.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrTuplets.o: ../src/lib/smartpointer.h
../src/msr/msrTuplets.o: ../src/visitors/visitable.h
../src/msr/msrTuplets.o: ../src/visitors/basevisitor.h
../src/msr/msrTuplets.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrTuplets.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrTuplets.o: ../src/utilities/messagesHandling.h
../src/msr/msrTuplets.o: ../src/utilities/exceptions.h
../src/msr/msrTuplets.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrTuplets.o: ../src/msr/msrTupletElements.h
../src/msr/msrTuplets.o: ../src/msr/msrMeasureElements.h
../src/msr/msrTuplets.o: ../src/msr/msrBasicTypes.h
../src/msr/msrTuplets.o: ../src/msr/msrVoiceElements.h
../src/msr/msrTuplets.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrTuplets.o: ../src/msr/msrIdentification.h
../src/msr/msrTuplets.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrTuplets.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrTuplets.o: ../src/msr/msrDivisions.h
../src/msr/msrTuplets.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrTuplets.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrTuplets.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrTuplets.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrTuplets.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrTuplets.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrTuplets.o: ../src/msr/msrTranspositions.h
../src/msr/msrTuplets.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/msr/msrTuplets.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/msr/msrTuplets.o: ../src/msr/msrDynamics.h
../src/msr/msrTuplets.o: ../src/msr/msrArticulations.h
../src/msr/msrTuplets.o: ../src/msr/msrTablatures.h
../src/msr/msrTuplets.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrTuplets.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrTuplets.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrTuplets.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrTuplets.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrTuplets.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrTuplets.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrTuplets.o: ../src/interface/libmusicxml.h
../src/msr/msrTuplets.o: ../src/oah/oahElements.h
../src/msr/msrTuplets.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrTuplets.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrTuplets.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrTuplets.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrVarValAssocs.o: ../src/utilities/utilities.h
../src/msr/msrVarValAssocs.o: ../src/lib/smartpointer.h
../src/msr/msrVarValAssocs.o: ../src/elements/exports.h
../src/msr/msrVarValAssocs.o: ../src/visitors/basevisitor.h
../src/msr/msrVarValAssocs.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrVarValAssocs.o: ../src/msr/msrElements.h
../src/msr/msrVarValAssocs.o: ../src/elements/typedefs.h
../src/msr/msrVarValAssocs.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrVarValAssocs.o: ../src/elements/elements.h
../src/msr/msrVarValAssocs.o: ../src/elements/xml/xmlElements.h
../src/msr/msrVarValAssocs.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrVarValAssocs.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrVarValAssocs.o: ../src/visitors/visitor.h
../src/msr/msrVarValAssocs.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrVarValAssocs.o: ../src/lib/rational.h
../src/msr/msrVarValAssocs.o: ../src/msr/msrBasicTypes.h
../src/msr/msrVarValAssocs.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrVarValAssocs.o: ../src/oah/traceOah.h
../src/msr/msrVarValAssocs.o: ../src/oah/oahBasicTypes.h
../src/msr/msrVarValAssocs.o: ../src/interface/libmusicxml.h
../src/msr/msrVarValAssocs.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/msr/msrVarValAssocs.o: ../src/oah/oahAtomsCollection.h
../src/msr/msrVarValAssocs.o: ../src/msr/msrMidi.h ../src/msr/msrOah.h
../src/msr/msrVoiceElements.o: ../src/msr/msrVoiceElements.h
../src/msr/msrVoiceElements.o: ../src/msr/msrElements.h
../src/msr/msrVoiceElements.o: ../src/elements/typedefs.h
../src/msr/msrVoiceElements.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrVoiceElements.o: ../src/elements/elements.h
../src/msr/msrVoiceElements.o: ../src/elements/xml/xmlElements.h
../src/msr/msrVoiceElements.o: ../src/elements/types.h
../src/msr/msrVoiceElements.o: ../src/elements/exports.h
../src/msr/msrVoiceElements.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrVoiceElements.o: ../src/lib/smartpointer.h
../src/msr/msrVoiceElements.o: ../src/visitors/visitable.h
../src/msr/msrVoiceElements.o: ../src/visitors/basevisitor.h
../src/msr/msrVoiceElements.o: ../src/visitors/visitor.h
../src/msr/msrVoiceElements.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/msr/msrVoiceElements.o: ../src/lib/rational.h
../src/msr/msrVoices.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/msr/msrVoices.o: ../src/elements/typedefs.h
../src/msr/msrVoices.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrVoices.o: ../src/elements/elements.h
../src/msr/msrVoices.o: ../src/elements/xml/xmlElements.h
../src/msr/msrVoices.o: ../src/elements/types.h ../src/elements/exports.h
../src/msr/msrVoices.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/msr/msrVoices.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/msr/msrVoices.o: ../src/visitors/basevisitor.h
../src/msr/msrVoices.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrVoices.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/msr/msrVoices.o: ../src/utilities/messagesHandling.h
../src/msr/msrVoices.o: ../src/utilities/exceptions.h
../src/msr/msrVoices.o: ../src/msr/msrElements.h ../src/lib/rational.h
../src/msr/msrVoices.o: ../src/msr/msrTupletElements.h
../src/msr/msrVoices.o: ../src/msr/msrMeasureElements.h
../src/msr/msrVoices.o: ../src/msr/msrBasicTypes.h
../src/msr/msrVoices.o: ../src/msr/msrVoiceElements.h
../src/msr/msrVoices.o: ../src/msr/msrVarValAssocs.h
../src/msr/msrVoices.o: ../src/msr/msrIdentification.h
../src/msr/msrVoices.o: ../src/msr/msrMusicXMLSpecifics.h
../src/msr/msrVoices.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/msr/msrVoices.o: ../src/msr/msrDivisions.h
../src/msr/msrVoices.o: ../src/msr/msrPartNamesDisplays.h
../src/msr/msrVoices.o: ../src/msr/msrSegnosAndCodas.h
../src/msr/msrVoices.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/msr/msrVoices.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/msr/msrVoices.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/msr/msrVoices.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/msr/msrVoices.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/msr/msrVoices.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/msr/msrVoices.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/msr/msrVoices.o: ../src/msr/msrArticulations.h
../src/msr/msrVoices.o: ../src/msr/msrTablatures.h
../src/msr/msrVoices.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/msr/msrVoices.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/msr/msrVoices.o: ../src/msr/msrSingleTremolos.h
../src/msr/msrVoices.o: ../src/msr/msrSlashes.h ../src/msr/msrPartGroups.h
../src/msr/msrVoices.o: ../src/msr/msrPartGroupElements.h
../src/msr/msrVoices.o: ../src/msr/msrScores.h ../src/msr/msrMidi.h
../src/msr/msrVoices.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/msr/msrVoices.o: ../src/interface/libmusicxml.h
../src/msr/msrVoices.o: ../src/oah/oahElements.h
../src/msr/msrVoices.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrSegments_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrSlides_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrNotes_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrChords_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrVoices_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrStaves_MUT_DEP.h
../src/msr/msrVoices.o: ../src/msr/msrParts_MUT_DEP.h
../src/msr/msrVoices.o: ../src/oah/enableTracingIfDesired.h
../src/msr/msrVoices.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/msr/msrVoices.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrOah.h
../src/msr/msrWords.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/msr/msrWords.o: ../src/elements/exports.h
../src/msr/msrWords.o: ../src/visitors/basevisitor.h ../src/msr/msrWords.h
../src/msr/msrWords.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/msr/msrWords.o: ../src/elements/xml/xmlTypedefs.h
../src/msr/msrWords.o: ../src/elements/elements.h
../src/msr/msrWords.o: ../src/elements/xml/xmlElements.h
../src/msr/msrWords.o: ../src/elements/types.h ../src/elements/xml.h
../src/msr/msrWords.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/msr/msrWords.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/msr/msrWords.o: ../src/lib/browser.h ../src/lib/rational.h
../src/msr/msrWords.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/msr/msrWords.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/msr/msrWords.o: ../src/oah/oahBasicTypes.h
../src/msr/msrWords.o: ../src/interface/libmusicxml.h
../src/msr/msrWords.o: ../src/oah/oahElements.h ../src/msr/msrOah.h
../src/oah/extraOah.o: ../src/oah/extraOah.h
../src/oah/extraOah.o: ../src/oah/enableExtraOahIfDesired.h
../src/oah/extraOah.o: ../src/msr/msrBasicTypes.h ../src/lib/smartpointer.h
../src/oah/extraOah.o: ../src/elements/exports.h ../src/lib/rational.h
../src/oah/extraOah.o: ../src/oah/oahBasicTypes.h
../src/oah/extraOah.o: ../src/interface/libmusicxml.h
../src/oah/extraOah.o: ../src/utilities/utilities.h
../src/oah/extraOah.o: ../src/visitors/basevisitor.h ../src/oah/oahElements.h
../src/oah/extraOah.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/oah/extraOah.o: ../src/visitors/visitable.h ../src/utilities/version.h
../src/oah/extraOah.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/oah/extraOah.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/oah/extraOah.o: ../src/elements/typedefs.h
../src/oah/extraOah.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/extraOah.o: ../src/elements/elements.h
../src/oah/extraOah.o: ../src/elements/xml/xmlElements.h
../src/oah/extraOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/extraOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/oah/extraOah.o: ../src/oah/generalOah.h ../src/lpsr/lpsrOah.h
../src/oah/extraOah.o: ../src/lpsr/lpsrBasicTypes.h
../src/oah/extraOah.o: ../src/oah/enableTracingIfDesired.h
../src/oah/extraOah.o: ../src/oah/traceOah.h
../src/oah/extraOah.o: ../src/utilities/messagesHandling.h
../src/oah/extraOah.o: ../src/utilities/exceptions.h
../src/oah/generalOah.o: ../src/utilities/utilities.h
../src/oah/generalOah.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/oah/generalOah.o: ../src/visitors/basevisitor.h
../src/oah/generalOah.o: ../src/oah/enableTracingIfDesired.h
../src/oah/generalOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/oah/generalOah.o: ../src/interface/libmusicxml.h
../src/oah/generalOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/generalOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/generalOah.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/oah/generalOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/generalOah.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/oah/generalOah.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/generalOah.o: ../src/elements/elements.h
../src/oah/generalOah.o: ../src/elements/xml/xmlElements.h
../src/oah/generalOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/generalOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/oah/generalOah.o: ../src/msr/msrBasicTypes.h ../src/oah/generalOah.h
../src/oah/mxmlTree2msrOah.o: ../src/utilities/version.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/exports.h
../src/oah/mxmlTree2msrOah.o: ../src/utilities/utilities.h
../src/oah/mxmlTree2msrOah.o: ../src/lib/smartpointer.h
../src/oah/mxmlTree2msrOah.o: ../src/visitors/basevisitor.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/enableTracingIfDesired.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/traceOah.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/oahBasicTypes.h
../src/oah/mxmlTree2msrOah.o: ../src/interface/libmusicxml.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/mxmlTree2msrOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/mxmlTree2msrOah.o: ../src/utilities/messagesHandling.h
../src/oah/mxmlTree2msrOah.o: ../src/utilities/exceptions.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/oahOah.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/oahAtomsCollection.h
../src/oah/mxmlTree2msrOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/mxmlTree2msrOah.o: ../src/msr/msrElements.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/typedefs.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/elements.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/xml/xmlElements.h
../src/oah/mxmlTree2msrOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/mxmlTree2msrOah.o: ../src/visitors/visitor.h
../src/oah/mxmlTree2msrOah.o: ../src/lib/tree_browser.h
../src/oah/mxmlTree2msrOah.o: ../src/msr/msrBasicTypes.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/generalOah.h
../src/oah/mxmlTree2msrOah.o: ../src/oah/mxmlTree2msrOah.h
../src/oah/mxmlTree2msrOah.o: ../src/msr/msrSegnosAndCodas.h
../src/oah/mxmlTree2msrOah.o: ../src/msr/msrMeasureElements.h
../src/oah/oahAtomsCollection.o: ../src/utilities/utilities.h
../src/oah/oahAtomsCollection.o: ../src/lib/smartpointer.h
../src/oah/oahAtomsCollection.o: ../src/elements/exports.h
../src/oah/oahAtomsCollection.o: ../src/visitors/basevisitor.h
../src/oah/oahAtomsCollection.o: ../src/utilities/messagesHandling.h
../src/oah/oahAtomsCollection.o: ../src/utilities/exceptions.h
../src/oah/oahAtomsCollection.o: ../src/utilities/version.h
../src/oah/oahAtomsCollection.o: ../src/oah/enableTracingIfDesired.h
../src/oah/oahAtomsCollection.o: ../src/oah/traceOah.h
../src/oah/oahAtomsCollection.o: ../src/oah/oahBasicTypes.h
../src/oah/oahAtomsCollection.o: ../src/interface/libmusicxml.h
../src/oah/oahAtomsCollection.o: ../src/oah/oahElements.h
../src/oah/oahAtomsCollection.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/oah/oahAtomsCollection.o: ../src/visitors/visitable.h
../src/oah/oahAtomsCollection.o: ../src/oah/oahOah.h
../src/oah/oahAtomsCollection.o: ../src/oah/oahAtomsCollection.h
../src/oah/oahAtomsCollection.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/oahAtomsCollection.o: ../src/msr/msrElements.h
../src/oah/oahAtomsCollection.o: ../src/elements/typedefs.h
../src/oah/oahAtomsCollection.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/oahAtomsCollection.o: ../src/elements/elements.h
../src/oah/oahAtomsCollection.o: ../src/elements/xml/xmlElements.h
../src/oah/oahAtomsCollection.o: ../src/elements/types.h
../src/oah/oahAtomsCollection.o: ../src/elements/xml.h
../src/oah/oahAtomsCollection.o: ../src/visitors/visitor.h
../src/oah/oahAtomsCollection.o: ../src/lib/tree_browser.h
../src/oah/oahAtomsCollection.o: ../src/msr/msrBasicTypes.h
../src/oah/oahBasicTypes.o: ../src/utilities/utilities.h
../src/oah/oahBasicTypes.o: ../src/lib/smartpointer.h
../src/oah/oahBasicTypes.o: ../src/elements/exports.h
../src/oah/oahBasicTypes.o: ../src/visitors/basevisitor.h
../src/oah/oahBasicTypes.o: ../src/utilities/messagesHandling.h
../src/oah/oahBasicTypes.o: ../src/utilities/exceptions.h
../src/oah/oahBasicTypes.o: ../src/oah/enableTracingIfDesired.h
../src/oah/oahBasicTypes.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/oah/oahBasicTypes.o: ../src/interface/libmusicxml.h
../src/oah/oahBasicTypes.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/oahBasicTypes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/oahBasicTypes.o: ../src/oah/generalOah.h ../src/oah/oahOah.h
../src/oah/oahBasicTypes.o: ../src/oah/oahAtomsCollection.h
../src/oah/oahBasicTypes.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/oahBasicTypes.o: ../src/msr/msrElements.h
../src/oah/oahBasicTypes.o: ../src/elements/typedefs.h
../src/oah/oahBasicTypes.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/oahBasicTypes.o: ../src/elements/elements.h
../src/oah/oahBasicTypes.o: ../src/elements/xml/xmlElements.h
../src/oah/oahBasicTypes.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/oahBasicTypes.o: ../src/visitors/visitor.h
../src/oah/oahBasicTypes.o: ../src/lib/tree_browser.h
../src/oah/oahBasicTypes.o: ../src/msr/msrBasicTypes.h
../src/oah/oahBasicTypes.o: ../src/utilities/version.h
../src/oah/oahElements.o: ../src/utilities/utilities.h
../src/oah/oahElements.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/oah/oahElements.o: ../src/visitors/basevisitor.h
../src/oah/oahElements.o: ../src/utilities/messagesHandling.h
../src/oah/oahElements.o: ../src/utilities/exceptions.h
../src/oah/oahElements.o: ../src/oah/enableTracingIfDesired.h
../src/oah/oahElements.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/oah/oahElements.o: ../src/interface/libmusicxml.h
../src/oah/oahElements.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/oahElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/oahElements.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/oah/oahElements.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/oahElements.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/oah/oahElements.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/oahElements.o: ../src/elements/elements.h
../src/oah/oahElements.o: ../src/elements/xml/xmlElements.h
../src/oah/oahElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/oahElements.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/oah/oahElements.o: ../src/msr/msrBasicTypes.h
../src/oah/oahOah.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/oah/oahOah.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/oah/oahOah.o: ../src/utilities/messagesHandling.h
../src/oah/oahOah.o: ../src/utilities/exceptions.h
../src/oah/oahOah.o: ../src/oah/enableTracingIfDesired.h
../src/oah/oahOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/oah/oahOah.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/oah/oahOah.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/oah/oahOah.o: ../src/visitors/visitable.h ../src/oah/oahOah.h
../src/oah/oahOah.o: ../src/oah/oahAtomsCollection.h ../src/lib/rational.h
../src/oah/oahOah.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/oah/oahOah.o: ../src/elements/typedefs.h
../src/oah/oahOah.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/oahOah.o: ../src/elements/elements.h
../src/oah/oahOah.o: ../src/elements/xml/xmlElements.h
../src/oah/oahOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/oahOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/oah/oahOah.o: ../src/msr/msrBasicTypes.h
../src/oah/oahRegularHandlers.o: ../src/utilities/utilities.h
../src/oah/oahRegularHandlers.o: ../src/lib/smartpointer.h
../src/oah/oahRegularHandlers.o: ../src/elements/exports.h
../src/oah/oahRegularHandlers.o: ../src/visitors/basevisitor.h
../src/oah/oahRegularHandlers.o: ../src/utilities/messagesHandling.h
../src/oah/oahRegularHandlers.o: ../src/utilities/exceptions.h
../src/oah/oahRegularHandlers.o: ../src/oah/enableTracingIfDesired.h
../src/oah/oahRegularHandlers.o: ../src/oah/traceOah.h
../src/oah/oahRegularHandlers.o: ../src/oah/oahBasicTypes.h
../src/oah/oahRegularHandlers.o: ../src/interface/libmusicxml.h
../src/oah/oahRegularHandlers.o: ../src/oah/oahElements.h
../src/oah/oahRegularHandlers.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/oah/oahRegularHandlers.o: ../src/visitors/visitable.h
../src/oah/oahRegularHandlers.o: ../src/oah/generalOah.h ../src/oah/oahOah.h
../src/oah/oahRegularHandlers.o: ../src/oah/oahAtomsCollection.h
../src/oah/oahRegularHandlers.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/oahRegularHandlers.o: ../src/msr/msrElements.h
../src/oah/oahRegularHandlers.o: ../src/elements/typedefs.h
../src/oah/oahRegularHandlers.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/oahRegularHandlers.o: ../src/elements/elements.h
../src/oah/oahRegularHandlers.o: ../src/elements/xml/xmlElements.h
../src/oah/oahRegularHandlers.o: ../src/elements/types.h
../src/oah/oahRegularHandlers.o: ../src/elements/xml.h
../src/oah/oahRegularHandlers.o: ../src/visitors/visitor.h
../src/oah/oahRegularHandlers.o: ../src/lib/tree_browser.h
../src/oah/oahRegularHandlers.o: ../src/msr/msrBasicTypes.h
../src/oah/oahRegularHandlers.o: ../src/oah/oahRegularHandlers.h
../src/oah/oahRegularHandlers.o: ../src/utilities/version.h
../src/oah/oahVisitor.o: ../src/oah/oahVisitor.h ../src/visitors/visitor.h
../src/oah/oahVisitor.o: ../src/visitors/basevisitor.h
../src/oah/oahVisitor.o: ../src/oah/oahBasicTypes.h ../src/lib/smartpointer.h
../src/oah/oahVisitor.o: ../src/elements/exports.h
../src/oah/oahVisitor.o: ../src/interface/libmusicxml.h
../src/oah/oahVisitor.o: ../src/utilities/utilities.h
../src/oah/oahVisitor.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/oahVisitor.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/traceOah.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/oah/traceOah.o: ../src/elements/exports.h
../src/oah/traceOah.o: ../src/visitors/basevisitor.h
../src/oah/traceOah.o: ../src/oah/enableTracingIfDesired.h
../src/oah/traceOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/oah/traceOah.o: ../src/interface/libmusicxml.h
../src/oah/traceOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/oah/traceOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/oah/traceOah.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/oah/traceOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/oah/traceOah.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/oah/traceOah.o: ../src/elements/xml/xmlTypedefs.h
../src/oah/traceOah.o: ../src/elements/elements.h
../src/oah/traceOah.o: ../src/elements/xml/xmlElements.h
../src/oah/traceOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/oah/traceOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/oah/traceOah.o: ../src/msr/msrBasicTypes.h ../src/oah/generalOah.h
../src/oah/traceOah.o: ../src/utilities/messagesHandling.h
../src/oah/traceOah.o: ../src/utilities/exceptions.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsr.h ../src/msr/msr.h
../src/lpsr/lpsr.o: ../src/elements/typedefs.h
../src/lpsr/lpsr.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsr.o: ../src/elements/elements.h
../src/lpsr/lpsr.o: ../src/elements/xml/xmlElements.h ../src/elements/types.h
../src/lpsr/lpsr.o: ../src/elements/exports.h ../src/elements/xml.h
../src/lpsr/lpsr.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/lpsr/lpsr.o: ../src/visitors/visitable.h ../src/visitors/basevisitor.h
../src/lpsr/lpsr.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsr.o: ../src/lib/browser.h ../src/utilities/utilities.h
../src/lpsr/lpsr.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsr.o: ../src/utilities/exceptions.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsr.o: ../src/lib/rational.h ../src/msr/msrElements.h
../src/lpsr/lpsr.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsr.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsr.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsr.o: ../src/msr/msrMeasureElements.h ../src/msr/msrScaling.h
../src/lpsr/lpsr.o: ../src/msr/msrCredits.h ../src/msr/msrDivisions.h
../src/lpsr/lpsr.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsr.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsr.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsr.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsr.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsr.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsr.o: ../src/msr/msrTranspositions.h ../src/msr/msrBreaks.h
../src/lpsr/lpsr.o: ../src/msr/msrStems.h ../src/msr/msrTechnicals.h
../src/lpsr/lpsr.o: ../src/msr/msrBars.h ../src/msr/msrDynamics.h
../src/lpsr/lpsr.o: ../src/msr/msrArticulations.h ../src/msr/msrTablatures.h
../src/lpsr/lpsr.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsr.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsr.o: ../src/msr/msrSingleTremolos.h ../src/msr/msrSlashes.h
../src/lpsr/lpsr.o: ../src/msr/msrScores.h ../src/msr/msrPartGroups.h
../src/lpsr/lpsr.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsr.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsr.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsr.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/lpsr/lpsr.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsr.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/lpsr/lpsr.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsr.o: ../src/msr/msrParts_MUT_DEP.h ../src/lpsr/lpsrComments.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrElements.h ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrScheme.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsr.o: ../src/oah/enableTracingIfDesired.h ../src/oah/traceOah.h
../src/lpsr/lpsr.o: ../src/oah/oahAtomsCollection.h ../src/lpsr/lpsrHeaders.h
../src/lpsr/lpsr.o: ../src/lilypond/lilypondOah.h ../src/lpsr/lpsrPapers.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrLayouts.h ../src/lpsr/lpsrBarNumbers.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrContexts.h ../src/lpsr/lpsrVariables.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrVoices.h ../src/lpsr/lpsrLyrics.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrStaves.h ../src/lpsr/lpsrParts.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrPartGroups.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrBookBlockElements.h
../src/lpsr/lpsr.o: ../src/lpsr/lpsrParallelMusic.h ../src/lpsr/lpsrScores.h
../src/lpsr/lpsrBarNumbers.o: ../src/lpsr/lpsrBarNumbers.h
../src/lpsr/lpsrBarNumbers.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrBarNumbers.o: ../src/msr/msrElements.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/typedefs.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/elements.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/types.h
../src/lpsr/lpsrBarNumbers.o: ../src/elements/exports.h ../src/elements/xml.h
../src/lpsr/lpsrBarNumbers.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/lpsr/lpsrBarNumbers.o: ../src/visitors/visitable.h
../src/lpsr/lpsrBarNumbers.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrBarNumbers.o: ../src/visitors/visitor.h
../src/lpsr/lpsrBarNumbers.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrBarNumbers.o: ../src/lib/rational.h
../src/lpsr/lpsrBarNumbers.o: ../src/utilities/utilities.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/oahOah.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrBarNumbers.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrBarNumbers.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/oahElements.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrBarNumbers.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrBarNumbers.o: ../src/oah/traceOah.h
../src/lpsr/lpsrBasicTypes.o: ../src/utilities/utilities.h
../src/lpsr/lpsrBasicTypes.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/exports.h
../src/lpsr/lpsrBasicTypes.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrBasicTypes.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrBasicTypes.o: ../src/lib/rational.h
../src/lpsr/lpsrBasicTypes.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/traceOah.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrBasicTypes.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/lpsr/lpsrBasicTypes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/oahOah.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrBasicTypes.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/typedefs.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/elements.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrBasicTypes.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrBasicTypes.o: ../src/visitors/visitor.h
../src/lpsr/lpsrBasicTypes.o: ../src/lib/tree_browser.h
../src/lpsr/lpsrBasicTypes.o: ../src/oah/generalOah.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrBasicTypes.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrBasicTypes.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrBookBlockElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrComments.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/typedefs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/elements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/types.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/exports.h
../src/lpsr/lpsrBookBlockElements.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrBookBlockElements.o: ../src/visitors/visitable.h
../src/lpsr/lpsrBookBlockElements.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrBookBlockElements.o: ../src/visitors/visitor.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lib/tree_browser.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lib/browser.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lib/rational.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrScheme.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/traceOah.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrBookBlockElements.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrBookBlockElements.o: ../src/utilities/utilities.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/oahElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrMidi.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrHeaders.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lilypond/lilypondOah.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrPapers.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrScaling.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrBookBlockElements.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrLayouts.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrParallelMusic.h
../src/lpsr/lpsrBookBlockElements.o: ../src/lpsr/lpsrPartGroups.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msr.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrCredits.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrStavesDetails.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrClefs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrKeys.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTimes.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTempos.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrBeams.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrWords.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrRehearsals.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrBreaks.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrStems.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTechnicals.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrBars.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrInstruments.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTies.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSlurs.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrLigatures.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSlashes.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrScores.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrVoiceElements.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/generalOah.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrBookBlockElements.o: ../src/utilities/version.h
../src/lpsr/lpsrBookBlockElements.o: ../src/oah/oahOah.h
../src/lpsr/lpsrComments.o: ../src/utilities/utilities.h
../src/lpsr/lpsrComments.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrComments.o: ../src/elements/exports.h
../src/lpsr/lpsrComments.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrComments.o: ../src/lpsr/lpsrComments.h
../src/lpsr/lpsrComments.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrComments.o: ../src/msr/msrElements.h
../src/lpsr/lpsrComments.o: ../src/elements/typedefs.h
../src/lpsr/lpsrComments.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrComments.o: ../src/elements/elements.h
../src/lpsr/lpsrComments.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrComments.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrComments.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrComments.o: ../src/visitors/visitor.h
../src/lpsr/lpsrComments.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrComments.o: ../src/lib/rational.h ../src/oah/oahOah.h
../src/lpsr/lpsrComments.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrComments.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrComments.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrComments.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrComments.o: ../src/oah/oahElements.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrComments.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrComments.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrComments.o: ../src/oah/traceOah.h
../src/lpsr/lpsrContexts.o: ../src/lpsr/lpsrContexts.h
../src/lpsr/lpsrContexts.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrElements.h
../src/lpsr/lpsrContexts.o: ../src/elements/typedefs.h
../src/lpsr/lpsrContexts.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrContexts.o: ../src/elements/elements.h
../src/lpsr/lpsrContexts.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrContexts.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrContexts.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrContexts.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrContexts.o: ../src/visitors/visitable.h
../src/lpsr/lpsrContexts.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrContexts.o: ../src/visitors/visitor.h
../src/lpsr/lpsrContexts.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrContexts.o: ../src/lib/rational.h ../src/msr/msr.h
../src/lpsr/lpsrContexts.o: ../src/utilities/utilities.h
../src/lpsr/lpsrContexts.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrContexts.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrStavesDetails.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrRehearsals.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrVoiceElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/lpsr/lpsrContexts.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrContexts.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrContexts.o: ../src/oah/oahElements.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrContexts.o: ../src/msr/msrParts_MUT_DEP.h ../src/oah/oahOah.h
../src/lpsr/lpsrContexts.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrContexts.o: ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrContexts.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrContexts.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrContexts.o: ../src/oah/traceOah.h
../src/lpsr/lpsrElements.o: ../src/utilities/utilities.h
../src/lpsr/lpsrElements.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrElements.o: ../src/elements/exports.h
../src/lpsr/lpsrElements.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrElements.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrElements.o: ../src/msr/msrElements.h
../src/lpsr/lpsrElements.o: ../src/elements/typedefs.h
../src/lpsr/lpsrElements.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrElements.o: ../src/elements/elements.h
../src/lpsr/lpsrElements.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrElements.o: ../src/visitors/visitor.h
../src/lpsr/lpsrElements.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrElements.o: ../src/lib/rational.h ../src/oah/oahOah.h
../src/lpsr/lpsrElements.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrElements.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrElements.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrElements.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrElements.o: ../src/oah/oahElements.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrElements.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrElements.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrElements.o: ../src/oah/traceOah.h
../src/lpsr/lpsrHeaders.o: ../src/utilities/utilities.h
../src/lpsr/lpsrHeaders.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrHeaders.o: ../src/elements/exports.h
../src/lpsr/lpsrHeaders.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrHeaders.o: ../src/lpsr/lpsrHeaders.h
../src/lpsr/lpsrHeaders.o: ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrHeaders.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrHeaders.o: ../src/msr/msrElements.h
../src/lpsr/lpsrHeaders.o: ../src/elements/typedefs.h
../src/lpsr/lpsrHeaders.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrHeaders.o: ../src/elements/elements.h
../src/lpsr/lpsrHeaders.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrHeaders.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrHeaders.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrHeaders.o: ../src/visitors/visitor.h
../src/lpsr/lpsrHeaders.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrHeaders.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrHeaders.o: ../src/lilypond/lilypondOah.h
../src/lpsr/lpsrHeaders.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrHeaders.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrHeaders.o: ../src/oah/oahElements.h
../src/lpsr/lpsrHeaders.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrHeaders.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/lpsr/lpsrHeaders.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrHeaders.o: ../src/msr/msrMidi.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrHeaders.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrLayouts.o: ../src/utilities/utilities.h
../src/lpsr/lpsrLayouts.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrLayouts.o: ../src/elements/exports.h
../src/lpsr/lpsrLayouts.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrLayouts.o: ../src/lpsr/lpsrLayouts.h
../src/lpsr/lpsrLayouts.o: ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrLayouts.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrLayouts.o: ../src/msr/msrElements.h
../src/lpsr/lpsrLayouts.o: ../src/elements/typedefs.h
../src/lpsr/lpsrLayouts.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrLayouts.o: ../src/elements/elements.h
../src/lpsr/lpsrLayouts.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrLayouts.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrLayouts.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrLayouts.o: ../src/visitors/visitor.h
../src/lpsr/lpsrLayouts.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrLayouts.o: ../src/lib/rational.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrLayouts.o: ../src/lpsr/lpsrScheme.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrLayouts.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrLayouts.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrLayouts.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrLayouts.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrLayouts.o: ../src/oah/oahElements.h
../src/lpsr/lpsrLayouts.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrLayouts.o: ../src/msr/msrMidi.h ../src/oah/oahOah.h
../src/lpsr/lpsrLyrics.o: ../src/lpsr/lpsrLyrics.h ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrLyrics.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrLyrics.o: ../src/elements/elements.h
../src/lpsr/lpsrLyrics.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrLyrics.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrLyrics.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrLyrics.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrLyrics.o: ../src/visitors/visitable.h
../src/lpsr/lpsrLyrics.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrLyrics.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrLyrics.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msr.h ../src/utilities/utilities.h
../src/lpsr/lpsrLyrics.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrLyrics.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/lpsr/lpsrLyrics.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrLyrics.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrLyrics.o: ../src/oah/oahElements.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrLyrics.o: ../src/lpsr/lpsrVariables.h ../src/oah/oahOah.h
../src/lpsr/lpsrLyrics.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrLyrics.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrLyrics.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrLyrics.o: ../src/oah/traceOah.h
../src/lpsr/lpsrOah.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/lpsr/lpsrOah.o: ../src/elements/exports.h
../src/lpsr/lpsrOah.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrOah.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrOah.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/lpsr/lpsrOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrOah.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/lpsr/lpsrOah.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrOah.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrOah.o: ../src/elements/elements.h
../src/lpsr/lpsrOah.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrOah.o: ../src/msr/msrBasicTypes.h ../src/oah/generalOah.h
../src/lpsr/lpsrOah.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrOah.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrOah.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrPapers.o: ../src/utilities/utilities.h
../src/lpsr/lpsrPapers.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/lpsr/lpsrPapers.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrPapers.o: ../src/lpsr/lpsrPapers.h ../src/msr/msrScaling.h
../src/lpsr/lpsrPapers.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrPapers.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrPapers.o: ../src/elements/elements.h
../src/lpsr/lpsrPapers.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrPapers.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrPapers.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrPapers.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrPapers.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrPapers.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrPapers.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrPapers.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrPapers.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrPapers.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrPapers.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrPapers.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrPapers.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrPapers.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrPapers.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/lpsr/lpsrPapers.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrPapers.o: ../src/msr/msrMidi.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrPapers.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrParallelMusic.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrPartGroups.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/typedefs.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/elements.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/types.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/exports.h
../src/lpsr/lpsrParallelMusic.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrParallelMusic.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrParallelMusic.o: ../src/visitors/visitable.h
../src/lpsr/lpsrParallelMusic.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrParallelMusic.o: ../src/visitors/visitor.h
../src/lpsr/lpsrParallelMusic.o: ../src/lib/tree_browser.h
../src/lpsr/lpsrParallelMusic.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msr.h
../src/lpsr/lpsrParallelMusic.o: ../src/utilities/utilities.h
../src/lpsr/lpsrParallelMusic.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrParallelMusic.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrScaling.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrCredits.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrStavesDetails.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrRehearsals.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTechnicals.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrBars.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrInstruments.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrLigatures.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSlashes.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrScores.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrVoiceElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrParallelMusic.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/oahElements.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrComments.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrVariables.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrBarNumbers.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrVoices.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrLyrics.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrStaves.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrContexts.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/oahOah.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrParallelMusic.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrParallelMusic.o: ../src/oah/traceOah.h
../src/lpsr/lpsrPartGroups.o: ../src/lpsr/lpsrPartGroups.h
../src/lpsr/lpsrPartGroups.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrElements.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/typedefs.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/elements.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/types.h
../src/lpsr/lpsrPartGroups.o: ../src/elements/exports.h ../src/elements/xml.h
../src/lpsr/lpsrPartGroups.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/lpsr/lpsrPartGroups.o: ../src/visitors/visitable.h
../src/lpsr/lpsrPartGroups.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrPartGroups.o: ../src/visitors/visitor.h
../src/lpsr/lpsrPartGroups.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrPartGroups.o: ../src/lib/rational.h ../src/msr/msr.h
../src/lpsr/lpsrPartGroups.o: ../src/utilities/utilities.h
../src/lpsr/lpsrPartGroups.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrPartGroups.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrStavesDetails.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrClefs.h ../src/msr/msrKeys.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTimes.h ../src/msr/msrTempos.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrRehearsals.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrInstruments.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTies.h ../src/msr/msrSlurs.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrLigatures.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrVoiceElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrPartGroups.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/oahElements.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/oahOah.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrPartGroups.o: ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrPartGroups.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrPartGroups.o: ../src/oah/traceOah.h
../src/lpsr/lpsrParts.o: ../src/lpsr/lpsrParts.h ../src/lpsr/lpsrContexts.h
../src/lpsr/lpsrParts.o: ../src/lpsr/lpsrElements.h ../src/msr/msrElements.h
../src/lpsr/lpsrParts.o: ../src/elements/typedefs.h
../src/lpsr/lpsrParts.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrParts.o: ../src/elements/elements.h
../src/lpsr/lpsrParts.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrParts.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrParts.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrParts.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrParts.o: ../src/visitors/visitable.h
../src/lpsr/lpsrParts.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrParts.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrParts.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrParts.o: ../src/msr/msr.h ../src/utilities/utilities.h
../src/lpsr/lpsrParts.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrParts.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrParts.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrParts.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrParts.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrParts.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrParts.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrParts.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrParts.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrParts.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrParts.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsrParts.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsrParts.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrParts.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrParts.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrParts.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrParts.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrParts.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrParts.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrParts.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrParts.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/lpsr/lpsrParts.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrParts.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrParts.o: ../src/oah/oahElements.h
../src/lpsr/lpsrParts.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrParts.o: ../src/lpsr/lpsrStaves.h
../src/lpsr/lpsrParts.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrParts.o: ../src/oah/traceOah.h ../src/oah/oahOah.h
../src/lpsr/lpsrParts.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrParts.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrScheme.o: ../src/utilities/utilities.h
../src/lpsr/lpsrScheme.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/lpsr/lpsrScheme.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrScheme.o: ../src/lpsr/lpsrScheme.h ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrScheme.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrScheme.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrScheme.o: ../src/elements/elements.h
../src/lpsr/lpsrScheme.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrScheme.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrScheme.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrScheme.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrScheme.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrScheme.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrScheme.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrScheme.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrScheme.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrScheme.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrScheme.o: ../src/oah/oahElements.h
../src/lpsr/lpsrScheme.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrScheme.o: ../src/msr/msrMidi.h ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrScheme.o: ../src/oah/oahOah.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrScores.h ../src/lpsr/lpsrComments.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrElements.h ../src/msr/msrElements.h
../src/lpsr/lpsrScores.o: ../src/elements/typedefs.h
../src/lpsr/lpsrScores.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrScores.o: ../src/elements/elements.h
../src/lpsr/lpsrScores.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrScores.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrScores.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrScores.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrScores.o: ../src/visitors/visitable.h
../src/lpsr/lpsrScores.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrScores.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrScores.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrScheme.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrScores.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrScores.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrScores.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrScores.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrScores.o: ../src/utilities/utilities.h
../src/lpsr/lpsrScores.o: ../src/oah/oahElements.h
../src/lpsr/lpsrScores.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrScores.o: ../src/msr/msrMidi.h ../src/lpsr/lpsrHeaders.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrScores.o: ../src/lilypond/lilypondOah.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrPapers.h ../src/msr/msrScaling.h
../src/lpsr/lpsrScores.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrScores.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrScores.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrScores.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrLayouts.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrParallelMusic.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrPartGroups.h ../src/msr/msr.h
../src/lpsr/lpsrScores.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrScores.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrScores.o: ../src/msr/msrCredits.h ../src/msr/msrDivisions.h
../src/lpsr/lpsrScores.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrScores.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsrScores.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsrScores.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrScores.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrScores.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrScores.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrScores.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrScores.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrScores.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrScores.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrScores.o: ../src/msr/msrVoiceElements.h
../src/lpsr/lpsrScores.o: ../src/oah/generalOah.h
../src/lpsr/lpsrScores.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrScores.o: ../src/lpsr/lpsrBookBlockElements.h
../src/lpsr/lpsrScores.o: ../src/utilities/version.h ../src/oah/oahOah.h
../src/lpsr/lpsrStaves.o: ../src/lpsr/lpsrStaves.h ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrStaves.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrStaves.o: ../src/elements/elements.h
../src/lpsr/lpsrStaves.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrStaves.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrStaves.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrStaves.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrStaves.o: ../src/visitors/visitable.h
../src/lpsr/lpsrStaves.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrStaves.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrStaves.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrStaves.o: ../src/msr/msr.h ../src/utilities/utilities.h
../src/lpsr/lpsrStaves.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrStaves.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrStaves.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/lpsr/lpsrStaves.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrStaves.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrStaves.o: ../src/oah/oahElements.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/msr/msrParts_MUT_DEP.h
../src/lpsr/lpsrStaves.o: ../src/lpsr/lpsrVoices.h ../src/lpsr/lpsrLyrics.h
../src/lpsr/lpsrStaves.o: ../src/lpsr/lpsrVariables.h ../src/oah/oahOah.h
../src/lpsr/lpsrStaves.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrStaves.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrStaves.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrStaves.o: ../src/oah/traceOah.h
../src/lpsr/lpsrVarValAssocs.o: ../src/utilities/utilities.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/exports.h
../src/lpsr/lpsrVarValAssocs.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lpsr/lpsrVarValAssocs.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrVarValAssocs.o: ../src/msr/msrElements.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/typedefs.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/elements.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrVarValAssocs.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lib/ctree.h
../src/lpsr/lpsrVarValAssocs.o: ../src/visitors/visitable.h
../src/lpsr/lpsrVarValAssocs.o: ../src/visitors/visitor.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lib/tree_browser.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrVarValAssocs.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/oahOah.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrVarValAssocs.o: ../src/msr/msrMidi.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrVarValAssocs.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/oahElements.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrVarValAssocs.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrVarValAssocs.o: ../src/oah/traceOah.h
../src/lpsr/lpsrVariables.o: ../src/utilities/utilities.h
../src/lpsr/lpsrVariables.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrVariables.o: ../src/elements/exports.h
../src/lpsr/lpsrVariables.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrVariables.o: ../src/lpsr/lpsrVariables.h
../src/lpsr/lpsrVariables.o: ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrVariables.o: ../src/msr/msrElements.h
../src/lpsr/lpsrVariables.o: ../src/elements/typedefs.h
../src/lpsr/lpsrVariables.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrVariables.o: ../src/elements/elements.h
../src/lpsr/lpsrVariables.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrVariables.o: ../src/elements/types.h ../src/elements/xml.h
../src/lpsr/lpsrVariables.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lpsr/lpsrVariables.o: ../src/visitors/visitor.h
../src/lpsr/lpsrVariables.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/lpsr/lpsrVariables.o: ../src/lib/rational.h ../src/oah/oahOah.h
../src/lpsr/lpsrVariables.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrVariables.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrVariables.o: ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrVariables.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrVariables.o: ../src/oah/oahElements.h ../src/lpsr/lpsrOah.h
../src/lpsr/lpsrVariables.o: ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrVariables.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrVariables.o: ../src/oah/traceOah.h
../src/lpsr/lpsrVoices.o: ../src/lpsr/lpsrVoices.h ../src/lpsr/lpsrElements.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/lpsr/lpsrVoices.o: ../src/elements/xml/xmlTypedefs.h
../src/lpsr/lpsrVoices.o: ../src/elements/elements.h
../src/lpsr/lpsrVoices.o: ../src/elements/xml/xmlElements.h
../src/lpsr/lpsrVoices.o: ../src/elements/types.h ../src/elements/exports.h
../src/lpsr/lpsrVoices.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/lpsr/lpsrVoices.o: ../src/lib/smartpointer.h
../src/lpsr/lpsrVoices.o: ../src/visitors/visitable.h
../src/lpsr/lpsrVoices.o: ../src/visitors/basevisitor.h
../src/lpsr/lpsrVoices.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/lpsr/lpsrVoices.o: ../src/lib/browser.h ../src/lib/rational.h
../src/lpsr/lpsrVoices.o: ../src/msr/msr.h ../src/utilities/utilities.h
../src/lpsr/lpsrVoices.o: ../src/utilities/messagesHandling.h
../src/lpsr/lpsrVoices.o: ../src/utilities/exceptions.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrBasicTypes.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrVarValAssocs.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrIdentification.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrMusicXMLSpecifics.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrMeasureElements.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrScaling.h ../src/msr/msrCredits.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrDivisions.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrPartNamesDisplays.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSegnosAndCodas.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrStavesDetails.h ../src/msr/msrClefs.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrKeys.h ../src/msr/msrTimes.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTempos.h ../src/msr/msrBeams.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrWords.h ../src/msr/msrRehearsals.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTranspositions.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrBreaks.h ../src/msr/msrStems.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTechnicals.h ../src/msr/msrBars.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrDynamics.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrArticulations.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTablatures.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrInstruments.h ../src/msr/msrTies.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSlurs.h ../src/msr/msrLigatures.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSingleTremolos.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSlashes.h ../src/msr/msrScores.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrPartGroups.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrPartGroupElements.h
../src/lpsr/lpsrVoices.o: ../src/msr/msr_MUTUAL_DEPENDENCIES.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTupletElements.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrVoiceElements.h ../src/msr/msrMidi.h
../src/lpsr/lpsrVoices.o: ../src/oah/generalOah.h ../src/oah/oahBasicTypes.h
../src/lpsr/lpsrVoices.o: ../src/interface/libmusicxml.h
../src/lpsr/lpsrVoices.o: ../src/oah/oahElements.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrOrnaments_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrDoubleTremolos_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSpanners_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrMeasures_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSegments_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrGraceNotes_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrAfterGraceNotes_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSyllables_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrHarmonies_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrFiguredBasses_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrGlissandos_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrSlides_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrNotes_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrChords_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrTuplets_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrStanzas_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrRepeats_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrMeasuresRepeats_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrRestMeasures_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrBeatsRepeats_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrRepeatCodas_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrVoices_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrStaves_MUT_DEP.h
../src/lpsr/lpsrVoices.o: ../src/msr/msrParts_MUT_DEP.h ../src/oah/oahOah.h
../src/lpsr/lpsrVoices.o: ../src/oah/oahAtomsCollection.h
../src/lpsr/lpsrVoices.o: ../src/lpsr/lpsrOah.h ../src/lpsr/lpsrBasicTypes.h
../src/lpsr/lpsrVoices.o: ../src/oah/enableTracingIfDesired.h
../src/lpsr/lpsrVoices.o: ../src/oah/traceOah.h
../src/lilypond/lilypondOah.o: ../src/utilities/version.h
../src/lilypond/lilypondOah.o: ../src/elements/exports.h
../src/lilypond/lilypondOah.o: ../src/utilities/utilities.h
../src/lilypond/lilypondOah.o: ../src/lib/smartpointer.h
../src/lilypond/lilypondOah.o: ../src/visitors/basevisitor.h
../src/lilypond/lilypondOah.o: ../src/oah/enableTracingIfDesired.h
../src/lilypond/lilypondOah.o: ../src/oah/traceOah.h
../src/lilypond/lilypondOah.o: ../src/oah/oahBasicTypes.h
../src/lilypond/lilypondOah.o: ../src/interface/libmusicxml.h
../src/lilypond/lilypondOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/lilypond/lilypondOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/lilypond/lilypondOah.o: ../src/utilities/messagesHandling.h
../src/lilypond/lilypondOah.o: ../src/utilities/exceptions.h
../src/lilypond/lilypondOah.o: ../src/oah/oahOah.h
../src/lilypond/lilypondOah.o: ../src/oah/oahAtomsCollection.h
../src/lilypond/lilypondOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/lilypond/lilypondOah.o: ../src/msr/msrElements.h
../src/lilypond/lilypondOah.o: ../src/elements/typedefs.h
../src/lilypond/lilypondOah.o: ../src/elements/xml/xmlTypedefs.h
../src/lilypond/lilypondOah.o: ../src/elements/elements.h
../src/lilypond/lilypondOah.o: ../src/elements/xml/xmlElements.h
../src/lilypond/lilypondOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/lilypond/lilypondOah.o: ../src/visitors/visitor.h
../src/lilypond/lilypondOah.o: ../src/lib/tree_browser.h
../src/lilypond/lilypondOah.o: ../src/msr/msrBasicTypes.h
../src/lilypond/lilypondOah.o: ../src/oah/generalOah.h
../src/lilypond/lilypondOah.o: ../src/lilypond/lilypondOah.h
../src/bsr/bsr.o: ../src/bsr/bsr.h ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsr.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsr.o: ../src/elements/typedefs.h
../src/bsr/bsr.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsr.o: ../src/elements/elements.h
../src/bsr/bsr.o: ../src/elements/xml/xmlElements.h ../src/elements/types.h
../src/bsr/bsr.o: ../src/elements/exports.h ../src/elements/xml.h
../src/bsr/bsr.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/bsr/bsr.o: ../src/visitors/visitable.h ../src/visitors/basevisitor.h
../src/bsr/bsr.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsr.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsr.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsr.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsr.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsr.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsr.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsr.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrClefs_MUT_DEP.h ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrWords_MUT_DEP.h ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsr.o: ../src/bsr/bsrPages_MUT_DEP.h ../src/bsr/bsrScores.h
../src/bsr/bsr.o: ../src/msr/msrScores.h ../src/msr/msrIdentification.h
../src/bsr/bsr.o: ../src/msr/msrVarValAssocs.h ../src/msr/msrScaling.h
../src/bsr/bsr.o: ../src/msr/msrMusicXMLSpecifics.h ../src/msr/msrCredits.h
../src/bsr/bsr.o: ../src/bsr/bsrTranscriptionNotes.h ../src/bsr/bsrOah.h
../src/bsr/bsr.o: ../src/oah/enableTracingIfDesired.h ../src/oah/traceOah.h
../src/bsr/bsr.o: ../src/oah/oahBasicTypes.h ../src/interface/libmusicxml.h
../src/bsr/bsr.o: ../src/utilities/utilities.h ../src/oah/oahElements.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrBarlines.o: ../src/elements/typedefs.h
../src/bsr/bsrBarlines.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrBarlines.o: ../src/elements/elements.h
../src/bsr/bsrBarlines.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrBarlines.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrBarlines.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrBarlines.o: ../src/lib/smartpointer.h
../src/bsr/bsrBarlines.o: ../src/visitors/visitable.h
../src/bsr/bsrBarlines.o: ../src/visitors/basevisitor.h
../src/bsr/bsrBarlines.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrBarlines.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrBarlines.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrBarlines.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrBarlines.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrBarlines.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrBarlines.o: ../src/msr/msrDynamics.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrBarlines.o: ../src/utilities/utilities.h
../src/bsr/bsrBarlines.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrBarlines.o: ../src/utilities/exceptions.h
../src/bsr/bsrBarlines.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrBarlines.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrBarlines.o: ../src/interface/libmusicxml.h
../src/bsr/bsrBarlines.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrBasicTypes.o: ../src/utilities/utilities.h
../src/bsr/bsrBasicTypes.o: ../src/lib/smartpointer.h
../src/bsr/bsrBasicTypes.o: ../src/elements/exports.h
../src/bsr/bsrBasicTypes.o: ../src/visitors/basevisitor.h
../src/bsr/bsrBasicTypes.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrBasicTypes.o: ../src/msr/msrBasicTypes.h ../src/lib/rational.h
../src/bsr/bsrBasicTypes.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrBasicTypes.o: ../src/utilities/exceptions.h
../src/bsr/bsrBasicTypes.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrBasicTypes.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrBasicTypes.o: ../src/interface/libmusicxml.h
../src/bsr/bsrBasicTypes.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/bsr/bsrBasicTypes.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrBasicTypes.o: ../src/oah/generalOah.h
../src/bsr/bsrBasicTypes.o: ../src/braille/brailleOah.h
../src/bsr/bsrCellsLists.o: ../src/utilities/utilities.h
../src/bsr/bsrCellsLists.o: ../src/lib/smartpointer.h
../src/bsr/bsrCellsLists.o: ../src/elements/exports.h
../src/bsr/bsrCellsLists.o: ../src/visitors/basevisitor.h
../src/bsr/bsrCellsLists.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrCellsLists.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrCellsLists.o: ../src/elements/typedefs.h
../src/bsr/bsrCellsLists.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrCellsLists.o: ../src/elements/elements.h
../src/bsr/bsrCellsLists.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrCellsLists.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrCellsLists.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrCellsLists.o: ../src/visitors/visitor.h
../src/bsr/bsrCellsLists.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrCellsLists.o: ../src/lib/rational.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrCellsLists.o: ../src/msr/msrBasicTypes.h ../src/oah/oahOah.h
../src/bsr/bsrCellsLists.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrCellsLists.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrCellsLists.o: ../src/interface/libmusicxml.h
../src/bsr/bsrCellsLists.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrCellsLists.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrCellsLists.o: ../src/oah/traceOah.h
../src/bsr/bsrClefs.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrClefs.o: ../src/elements/typedefs.h
../src/bsr/bsrClefs.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrClefs.o: ../src/elements/elements.h
../src/bsr/bsrClefs.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrClefs.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrClefs.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrClefs.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrClefs.o: ../src/visitors/basevisitor.h
../src/bsr/bsrClefs.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrClefs.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrClefs.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrClefs.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrClefs.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrClefs.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrClefs.o: ../src/utilities/utilities.h
../src/bsr/bsrClefs.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrClefs.o: ../src/utilities/exceptions.h
../src/bsr/bsrClefs.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrClefs.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrClefs.o: ../src/interface/libmusicxml.h
../src/bsr/bsrClefs.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrClefs.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrClefs.o: ../src/bsr/bsrOah.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrDynamics.o: ../src/elements/typedefs.h
../src/bsr/bsrDynamics.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrDynamics.o: ../src/elements/elements.h
../src/bsr/bsrDynamics.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrDynamics.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrDynamics.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrDynamics.o: ../src/lib/smartpointer.h
../src/bsr/bsrDynamics.o: ../src/visitors/visitable.h
../src/bsr/bsrDynamics.o: ../src/visitors/basevisitor.h
../src/bsr/bsrDynamics.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrDynamics.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrDynamics.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrDynamics.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrDynamics.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrDynamics.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrDynamics.o: ../src/msr/msrDynamics.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrDynamics.o: ../src/utilities/utilities.h
../src/bsr/bsrDynamics.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrDynamics.o: ../src/utilities/exceptions.h ../src/oah/oahOah.h
../src/bsr/bsrDynamics.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrDynamics.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrDynamics.o: ../src/interface/libmusicxml.h
../src/bsr/bsrDynamics.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrDynamics.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrDynamics.o: ../src/oah/traceOah.h
../src/bsr/bsrElements.o: ../src/utilities/utilities.h
../src/bsr/bsrElements.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/bsr/bsrElements.o: ../src/visitors/basevisitor.h
../src/bsr/bsrElements.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrElements.o: ../src/elements/typedefs.h
../src/bsr/bsrElements.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrElements.o: ../src/elements/elements.h
../src/bsr/bsrElements.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrElements.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrElements.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrElements.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/bsr/bsrElements.o: ../src/msr/msrMidi.h ../src/msr/msrBasicTypes.h
../src/bsr/bsrElements.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrElements.o: ../src/interface/libmusicxml.h
../src/bsr/bsrElements.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrElements.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrElements.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrElements.o: ../src/oah/traceOah.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrFootNotes.o: ../src/elements/typedefs.h
../src/bsr/bsrFootNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrFootNotes.o: ../src/elements/elements.h
../src/bsr/bsrFootNotes.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrFootNotes.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrFootNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrFootNotes.o: ../src/lib/smartpointer.h
../src/bsr/bsrFootNotes.o: ../src/visitors/visitable.h
../src/bsr/bsrFootNotes.o: ../src/visitors/basevisitor.h
../src/bsr/bsrFootNotes.o: ../src/visitors/visitor.h
../src/bsr/bsrFootNotes.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrFootNotes.o: ../src/lib/rational.h ../src/bsr/bsrCellsLists.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrDynamics.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrFootNotes.o: ../src/utilities/utilities.h ../src/oah/oahOah.h
../src/bsr/bsrFootNotes.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrFootNotes.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrFootNotes.o: ../src/interface/libmusicxml.h
../src/bsr/bsrFootNotes.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrFootNotes.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrFootNotes.o: ../src/oah/traceOah.h
../src/bsr/bsrKeys.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrKeys.o: ../src/elements/typedefs.h
../src/bsr/bsrKeys.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrKeys.o: ../src/elements/elements.h
../src/bsr/bsrKeys.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrKeys.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrKeys.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrKeys.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrKeys.o: ../src/visitors/basevisitor.h ../src/visitors/visitor.h
../src/bsr/bsrKeys.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrKeys.o: ../src/lib/rational.h ../src/bsr/bsrCellsLists.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrBasicTypes.h ../src/msr/msrBasicTypes.h
../src/bsr/bsrKeys.o: ../src/msr/msrTempos.h ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrKeys.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrKeys.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrKeys.o: ../src/utilities/utilities.h
../src/bsr/bsrKeys.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrKeys.o: ../src/utilities/exceptions.h
../src/bsr/bsrKeys.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrKeys.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrKeys.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/bsr/bsrKeys.o: ../src/oah/oahOah.h ../src/oah/oahAtomsCollection.h
../src/bsr/bsrKeys.o: ../src/msr/msrMidi.h ../src/oah/generalOah.h
../src/bsr/bsrKeys.o: ../src/bsr/bsrOah.h
../src/bsr/bsrLineElements.o: ../src/utilities/utilities.h
../src/bsr/bsrLineElements.o: ../src/lib/smartpointer.h
../src/bsr/bsrLineElements.o: ../src/elements/exports.h
../src/bsr/bsrLineElements.o: ../src/visitors/basevisitor.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrElements.h
../src/bsr/bsrLineElements.o: ../src/msr/msrElements.h
../src/bsr/bsrLineElements.o: ../src/elements/typedefs.h
../src/bsr/bsrLineElements.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrLineElements.o: ../src/elements/elements.h
../src/bsr/bsrLineElements.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrLineElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrLineElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrLineElements.o: ../src/visitors/visitor.h
../src/bsr/bsrLineElements.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrLineElements.o: ../src/lib/rational.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrLineElements.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrLineElements.o: ../src/msr/msrTempos.h
../src/bsr/bsrLineElements.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrLineElements.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrLineElements.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrLineElements.o: ../src/msr/msrDynamics.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrLineElements.o: ../src/oah/oahOah.h
../src/bsr/bsrLineElements.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrLineElements.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrLineElements.o: ../src/interface/libmusicxml.h
../src/bsr/bsrLineElements.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrLineElements.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrLineElements.o: ../src/oah/traceOah.h
../src/bsr/bsrLineElements.o: ../src/braille/brailleOah.h
../src/bsr/bsrLines.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/bsr/bsrLines.o: ../src/elements/exports.h
../src/bsr/bsrLines.o: ../src/visitors/basevisitor.h
../src/bsr/bsrLines.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrLines.o: ../src/utilities/exceptions.h
../src/bsr/bsrLines.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrLines.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrLines.o: ../src/elements/typedefs.h
../src/bsr/bsrLines.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrLines.o: ../src/elements/elements.h
../src/bsr/bsrLines.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrLines.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrLines.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrLines.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrLines.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrLines.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrLines.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrLines.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrLines.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrLines.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrLines.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrLines.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrLines.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrLines.o: ../src/interface/libmusicxml.h
../src/bsr/bsrLines.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrLines.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrLines.o: ../src/oah/generalOah.h ../src/bsr/bsrOah.h
../src/bsr/bsrLines.o: ../src/braille/brailleOah.h
../src/bsr/bsrMeasures.o: ../src/utilities/utilities.h
../src/bsr/bsrMeasures.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/bsr/bsrMeasures.o: ../src/visitors/basevisitor.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrMeasures.o: ../src/elements/typedefs.h
../src/bsr/bsrMeasures.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrMeasures.o: ../src/elements/elements.h
../src/bsr/bsrMeasures.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrMeasures.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrMeasures.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrMeasures.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrMeasures.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrMeasures.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrMeasures.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrMeasures.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrMeasures.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrMeasures.o: ../src/msr/msrDynamics.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrMeasures.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrMeasures.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrMeasures.o: ../src/interface/libmusicxml.h
../src/bsr/bsrMeasures.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrMeasures.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrMeasures.o: ../src/msr/msrMidi.h ../src/bsr/bsrOah.h
../src/bsr/bsrMeasures.o: ../src/braille/brailleOah.h
../src/bsr/bsrMusicHeadings.o: ../src/utilities/utilities.h
../src/bsr/bsrMusicHeadings.o: ../src/lib/smartpointer.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/exports.h
../src/bsr/bsrMusicHeadings.o: ../src/visitors/basevisitor.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrElements.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrElements.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/typedefs.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/elements.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrMusicHeadings.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrMusicHeadings.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrMusicHeadings.o: ../src/visitors/visitor.h
../src/bsr/bsrMusicHeadings.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrMusicHeadings.o: ../src/lib/rational.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrTempos.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrDynamics.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/oahOah.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrMusicHeadings.o: ../src/msr/msrMidi.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrMusicHeadings.o: ../src/interface/libmusicxml.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrMusicHeadings.o: ../src/oah/traceOah.h
../src/bsr/bsrNotes.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrNotes.o: ../src/elements/typedefs.h
../src/bsr/bsrNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrNotes.o: ../src/elements/elements.h
../src/bsr/bsrNotes.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrNotes.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrNotes.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrNotes.o: ../src/visitors/basevisitor.h
../src/bsr/bsrNotes.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrNotes.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrNotes.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrNotes.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrNotes.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrNotes.o: ../src/utilities/utilities.h
../src/bsr/bsrNotes.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrNotes.o: ../src/utilities/exceptions.h ../src/oah/oahOah.h
../src/bsr/bsrNotes.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrNotes.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrNotes.o: ../src/interface/libmusicxml.h
../src/bsr/bsrNotes.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrNotes.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrNotes.o: ../src/oah/traceOah.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrNumbers.o: ../src/elements/typedefs.h
../src/bsr/bsrNumbers.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrNumbers.o: ../src/elements/elements.h
../src/bsr/bsrNumbers.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrNumbers.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrNumbers.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrNumbers.o: ../src/lib/smartpointer.h
../src/bsr/bsrNumbers.o: ../src/visitors/visitable.h
../src/bsr/bsrNumbers.o: ../src/visitors/basevisitor.h
../src/bsr/bsrNumbers.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrNumbers.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrNumbers.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrNumbers.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrNumbers.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrNumbers.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrNumbers.o: ../src/msr/msrDynamics.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrNumbers.o: ../src/utilities/utilities.h
../src/bsr/bsrNumbers.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrNumbers.o: ../src/utilities/exceptions.h ../src/oah/oahOah.h
../src/bsr/bsrNumbers.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrNumbers.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrNumbers.o: ../src/interface/libmusicxml.h
../src/bsr/bsrNumbers.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrNumbers.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrNumbers.o: ../src/oah/traceOah.h
../src/bsr/bsrOah.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/bsr/bsrOah.o: ../src/elements/exports.h ../src/visitors/basevisitor.h
../src/bsr/bsrOah.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrOah.o: ../src/interface/libmusicxml.h ../src/oah/oahElements.h
../src/bsr/bsrOah.o: ../src/lib/browser.h ../src/lib/ctree.h
../src/bsr/bsrOah.o: ../src/visitors/visitable.h ../src/oah/oahOah.h
../src/bsr/bsrOah.o: ../src/oah/oahAtomsCollection.h ../src/lib/rational.h
../src/bsr/bsrOah.o: ../src/msr/msrMidi.h ../src/msr/msrElements.h
../src/bsr/bsrOah.o: ../src/elements/typedefs.h
../src/bsr/bsrOah.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrOah.o: ../src/elements/elements.h
../src/bsr/bsrOah.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrOah.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrOah.o: ../src/msr/msrBasicTypes.h ../src/oah/generalOah.h
../src/bsr/bsrOah.o: ../src/bsr/bsrOah.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrOah.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrOah.o: ../src/utilities/exceptions.h
../src/bsr/bsrPageElements.o: ../src/utilities/utilities.h
../src/bsr/bsrPageElements.o: ../src/lib/smartpointer.h
../src/bsr/bsrPageElements.o: ../src/elements/exports.h
../src/bsr/bsrPageElements.o: ../src/visitors/basevisitor.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrElements.h
../src/bsr/bsrPageElements.o: ../src/msr/msrElements.h
../src/bsr/bsrPageElements.o: ../src/elements/typedefs.h
../src/bsr/bsrPageElements.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrPageElements.o: ../src/elements/elements.h
../src/bsr/bsrPageElements.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrPageElements.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrPageElements.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrPageElements.o: ../src/visitors/visitor.h
../src/bsr/bsrPageElements.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrPageElements.o: ../src/lib/rational.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrPageElements.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrPageElements.o: ../src/msr/msrTempos.h
../src/bsr/bsrPageElements.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrPageElements.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrPageElements.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrPageElements.o: ../src/msr/msrDynamics.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrPageElements.o: ../src/oah/oahOah.h
../src/bsr/bsrPageElements.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrPageElements.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrPageElements.o: ../src/interface/libmusicxml.h
../src/bsr/bsrPageElements.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrPageElements.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrPageElements.o: ../src/oah/traceOah.h
../src/bsr/bsrPageElements.o: ../src/braille/brailleOah.h
../src/bsr/bsrPageHeadings.o: ../src/utilities/utilities.h
../src/bsr/bsrPageHeadings.o: ../src/lib/smartpointer.h
../src/bsr/bsrPageHeadings.o: ../src/elements/exports.h
../src/bsr/bsrPageHeadings.o: ../src/visitors/basevisitor.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrElements.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrElements.h
../src/bsr/bsrPageHeadings.o: ../src/elements/typedefs.h
../src/bsr/bsrPageHeadings.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrPageHeadings.o: ../src/elements/elements.h
../src/bsr/bsrPageHeadings.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrPageHeadings.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrPageHeadings.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrPageHeadings.o: ../src/visitors/visitor.h
../src/bsr/bsrPageHeadings.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrPageHeadings.o: ../src/lib/rational.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrTempos.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrDynamics.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrPageHeadings.o: ../src/oah/oahOah.h
../src/bsr/bsrPageHeadings.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrPageHeadings.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrPageHeadings.o: ../src/interface/libmusicxml.h
../src/bsr/bsrPageHeadings.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrPageHeadings.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrPageHeadings.o: ../src/oah/traceOah.h
../src/bsr/bsrPages.o: ../src/utilities/utilities.h ../src/lib/smartpointer.h
../src/bsr/bsrPages.o: ../src/elements/exports.h
../src/bsr/bsrPages.o: ../src/visitors/basevisitor.h
../src/bsr/bsrPages.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrPages.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrPages.o: ../src/elements/typedefs.h
../src/bsr/bsrPages.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrPages.o: ../src/elements/elements.h
../src/bsr/bsrPages.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrPages.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrPages.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrPages.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrPages.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrPages.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrPages.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrPages.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrPages.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrPages.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrPages.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrPages.o: ../src/bsr/bsrPages_MUT_DEP.h ../src/oah/oahOah.h
../src/bsr/bsrPages.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrPages.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrPages.o: ../src/interface/libmusicxml.h
../src/bsr/bsrPages.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrPages.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrPages.o: ../src/oah/traceOah.h ../src/braille/brailleOah.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrElements.h
../src/bsr/bsrPaginations.o: ../src/msr/msrElements.h
../src/bsr/bsrPaginations.o: ../src/elements/typedefs.h
../src/bsr/bsrPaginations.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrPaginations.o: ../src/elements/elements.h
../src/bsr/bsrPaginations.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrPaginations.o: ../src/elements/types.h
../src/bsr/bsrPaginations.o: ../src/elements/exports.h ../src/elements/xml.h
../src/bsr/bsrPaginations.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/bsr/bsrPaginations.o: ../src/visitors/visitable.h
../src/bsr/bsrPaginations.o: ../src/visitors/basevisitor.h
../src/bsr/bsrPaginations.o: ../src/visitors/visitor.h
../src/bsr/bsrPaginations.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrPaginations.o: ../src/lib/rational.h ../src/bsr/bsrCellsLists.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrPaginations.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrPaginations.o: ../src/msr/msrTempos.h
../src/bsr/bsrPaginations.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrPaginations.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrPaginations.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrPaginations.o: ../src/msr/msrDynamics.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrPaginations.o: ../src/utilities/utilities.h
../src/bsr/bsrPaginations.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrPaginations.o: ../src/utilities/exceptions.h
../src/bsr/bsrPaginations.o: ../src/oah/oahOah.h
../src/bsr/bsrPaginations.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrPaginations.o: ../src/msr/msrMidi.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrPaginations.o: ../src/interface/libmusicxml.h
../src/bsr/bsrPaginations.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrPaginations.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrPaginations.o: ../src/oah/traceOah.h
../src/bsr/bsrParallels.o: ../src/utilities/utilities.h
../src/bsr/bsrParallels.o: ../src/lib/smartpointer.h
../src/bsr/bsrParallels.o: ../src/elements/exports.h
../src/bsr/bsrParallels.o: ../src/visitors/basevisitor.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrParallels.h
../src/bsr/bsrParallels.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrParallels.o: ../src/elements/typedefs.h
../src/bsr/bsrParallels.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrParallels.o: ../src/elements/elements.h
../src/bsr/bsrParallels.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrParallels.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrParallels.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrParallels.o: ../src/visitors/visitor.h
../src/bsr/bsrParallels.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/bsr/bsrParallels.o: ../src/lib/rational.h ../src/bsr/bsrCellsLists.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrParallels.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrParallels.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrParallels.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrParallels.o: ../src/msr/msrBeams.h ../src/msr/msrWords.h
../src/bsr/bsrParallels.o: ../src/msr/msrDynamics.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrParallels.o: ../src/bsr/bsrPages_MUT_DEP.h ../src/bsr/bsrOah.h
../src/bsr/bsrParallels.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrParallels.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrParallels.o: ../src/interface/libmusicxml.h
../src/bsr/bsrParallels.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrParallels.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrParallels.o: ../src/msr/msrMidi.h ../src/braille/brailleOah.h
../src/bsr/bsrScores.o: ../src/utilities/utilities.h
../src/bsr/bsrScores.o: ../src/lib/smartpointer.h ../src/elements/exports.h
../src/bsr/bsrScores.o: ../src/visitors/basevisitor.h
../src/bsr/bsrScores.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrScores.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrScores.o: ../src/elements/typedefs.h
../src/bsr/bsrScores.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrScores.o: ../src/elements/elements.h
../src/bsr/bsrScores.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrScores.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrScores.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrScores.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrScores.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrScores.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrScores.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrScores.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrScores.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrScores.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrScores.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrScores.o: ../src/bsr/bsrPages_MUT_DEP.h ../src/bsr/bsrScores.h
../src/bsr/bsrScores.o: ../src/msr/msrScores.h ../src/msr/msrIdentification.h
../src/bsr/bsrScores.o: ../src/msr/msrVarValAssocs.h ../src/msr/msrScaling.h
../src/bsr/bsrScores.o: ../src/msr/msrMusicXMLSpecifics.h
../src/bsr/bsrScores.o: ../src/msr/msrCredits.h
../src/bsr/bsrScores.o: ../src/bsr/bsrTranscriptionNotes.h
../src/bsr/bsrScores.o: ../src/bsr/bsrOah.h
../src/bsr/bsrScores.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrScores.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrScores.o: ../src/interface/libmusicxml.h
../src/bsr/bsrScores.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrScores.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrSpaces.o: ../src/elements/typedefs.h
../src/bsr/bsrSpaces.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrSpaces.o: ../src/elements/elements.h
../src/bsr/bsrSpaces.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrSpaces.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrSpaces.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrSpaces.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrSpaces.o: ../src/visitors/basevisitor.h
../src/bsr/bsrSpaces.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrSpaces.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrSpaces.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrSpaces.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrSpaces.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrSpaces.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrSpaces.o: ../src/utilities/utilities.h
../src/bsr/bsrSpaces.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrSpaces.o: ../src/utilities/exceptions.h ../src/oah/oahOah.h
../src/bsr/bsrSpaces.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrSpaces.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrSpaces.o: ../src/interface/libmusicxml.h
../src/bsr/bsrSpaces.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrSpaces.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrSpaces.o: ../src/oah/traceOah.h
../src/bsr/bsrTempos.o: ../src/msr/msrBasicTypes.h ../src/lib/smartpointer.h
../src/bsr/bsrTempos.o: ../src/elements/exports.h ../src/lib/rational.h
../src/bsr/bsrTempos.o: ../src/msr/msrTempos.h ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrTempos.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrTempos.o: ../src/msr/msrElements.h ../src/elements/typedefs.h
../src/bsr/bsrTempos.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrTempos.o: ../src/elements/elements.h
../src/bsr/bsrTempos.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrTempos.o: ../src/elements/types.h ../src/elements/xml.h
../src/bsr/bsrTempos.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/bsr/bsrTempos.o: ../src/visitors/basevisitor.h
../src/bsr/bsrTempos.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrTempos.o: ../src/lib/browser.h ../src/msr/msrBeams.h
../src/bsr/bsrTempos.o: ../src/msr/msrWords.h
../src/bsr/bsrTempos.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrElements.h ../src/bsr/bsrCellsLists.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrBasicTypes.h ../src/msr/msrDynamics.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrTempos.o: ../src/utilities/utilities.h
../src/bsr/bsrTempos.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrTempos.o: ../src/utilities/exceptions.h
../src/bsr/bsrTempos.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrTempos.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrTempos.o: ../src/interface/libmusicxml.h
../src/bsr/bsrTempos.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrTempos.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrTempos.o: ../src/oah/generalOah.h ../src/bsr/bsrOah.h
../src/bsr/bsrTimes.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrTimes.o: ../src/elements/typedefs.h
../src/bsr/bsrTimes.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrTimes.o: ../src/elements/elements.h
../src/bsr/bsrTimes.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrTimes.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrTimes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrTimes.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrTimes.o: ../src/visitors/basevisitor.h
../src/bsr/bsrTimes.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrTimes.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrTimes.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrTimes.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrTimes.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrTimes.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrTimes.o: ../src/utilities/utilities.h
../src/bsr/bsrTimes.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrTimes.o: ../src/utilities/exceptions.h
../src/bsr/bsrTimes.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrTimes.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/bsr/bsrTimes.o: ../src/interface/libmusicxml.h
../src/bsr/bsrTimes.o: ../src/oah/oahElements.h ../src/oah/oahOah.h
../src/bsr/bsrTimes.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrTimes.o: ../src/oah/generalOah.h ../src/bsr/bsrOah.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrTranscriptionNotes.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrElements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrElements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/typedefs.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/elements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/types.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/exports.h
../src/bsr/bsrTranscriptionNotes.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrTranscriptionNotes.o: ../src/lib/smartpointer.h
../src/bsr/bsrTranscriptionNotes.o: ../src/visitors/visitable.h
../src/bsr/bsrTranscriptionNotes.o: ../src/visitors/basevisitor.h
../src/bsr/bsrTranscriptionNotes.o: ../src/visitors/visitor.h
../src/bsr/bsrTranscriptionNotes.o: ../src/lib/tree_browser.h
../src/bsr/bsrTranscriptionNotes.o: ../src/lib/browser.h
../src/bsr/bsrTranscriptionNotes.o: ../src/lib/rational.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrCellsLists.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrBasicTypes.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrTempos.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrMeasureElements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrBeams.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrWords.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrDynamics.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrTranscriptionNotes.o: ../src/utilities/utilities.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/oahOah.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/oahAtomsCollection.h
../src/bsr/bsrTranscriptionNotes.o: ../src/msr/msrMidi.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrTranscriptionNotes.o: ../src/interface/libmusicxml.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/oahElements.h
../src/bsr/bsrTranscriptionNotes.o: ../src/bsr/bsrOah.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrTranscriptionNotes.o: ../src/oah/traceOah.h
../src/bsr/bsrWords.o: ../src/bsr/bsr_MUTUAL_DEPENDENCIES.h
../src/bsr/bsrWords.o: ../src/bsr/bsrElements.h ../src/msr/msrElements.h
../src/bsr/bsrWords.o: ../src/elements/typedefs.h
../src/bsr/bsrWords.o: ../src/elements/xml/xmlTypedefs.h
../src/bsr/bsrWords.o: ../src/elements/elements.h
../src/bsr/bsrWords.o: ../src/elements/xml/xmlElements.h
../src/bsr/bsrWords.o: ../src/elements/types.h ../src/elements/exports.h
../src/bsr/bsrWords.o: ../src/elements/xml.h ../src/lib/ctree.h
../src/bsr/bsrWords.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
../src/bsr/bsrWords.o: ../src/visitors/basevisitor.h
../src/bsr/bsrWords.o: ../src/visitors/visitor.h ../src/lib/tree_browser.h
../src/bsr/bsrWords.o: ../src/lib/browser.h ../src/lib/rational.h
../src/bsr/bsrWords.o: ../src/bsr/bsrCellsLists.h ../src/bsr/bsrBasicTypes.h
../src/bsr/bsrWords.o: ../src/msr/msrBasicTypes.h ../src/msr/msrTempos.h
../src/bsr/bsrWords.o: ../src/msr/msrSegnosAndCodas.h
../src/bsr/bsrWords.o: ../src/msr/msrMeasureElements.h ../src/msr/msrBeams.h
../src/bsr/bsrWords.o: ../src/msr/msrWords.h ../src/msr/msrDynamics.h
../src/bsr/bsrWords.o: ../src/bsr/bsrLineElements_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrPageElements_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrLines_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrSpaces_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrPaginations_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrClefs_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrKeys_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrTimes_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrBarlines_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrNumbers_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrWords_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrNotes_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrDynamics_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrMeasures_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrTempos_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrPageHeadings_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrMusicHeadings_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrFootNotes_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/bsr/bsrPages_MUT_DEP.h
../src/bsr/bsrWords.o: ../src/utilities/utilities.h
../src/bsr/bsrWords.o: ../src/utilities/messagesHandling.h
../src/bsr/bsrWords.o: ../src/utilities/exceptions.h ../src/oah/oahOah.h
../src/bsr/bsrWords.o: ../src/oah/oahAtomsCollection.h ../src/msr/msrMidi.h
../src/bsr/bsrWords.o: ../src/oah/oahBasicTypes.h
../src/bsr/bsrWords.o: ../src/interface/libmusicxml.h
../src/bsr/bsrWords.o: ../src/oah/oahElements.h ../src/bsr/bsrOah.h
../src/bsr/bsrWords.o: ../src/oah/enableTracingIfDesired.h
../src/bsr/bsrWords.o: ../src/oah/traceOah.h
../src/braille/brailleOah.o: ../src/utilities/utilities.h
../src/braille/brailleOah.o: ../src/lib/smartpointer.h
../src/braille/brailleOah.o: ../src/elements/exports.h
../src/braille/brailleOah.o: ../src/visitors/basevisitor.h
../src/braille/brailleOah.o: ../src/oah/enableTracingIfDesired.h
../src/braille/brailleOah.o: ../src/oah/traceOah.h ../src/oah/oahBasicTypes.h
../src/braille/brailleOah.o: ../src/interface/libmusicxml.h
../src/braille/brailleOah.o: ../src/oah/oahElements.h ../src/lib/browser.h
../src/braille/brailleOah.o: ../src/lib/ctree.h ../src/visitors/visitable.h
../src/braille/brailleOah.o: ../src/oah/oahOah.h
../src/braille/brailleOah.o: ../src/oah/oahAtomsCollection.h
../src/braille/brailleOah.o: ../src/lib/rational.h ../src/msr/msrMidi.h
../src/braille/brailleOah.o: ../src/msr/msrElements.h
../src/braille/brailleOah.o: ../src/elements/typedefs.h
../src/braille/brailleOah.o: ../src/elements/xml/xmlTypedefs.h
../src/braille/brailleOah.o: ../src/elements/elements.h
../src/braille/brailleOah.o: ../src/elements/xml/xmlElements.h
../src/braille/brailleOah.o: ../src/elements/types.h ../src/elements/xml.h
../src/braille/brailleOah.o: ../src/visitors/visitor.h
../src/braille/brailleOah.o: ../src/lib/tree_browser.h
../src/braille/brailleOah.o: ../src/msr/msrBasicTypes.h
../src/braille/brailleOah.o: ../src/oah/generalOah.h
../src/braille/brailleOah.o: ../src/braille/brailleOah.h
../src/braille/brailleOah.o: ../src/bsr/bsrBasicTypes.h
