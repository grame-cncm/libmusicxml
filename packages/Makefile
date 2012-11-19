VERSION  := 3.00
TAG      :=-D tomorrow
ROOTDIR  := ..
SRCDIR   := $(ROOTDIR)/src
CMAKEDIR := $(ROOTDIR)/cmake
DEST     := .
SVN      := svn

SRC 	 := libmusicxml-$(VERSION)-src
MACOSLIB := libmusicxml-$(VERSION)-macosx
MACOSDIR := $(MACOSLIB)
LINUXLIB := libmusicxml-$(VERSION)-linux-x86
LINUXDIR := $(LINUXLIB)
WINLIB   := libmusicxml-$(VERSION)-win32
WINDIR   := $(WINLIB)

HEADERS  = $(wildcard .$(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*/*.h)
FILES    = $(SRCDIR)/CHANGELOG.txt ../README.txt ../license.txt
TOOLS    = xml2guido xmltranspose
WINTOOLS = $(TOOLS:%=$(CMAKEDIR)/release/%.exe)
MACTOOLS = $(TOOLS:%=$(CMAKEDIR)/Release/%)
LINUXTOOLS = $(TOOLS:%=$(CMAKEDIR)/%)
DTDS     = $(wildcard $(ROOTDIR)/dtds/current/*.*)
SAMPLES  = $(wildcard $(ROOTDIR)/samples/*.cpp)
DOC      = $(ROOTDIR)/doc/presentation/libmusicxml2.pdf

nothing:
	@ echo this is a packaging makefile, available targets are:
	@ echo "  'srcpack': builds an archive with doc, source code and dtds"
	@ echo "  'macos'  : builds an archive with doc, dtds and binary framework"
	@ echo "  'linux'  : builds an archive with doc, dtds, includes, static and dynamic lib"
	@ echo "  'windows': builds an archive with doc, dtds, includes, dll and lib"

srcpack:
	rm -rf $(SRC)
	$(SVN) export $(ROOTDIR) $(SRC)
	tar czf $(SRC).tgz $(SRC)

macos:
	make common DEST=$(MACOSDIR)
	make exe DEST=$(MACOSDIR) TOOLS="$(MACTOOLS)"
	cp -R $(CMAKEDIR)/Release/libmusicxml3.framework $(MACOSDIR)
	tar czf $(MACOSLIB).tgz $(MACOSLIB)


linux:
	make common DEST=$(LINUXDIR)
	make incl DEST=$(LINUXDIR)
	make exe DEST=$(LINUXDIR) TOOLS="$(LINUXTOOLS)"
	cp  $(CMAKEDIR)/libmusicxml2.so $(LINUXDIR)
	tar czf $(LINUXLIB).tgz $(LINUXLIB)

windows:
	make common DEST=$(WINDIR)
	make incl DEST=$(WINDIR) 
	make exe DEST=$(WINDIR) TOOLS="$(WINTOOLS)"
	cp  $(CMAKEDIR)/release/libmusicxml2.dll $(WINDIR)
	cp  $(CMAKEDIR)/release/libmusicxml2.lib $(WINDIR)

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
	mkdir -p $(DEST)/samples
	cp  $(DOC) $(DEST)/doc
	cp  $(FILES) $(DEST)
	cp  $(DTDS) $(DEST)/dtds
	cp  $(SAMPLES) $(DEST)/samples
