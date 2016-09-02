subprojects := ../src/elements ../src/guido ../src/interface ../src/files ../src/lib ../src/parser ../src/operations ../src/visitors

SRC = $(wildcard ../src/*.cpp) $(wildcard ../src/*/*.cpp)
APPSRC = $(wildcard ../samples/*.cpp) 
OBJ = $(SRC:.cpp=.o)

VPATH = $(subprojects)

CXXFLAGS := -g -O3 -Wall -Wuninitialized $(addprefix -I, $(subprojects))

lib := ../libmusicxml2.a

all : $(lib) samples

samples : 
	make -C ../samples

$(lib): $(OBJ)
	ar cru $(lib) $(OBJ) 
	ranlib $(lib)

clean :
	rm -f $(lib) $(OBJ)

dep:
	makedepend -- $(CXXFLAGS) -- $(SRC)


# DO NOT DELETE

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
../src/elements/xml.o: /usr/include/stdlib.h /usr/include/available.h
../src/elements/xml.o: /usr/include/_types.h /usr/include/sys/_types.h
../src/elements/xml.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
../src/elements/xml.o: /usr/include/ppc/_types.h /usr/include/sys/wait.h
../src/elements/xml.o: /usr/include/sys/signal.h
../src/elements/xml.o: /usr/include/sys/appleapiopts.h
../src/elements/xml.o: /usr/include/machine/signal.h
../src/elements/xml.o: /usr/include/ppc/signal.h /usr/include/ppc/_structs.h
../src/elements/xml.o: /usr/include/sys/_structs.h
../src/elements/xml.o: /usr/include/machine/_structs.h
../src/elements/xml.o: /usr/include/sys/resource.h
../src/elements/xml.o: /usr/include/machine/endian.h
../src/elements/xml.o: /usr/include/ppc/endian.h /usr/include/sys/_endian.h
../src/elements/xml.o: /usr/include/alloca.h /usr/include/machine/types.h
../src/elements/xml.o: /usr/include/ppc/types.h ../src/elements/xml.h
../src/elements/xml.o: ../src/elements/exports.h ../src/lib/ctree.h
../src/elements/xml.o: ../src/lib/smartpointer.h ../src/visitors/visitable.h
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
../src/files/xmlreader.o: ../src/files/xmlreader.h /usr/include/stdio.h
../src/files/xmlreader.o: /usr/include/_types.h /usr/include/sys/_types.h
../src/files/xmlreader.o: /usr/include/sys/cdefs.h
../src/files/xmlreader.o: /usr/include/machine/_types.h
../src/files/xmlreader.o: /usr/include/ppc/_types.h ../src/elements/exports.h
../src/files/xmlreader.o: ../src/files/xmlfile.h ../src/elements/xml.h
../src/files/xmlreader.o: ../src/lib/ctree.h ../src/lib/smartpointer.h
../src/files/xmlreader.o: ../src/visitors/visitable.h
../src/files/xmlreader.o: ../src/visitors/basevisitor.h
../src/files/xmlreader.o: ../src/parser/reader.h ../src/elements/factory.h
../src/files/xmlreader.o: ../src/lib/functor.h ../src/lib/singleton.h
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
../src/guido/xmlpart2guido.o: ../src/guido/xmlpart2guido.h
../src/guido/xmlpart2guido.o: ../src/visitors/clefvisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/keysignvisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/metronomevisitor.h
../src/guido/xmlpart2guido.o: ../src/visitors/timesignvisitor.h
../src/guido/xmlpart2guido.o: ../src/elements/xml_tree_browser.h
../src/guido/xmlpart2guido.o: ../src/lib/tree_browser.h ../src/lib/browser.h
../src/interface/libmusicxml.o: ../src/interface/libmusicxml.h
../src/interface/libmusicxml.o: /usr/include/stdio.h /usr/include/_types.h
../src/interface/libmusicxml.o: /usr/include/sys/_types.h
../src/interface/libmusicxml.o: /usr/include/sys/cdefs.h
../src/interface/libmusicxml.o: /usr/include/machine/_types.h
../src/interface/libmusicxml.o: /usr/include/ppc/_types.h
../src/interface/libmusicxml.o: ../src/elements/exports.h
../src/interface/libmusicxml.o: ../src/elements/versions.h
../src/interface/musicxml2guido.o: ../src/interface/libmusicxml.h
../src/interface/musicxml2guido.o: /usr/include/stdio.h /usr/include/_types.h
../src/interface/musicxml2guido.o: /usr/include/sys/_types.h
../src/interface/musicxml2guido.o: /usr/include/sys/cdefs.h
../src/interface/musicxml2guido.o: /usr/include/machine/_types.h
../src/interface/musicxml2guido.o: /usr/include/ppc/_types.h
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
../src/lib/conversions.o: ../src/lib/conversions.h ../src/elements/exports.h
../src/lib/conversions.o: ../src/lib/rational.h ../src/lib/bimap.h
../src/lib/rational.o: ../src/lib/rational.h ../src/elements/exports.h
../src/lib/rational.o: /usr/include/stdlib.h /usr/include/available.h
../src/lib/rational.o: /usr/include/_types.h /usr/include/sys/_types.h
../src/lib/rational.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
../src/lib/rational.o: /usr/include/ppc/_types.h /usr/include/sys/wait.h
../src/lib/rational.o: /usr/include/sys/signal.h
../src/lib/rational.o: /usr/include/sys/appleapiopts.h
../src/lib/rational.o: /usr/include/machine/signal.h
../src/lib/rational.o: /usr/include/ppc/signal.h /usr/include/ppc/_structs.h
../src/lib/rational.o: /usr/include/sys/_structs.h
../src/lib/rational.o: /usr/include/machine/_structs.h
../src/lib/rational.o: /usr/include/sys/resource.h
../src/lib/rational.o: /usr/include/machine/endian.h
../src/lib/rational.o: /usr/include/ppc/endian.h /usr/include/sys/_endian.h
../src/lib/rational.o: /usr/include/alloca.h /usr/include/machine/types.h
../src/lib/rational.o: /usr/include/ppc/types.h /usr/include/string.h
../src/operations/transposition.o: /usr/include/math.h
../src/operations/transposition.o: /usr/include/architecture/ppc/math.h
../src/operations/transposition.o: /usr/include/sys/cdefs.h
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
../src/parser/xmlparse.o: /usr/include/stdio.h /usr/include/_types.h
../src/parser/xmlparse.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../src/parser/xmlparse.o: /usr/include/machine/_types.h
../src/parser/xmlparse.o: /usr/include/ppc/_types.h /usr/include/string.h
../src/parser/xmlparse.o: ../src/parser/reader.h ../src/parser/xmllex.c++
../src/parser/xmlparse.o: /usr/include/errno.h /usr/include/sys/errno.h
../src/parser/xmlparse.o: /usr/include/stdlib.h /usr/include/available.h
../src/parser/xmlparse.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
../src/parser/xmlparse.o: /usr/include/sys/appleapiopts.h
../src/parser/xmlparse.o: /usr/include/machine/signal.h
../src/parser/xmlparse.o: /usr/include/ppc/signal.h
../src/parser/xmlparse.o: /usr/include/ppc/_structs.h
../src/parser/xmlparse.o: /usr/include/sys/_structs.h
../src/parser/xmlparse.o: /usr/include/machine/_structs.h
../src/parser/xmlparse.o: /usr/include/sys/resource.h
../src/parser/xmlparse.o: /usr/include/machine/endian.h
../src/parser/xmlparse.o: /usr/include/ppc/endian.h
../src/parser/xmlparse.o: /usr/include/sys/_endian.h /usr/include/alloca.h
../src/parser/xmlparse.o: /usr/include/machine/types.h
../src/parser/xmlparse.o: /usr/include/ppc/types.h ../src/parser/xmlparse.hpp
../src/parser/xmlparse.o: /usr/include/stddef.h
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
../src/visitors/midicontextvisitor.o: /usr/include/stdlib.h
../src/visitors/midicontextvisitor.o: /usr/include/available.h
../src/visitors/midicontextvisitor.o: /usr/include/_types.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/_types.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/cdefs.h
../src/visitors/midicontextvisitor.o: /usr/include/machine/_types.h
../src/visitors/midicontextvisitor.o: /usr/include/ppc/_types.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/wait.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/signal.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/appleapiopts.h
../src/visitors/midicontextvisitor.o: /usr/include/machine/signal.h
../src/visitors/midicontextvisitor.o: /usr/include/ppc/signal.h
../src/visitors/midicontextvisitor.o: /usr/include/ppc/_structs.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/_structs.h
../src/visitors/midicontextvisitor.o: /usr/include/machine/_structs.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/resource.h
../src/visitors/midicontextvisitor.o: /usr/include/machine/endian.h
../src/visitors/midicontextvisitor.o: /usr/include/ppc/endian.h
../src/visitors/midicontextvisitor.o: /usr/include/sys/_endian.h
../src/visitors/midicontextvisitor.o: /usr/include/alloca.h
../src/visitors/midicontextvisitor.o: /usr/include/machine/types.h
../src/visitors/midicontextvisitor.o: /usr/include/ppc/types.h
../src/visitors/midicontextvisitor.o: /usr/include/math.h
../src/visitors/midicontextvisitor.o: /usr/include/architecture/ppc/math.h
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
../src/visitors/timesignvisitor.o: /usr/include/stdlib.h
../src/visitors/timesignvisitor.o: /usr/include/available.h
../src/visitors/timesignvisitor.o: /usr/include/_types.h
../src/visitors/timesignvisitor.o: /usr/include/sys/_types.h
../src/visitors/timesignvisitor.o: /usr/include/sys/cdefs.h
../src/visitors/timesignvisitor.o: /usr/include/machine/_types.h
../src/visitors/timesignvisitor.o: /usr/include/ppc/_types.h
../src/visitors/timesignvisitor.o: /usr/include/sys/wait.h
../src/visitors/timesignvisitor.o: /usr/include/sys/signal.h
../src/visitors/timesignvisitor.o: /usr/include/sys/appleapiopts.h
../src/visitors/timesignvisitor.o: /usr/include/machine/signal.h
../src/visitors/timesignvisitor.o: /usr/include/ppc/signal.h
../src/visitors/timesignvisitor.o: /usr/include/ppc/_structs.h
../src/visitors/timesignvisitor.o: /usr/include/sys/_structs.h
../src/visitors/timesignvisitor.o: /usr/include/machine/_structs.h
../src/visitors/timesignvisitor.o: /usr/include/sys/resource.h
../src/visitors/timesignvisitor.o: /usr/include/machine/endian.h
../src/visitors/timesignvisitor.o: /usr/include/ppc/endian.h
../src/visitors/timesignvisitor.o: /usr/include/sys/_endian.h
../src/visitors/timesignvisitor.o: /usr/include/alloca.h
../src/visitors/timesignvisitor.o: /usr/include/machine/types.h
../src/visitors/timesignvisitor.o: /usr/include/ppc/types.h
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
