#
# 
#

.PHONY : msys win32 win64 linux android

PDIR   ?= libdir

system	:= $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET ?= macos
	OUTDIR = $(PDIR)/Release
	GENERATOR ?= Xcode
	PREFIX ?= /usr/local
else
ifeq ($(system), MINGW)
	TARGET ?= windows
	PDIR  ?= win64
else
ifeq ($(system), Linux)
	TARGET ?= linux
	OUTDIR = $(PDIR)
	GENERATOR ?= Unix Makefiles
	PREFIX ?= /usr/local
else
	TARGET = undefined
endif
endif
endif

IOS  := iosdir
MSVC   ?= Visual Studio 14 2015
MSVC64 := $(MSVC) Win64

CMAKEOPT ?= 
TOOLS := xml2midi xmlread xml2guido xml2ly xmlversion

all :
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
	@echo "   universal switch to universal binaries (MacOS)"	
	@echo "   native 	switch universal binaries off (MacOS)"	
	@echo "   cmake     re-generates the cmake project"	
	@echo "   format    source code formatting using clang-format"	
	@echo "   install   install library, tools and headers"	
	@echo "   localinstall   install the tools to ~/bin"
	@echo 
	@echo "NOTE: windows cmake generator is $(MSVC)"	
	@echo "      use MSVC=\"another generator\" to change"


$(PDIR) :
	mkdir $(PDIR)

#===============================================================
# building the javascript library
#===============================================================
js:
	make -j 4 -C ../javascript/build
	make -j 4 -C ../javascript/build WASM=yes

#===============================================================
# building the library on Mac OS X
#===============================================================
macos : $(PDIR)/libmusicxml2.xcodeproj
	cmake --build $(PDIR) --config Release -- -jobs 4
	cd lib &&  tar czf libmusicxml2.tgz libmusicxml2.framework

$(PDIR)/libmusicxml2.xcodeproj : $(PDIR) CMakeLists.txt
	cd $(PDIR) && cmake .. -G Xcode $(CMAKEOPT)  -Wno-dev

universal: $(PDIR)
	cd $(PDIR) && cmake .. -DUNIVERSAL=on -Wno-dev

native: $(PDIR)
	cd $(PDIR) && cmake .. -DUNIVERSAL=off -Wno-dev


#===============================================================
# building the library for iOS
#===============================================================
ios : $(IOS)/libmusicxml2.xcodeproj
	cmake --build $(IOS) --config Release -- -jobs 4

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
	ndk-build -C android -j 4

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
