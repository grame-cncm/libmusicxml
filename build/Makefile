#
# 
#

.PHONY : msys win32 win64 linux android

PDIR   ?= libdir
VERSION = 3.19

system	:= $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET ?= macos
	OUTDIR = $(PDIR)/Release
	GENERATOR ?= Xcode
	PREFIX ?= /usr/local
	NATIVEPACK = macpack
else
ifeq ($(system), MINGW)
	TARGET ?= windows
	PDIR  ?= win64
	NATIVEPACK = winpack
else
ifeq ($(system), Linux)
	TARGET ?= linux
	OUTDIR = $(PDIR)
	GENERATOR ?= Unix Makefiles
	PREFIX ?= /usr/local
	NATIVEPACK = linuxpack
else
	TARGET = undefined
	NATIVEPACK = undefined
endif
endif
endif

IOS  := iosdir
MSVC   ?= Visual Studio 14 2015
MSVC64 := $(MSVC) Win64
JOBS ?= 6

CMAKEOPT ?= 
TOOLS := xml2midi xmlread xml2guido xml2ly xmlversion xml2brl

all :
	git submodule update --init
	make $(TARGET)

undefined:
	$(error System is undefined, not target available)

help :
	@echo libmusicxml makefile - Targets are :
	@echo "   all (default): build the libmusicxml library for the current platform,"	
	@echo "                  build the libmusicxml tools,"	
	@echo 
	@echo "Platform targets to build the libmusicxml library are:"	
	@echo "   macos     build the library for macos"	
	@echo "   windows   build 32 and 64 bits library for windows"
	@echo "   linux     build the library for linux"
	@echo "   android   build a static library for Android"	
	@echo "   ios       build a static library for iOS"	
	@echo "   msys      build on Windows using MSys"	
	@echo "   js        build a javascript library"	
	@echo "the platform targets is automatically evaluated by the default target."	
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
	@echo "   PDIR      the generation folder. Currently '$(PDIR)'"
	@echo "   MSVC      [Windows only] the windows generator. Currently '$(MSVC)'"
	@echo "   PREFIX    the install location prefix. Currently $(PREFIX)'"
	@echo 
	@echo "CMake options:"
	@echo "   FMWK 		[MacOS only] Generates a framework on MacOS. Default is on"
	@echo "   GDB 		Activates ggdb3 option. Default is off"
	@echo "   LILY 		Include lilypond part. Default is on"
	@echo "NOTE:  CMake options can be passed using CMAKEOPT, e.g."	
	@echo "      'make cmake CMAKEOPT=-DLILY=off'"


$(PDIR) :
	mkdir $(PDIR)

#===============================================================
# building the javascript library
#===============================================================
js:
	make -C ../javascript

#===============================================================
# building the library on Mac OS X
#===============================================================
macos : $(PDIR)/libmusicxml2.xcodeproj
	cmake --build $(PDIR) --config Release -- -jobs $(JOBS)
	-cd lib &&  [ -d libmusicxml2.framework ] && tar czf libmusicxml2.tgz libmusicxml2.framework || echo "no framework"

$(PDIR)/libmusicxml2.xcodeproj : $(PDIR) CMakeLists.txt
	cd $(PDIR) && cmake .. -G Xcode $(CMAKEOPT)  -Wno-dev


#===============================================================
# building the library for iOS
#===============================================================
ios : $(IOS)/libmusicxml2.xcodeproj
	cmake --build $(IOS) --config Release -- -jobs $(JOBS)

$(IOS)/libmusicxml2.xcodeproj : CMakeLists.txt
	[ -d $(IOS) ] || mkdir $(IOS)
	cd $(IOS) && cmake .. -G Xcode -DIOS=yes -Wno-dev $(CMAKEOPT)


#===============================================================
# building the library on windows
#===============================================================

#===============================================================
# using msys
msys : GENERATOR ?= MSYS Makefiles
msys : $(PDIR)/Makefile
	make -C $(PDIR)

#===============================================================
# using visual studio
windows :
	make win32
	make win64

win32 : win32/libmusicxml2.sln 
	cmake --build win32 --config Release -- /maxcpucount:4

win32/libmusicxml2.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(MSVC)" $(CMAKEOPT)

win64 : win64/libmusicxml2.sln 
	cmake --build win64 --config Release -- /maxcpucount:4 

win64/libmusicxml2.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(MSVC64)" $(CMAKEOPT)

#===============================================================
# building the library on linux
#===============================================================
linux : $(PDIR)/Makefile
	make -C $(PDIR)

$(PDIR)/Makefile : $(PDIR) CMakeLists.txt
	cd $(PDIR) && cmake .. -G "$(GENERATOR)"  $(CMAKEOPT)


#===============================================================
cmake : $(PDIR)
	cd $(PDIR) && cmake .. -G "$(GENERATOR)" -Wno-dev $(CMAKEOPT)

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
installLog := $(PDIR)/install_manifest.txt
install:
	cd $(PDIR) && cmake .. -DCMAKE_INSTALL_PREFIX=$(PREFIX) 
	cmake --build $(PDIR) --config Release --target install

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
