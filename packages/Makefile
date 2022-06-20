
# current dtds and schema version
CURRENT = 3.1

VERSION  := 3.22
TAG      :=-D tomorrow
ROOTDIR  := ..
SRCDIR   := $(ROOTDIR)/src
BUILDDIR := $(ROOTDIR)/build
DEST     := .

SRC 	 := libmusicxml-$(VERSION)-src
MACOSLIB := libmusicxml-$(VERSION)-macosx
MACOSDIR := $(MACOSLIB)
LINUXLIB := libmusicxml-$(VERSION)-ubuntu-x86_64
LINUXDIR := $(LINUXLIB)
WINDIR32 := libmusicxml-$(VERSION)-win32
WINDIR64 := libmusicxml-$(VERSION)-win64
JSDIR    := libmusicxml-javascript-$(VERSION)
JSSRC 	 := $(ROOTDIR)/javascript

HEADERS  = $(wildcard .$(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*/*.h)
FILES    = $(ROOTDIR)/CHANGELOG.txt ./README.md $(ROOTDIR)/license.txt
TOOLS    = xml2guido xml2ly xmltranspose xmlversion
WINTOOLS32 = $(TOOLS:%=$(BUILDDIR)/win32/Release/%.exe)
WINTOOLS64 = $(TOOLS:%=$(BUILDDIR)/win64/Release/%.exe)
MACTOOLS = $(TOOLS:%=$(BUILDDIR)/libdir/Release/%)
LINUXTOOLS = $(TOOLS:%=$(BUILDDIR)/%)
DTDS     = $(wildcard $(ROOTDIR)/dtds/$(CURRENT)/*.*)
SCHEMA   = $(wildcard $(ROOTDIR)/schema/$(CURRENT)/*.xsd)
SAMPLES  = $(wildcard $(ROOTDIR)/samples/*.cpp)
DOC      = $(ROOTDIR)/doc/presentation/libmusicxml2.pdf


nothing:
	@ echo this is a packaging makefile, available targets are:
	@ echo "  'srcpack': builds an archive with doc, source code and dtds"
	@ echo "  'macos'  : builds an archive with doc, dtds and binary framework"
	@ echo "  'linux'  : builds an archive with doc, dtds, includes, static and dynamic lib"
	@ echo "  'windows': builds an archive with doc, dtds, includes, dll and lib"
	@ echo "  'js'     : builds an archive with doc, dtds, includes, dll and javascript libraries"

srcpack:
	-[ -d $(SRC) ] && rm -rf $(SRC)
	cd .. && git archive --format=tar.gz HEAD -o $(SRC).tgz
	mv ../$(SRC).tgz .

js:
	make common DEST=$(JSDIR)
	mkdir -p $(JSDIR)/lib
	mkdir -p $(JSDIR)/typescript
	cp $(JSSRC)/libmusicxml.d.ts $(JSDIR)/typescript
	cp $(JSSRC)/libmusicxml.js $(JSDIR)/lib
	cp $(JSSRC)/libmusicxml.wasm.* $(JSDIR)/lib
	tar czf $(JSDIR).tgz $(JSDIR)	

macos:
	make common DEST=$(MACOSDIR)
	make exe DEST=$(MACOSDIR) TOOLS="$(MACTOOLS)"
	cp -R $(BUILDDIR)/libdir/Release/libmusicxml2.framework $(MACOSDIR)
	cd $(MACOSDIR) && ../macdeploy libmusicxml2.framework/libmusicxml2 $(TOOLS)
	tar czf $(MACOSLIB).tgz $(MACOSLIB)

linux:
	make common DEST=$(LINUXDIR)
	make incl DEST=$(LINUXDIR)
	make exe DEST=$(LINUXDIR) TOOLS="$(LINUXTOOLS)"
	cp  $(BUILDDIR)/libmusicxml2.so.$(VERSION) $(LINUXDIR)
	cd  $(LINUXDIR) && ln -sf libmusicxml2.so.$(VERSION) libmusicxml2.so
	tar czf $(LINUXLIB).tgz $(LINUXLIB)

windows:
	make common DEST=$(WINDIR32)
	make incl DEST=$(WINDIR32)
	make exe DEST=$(WINDIR32) TOOLS="$(WINTOOLS32)"
	make common DEST=$(WINDIR64)
	make incl DEST=$(WINDIR64)
	make exe DEST=$(WINDIR64) TOOLS="$(WINTOOLS64)"
	cp  $(BUILDDIR)/win32/Release/libmusicxml2.dll $(WINDIR32)
	cp  $(BUILDDIR)/win32/Release/libmusicxml2.lib $(WINDIR32)
	cp  $(BUILDDIR)/win64/Release/libmusicxml2.dll $(WINDIR64)
	cp  $(BUILDDIR)/win64/Release/libmusicxml2.lib $(WINDIR64)

exe:
	cp $(TOOLS) $(DEST)

incl:
	mkdir -p $(DEST)/include
	cp  $(HEADERS) $(DEST)/include
	-cd $(DEST)/include && chmod 644 *.h

common:
	rm -rf $(DEST)
	mkdir -p $(DEST)/doc
	mkdir -p $(DEST)/dtds
	mkdir -p $(DEST)/schema
	mkdir -p $(DEST)/samples
	cp  $(DOC) $(DEST)/doc
	cp  $(FILES) $(DEST)
	cp  $(DTDS) $(DEST)/dtds
	cp  $(SCHEMA) $(DEST)/schema
	cp  $(SAMPLES) $(DEST)/samples
	
