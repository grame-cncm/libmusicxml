#
# 
#

.PHONY : macos ios msys win32 win64 linux android

system	:= $(shell uname -s)
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET ?= macos
	OUTDIR = macos/Release
else
ifeq ($(system), MINGW)
	TARGET ?= windows
else
ifeq ($(system), Linux)
	TARGET ?= linux
	OUTDIR = linux
else
	TARGET = undefined
endif
endif
endif

OUT  := libmusicxml2.framework libmusicxml2.a
MACX ?= macos
IOS  := ios
ANDROID := android
LINUX   := linux
MSVC   ?= Visual Studio 14 2015
MSVC64 := $(MSVC) Win64

CMAKEOPT ?= 
TOOLS := xml2midi xmlread xml2guido xml2ly

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
	@echo "   install   install the tools to ~/bin"	
	@echo 
	@echo "NOTE: windows cmake generator is $(MSVC)"	
	@echo "      use MSVC=\"another generator\" to change"

#===============================================================
# building the javascript library
#===============================================================
js:
	make -j 4 -C ../javascript/build
	make -j 4 -C ../javascript/build WASM=yes

#===============================================================
# building the library on Mac OS X
#===============================================================
.PHONY : libmusicxml2.framework

macos : libmusicxml2.framework libmusicxml2.tgz

libmusicxml2.framework : $(MACX)/libmusicxml2.xcodeproj
	cmake --build $(MACX) --config Release -- -jobs 4
	rm -rf libmusicxml2.framework
	cp -R $(MACX)/Release/libmusicxml2.framework .

$(MACX)/libmusicxml2.xcodeproj : CMakeLists.txt
	@[ -d $(MACX) ] || mkdir $(MACX)
	cd $(MACX) && cmake .. -G Xcode $(CMAKEOPT)  -Wno-dev

libmusicxml2.tgz : libmusicxml2.framework
	tar czf libmusicxml2.tgz libmusicxml2.framework


#===============================================================
# building the library for iOS
#===============================================================
.PHONY : libmusicxml2.a

$(IOS) : libmusicxml2.a

libmusicxml2.a : $(IOS)/libmusicxml2.xcodeproj
	cmake --build $(MACX) --config Release -- -jobs 4
	cp $(IOS)/Release-iphoneos/libmusicxml2.a .

$(IOS)/libmusicxml2.xcodeproj : CMakeLists.txt
	[ -d $(IOS) ] || mkdir $(IOS)
	cd $(IOS) && cmake .. -G Xcode -DIOS=yes


#===============================================================
# building the library on windows
#===============================================================
.PHONY : msys win32 win64 win32/Release/libmusicxml2.dll win64/Release/libmusicxml264.dll

#===============================================================
# using msys
msys : msys/Makefile msys/libmusicxml2.so

msys/libmusicxml2.so :
	make -C msys

msys/Makefile : CMakeLists.txt
	[ -d msys ] || mkdir msys
	cd msys && cmake .. -G "MSYS Makefiles"  $(CMAKEOPT)

#===============================================================
# using visual studio
windows :
	make win32
	make win64

win32 : win32/libmusicxml2.sln libmusicxml2.dll 

libmusicxml2.dll : win32/Release/libmusicxml2.dll
	cp win32/Release/libmusicxml2.dll .
	cp win32/Release/libmusicxml2.lib .

win32/Release/libmusicxml2.dll : win32/libmusicxml2.sln
	cmake --build win32 --config Release -- /maxcpucount:4

win32/libmusicxml2.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(MSVC)" $(CMAKEOPT)


win64 : win64/libmusicxml2.sln libmusicxml2-64.dll libmusicxml2-64.lib 

libmusicxml2-64.lib : libmusicxml2-64.dll
libmusicxml2-64.dll : win64/Release/libmusicxml2.dll
	cp win64/Release/libmusicxml2.dll libmusicxml2-64.dll
	cp win64/Release/libmusicxml2.lib libmusicxml2-64.lib

win64/Release/libmusicxml264.dll : win64/libmusicxml2.sln
	cmake --build win64 --config Release -- /maxcpucount:4 

win64/libmusicxml2.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(MSVC64)" $(CMAKEOPT)

#===============================================================
# building the library on linux
#===============================================================
.PHONY : linux libmusicxml2.so

linux : libmusicxml2.so

libmusicxml2.so :  $(OUTDIR)/Makefile 
	make -C linux
	rm -rf libmusicxml2*.so*
	cp -R linux/libmusicxml2*.so* .
	
linux/Makefile : CMakeLists.txt
	[ -d linux ] || mkdir linux
	cd linux && cmake .. -G "Unix Makefiles" 

#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C android -j 4

#===============================================================
# src code formatting
#===============================================================
format :
	find ../src -path ../src/parser -prune -o -iname '*.cpp' -execdir clang-format -i -style=file {} \;
	find ../src -path ../src/parser -prune -o -iname '*.h' -execdir clang-format -i -style=file {} \;

#===============================================================
# install
#===============================================================
install:
	cd $(OUTDIR) && cp $(TOOLS) $(HOME)/bin
