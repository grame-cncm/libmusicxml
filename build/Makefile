#
# 
#

.PHONY : msys win32 win64 linux android

MAKE   ?= make
CMAKE  ?= cmake
LIBDIR ?= libdir
IOSDIR ?= iosdir

VERSION = 3.20

system	:= $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	PREFIX ?= /usr/local
	GENERATOR ?= -G Xcode
	MAKEFILE = libmusicxml2.xcodeproj
endif
ifeq ($(system), MINGW)
	GENERATOR ?= ""
	MAKEFILE = Makefile
endif

ifeq ($(system), Linux)
	GENERATOR ?= ""
	MAKEFILE = Makefile
endif


CMAKEOPT ?= 
TOOLS := xml2midi xmlread xml2guido xml2ly xmlversion

all :
	$(MAKE) libmusicxml

help :
	@echo "libmusicxml makefile - Available targets:"
	@echo "   all (default): build the libmusicxml library for the current platform,"	
	@echo "                  build the libmusicxml tools,"	
	@echo "   android   build a static library for Android"	
	@echo "   ios       build a static library for iOS"	
	@echo "   wasm      build the wasm library"	
	@echo "   minimal   build the library with only guido support (no tools and no static library)"	
	@echo 	
	@echo "Misc:"
	@echo "   cmake     re-generates the cmake project"	
	@echo "   format    source code formatting using clang-format"	
	@echo "   install   install library, tools and headers"	
	@echo "   localinstall   install the tools to ~/bin"
	@echo "   package   create the libmusicxml-$(VERSION) package"
	@echo 	
	@echo "Options:"
	@echo "   CMAKEOPT  cmake options passed to cmake by the 'cmake' target"
	@echo "   GENERATOR the cmake generator. Currently '$(GENERATOR)'"
	@echo "   LIBDIR      the generation folder. Currently '$(LIBDIR)'"
	@echo "   MSVC      [Windows only] the windows generator. Currently '$(MSVC)'"
	@echo "   PREFIX    the install location prefix. Currently $(PREFIX)'"
	@echo 
	@echo "CMake options:"
	@echo "   FMWK 		[MacOS only] Generates a framework on MacOS. Default is off"
	@echo "   GDB 		Activates ggdb3 option. Default is off"
	@echo "   LILY 		Include lilypond part. Default is on"
	@echo "NOTE:  CMake options can be passed using CMAKEOPT, e.g."	
	@echo "      'make cmake CMAKEOPT=-DLILY=off'"


#===============================================================
# building inscore and inscoreviewer
#===============================================================
libmusicxml: $(LIBDIR) $(LIBDIR)/$(MAKEFILE)
	$(CMAKE) --build $(LIBDIR) --config Release

$(LIBDIR):
	mkdir $(LIBDIR)

$(LIBDIR)/$(MAKEFILE) : CMakeLists.txt
	cd $(LIBDIR) && $(CMAKE) .. $(CMAKEOPTS) $(GENERATOR)

#===============================================================
# building the javascript library
#===============================================================
wasm:
	$(MAKE) -C ../javascript

#===============================================================
minimal:
	$(MAKE) cmake CMAKEOPT="-DGENTOOLS=off -DLILY=off -DGENSTATIC=off"
	$(CMAKE) --build $(LIBDIR) --config Release

#===============================================================
# building the library for iOS
#===============================================================
ios : 
	$(MAKE) libmusicxml LIBDIR=$(IOSDIR) CMAKEOPTS="-DIOS=on"

#===============================================================
cmake : $(LIBDIR)
	cd $(LIBDIR) && $(CMAKE) .. $(CMAKEOPT)

#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C android -j 6
	cp android/libs/armeabi-v7a/libmusicxml2.so android/libmusicxml2.armeabi-v7a.so
	cp android/libs/x86/libmusicxml2.so android/libmusicxml2.x86.so

#===============================================================
# src code formatting
#===============================================================
format : 
	@which clang-format >/dev/null || (echo "clang-format must be available from your path to run the 'format' target"; false)
	find ../src -path ../src/parser -prune -o -iname '*.cpp' -execdir clang-format -i -style=file {} \;
	find ../src -path ../src/parser -prune -o -iname '*.h' -execdir clang-format -i -style=file {} \;

#===============================================================
# install
#===============================================================
installLog := $(LIBDIR)/install_manifest.txt
install:
	cd $(LIBDIR) && $(CMAKE) .. -DCMAKE_INSTALL_PREFIX=$(PREFIX) 
	$(CMAKE) --build $(LIBDIR) --config Release --target install

uninstall: installedfiles = $(shell cat $(installLog))
uninstall: $(installLog)
	rm -f $(installedfiles) $(installLog)

localinstall:
	cd bin && cp $(TOOLS) $(HOME)/bin


#===============================================================
# packaging
#===============================================================
PACKDIR := ../packages
PACK := $(PACKDIR)/libmusicxml2-$(VERSION)
package:
	$(MAKE) $(NATIVEPACK)

winpack:
	$(MAKE) win64
	cd win64 && cpack -G NSIS64
	mv win64/libmusicxml-*.exe .

macpack: $(PACKDIR)/README.html
	$(MAKE) cmake CMAKEOPT="-DLILY=on -DBRL=on -DFMWK=off"
	$(MAKE) macos
	-[ -d $(PACK) ] && rm -rf $(PACK)
	$(MAKE) install PREFIX=../$(PACK)
	hdiutil create $(PACK).dmg -fs HFS+ -srcfolder $(PACK) -format UDBZ -ov

linuxpack:
	@echo Linux packaging not implemented. 

$(PACKDIR)/README.html: $(PACKDIR)/README.md
	echo "<!DOCTYPE html><html><xmp>" > $@ 
	cat $< >> $@ 
	echo "</xmp>" >> $@ 
	echo "<script src=http://strapdownjs.com/v/0.2/strapdown.js></script>" >> $@ 
	echo "</html>" >> $@ 

