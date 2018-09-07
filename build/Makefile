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

MACX := macos
IOS  := ios
ANDROID := android
LINUX := linux

# default generator for windows
GENERATOR = "Visual Studio 14 2015"

C++11 ?= no
CMAKEOPT ?= -DC++11=$(C++11)
TOOLS := RandomMusic readunrolled xml2midi xmlfactory xmlread xmlversion countnotes partsummary xml2guido xmlclone xmliter xmltranspose

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
	@echo "   js        build a javascript library"	
	@echo "the platform targets is automatically evaluated by the default target."	
	@echo "NOTE: windows target is based on $(GENERATOR)"	
	@echo "      use GENERATOR=\"another version\" to change"	
	@echo "Misc:"
	@echo "   install   install the tools to ~/bin"	

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
	cd win32 && cmake .. -G "$(GENERATOR)" $(CMAKEOPT)


win64 : win64/libmusicxml2.sln libmusicxml264.dll

libmusicxml264.dll : win64/Release/libmusicxml264.dll
	cp win64/Release/libmusicxml264.dll .
	cp win64/Release/libmusicxml264.lib .

win64/Release/libmusicxml264.dll : win64/libmusicxml2.sln
	cmake --build win34 --config Release -- /maxcpucount:4

win64/libmusicxml2.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(GENERATOR) Win64" $(CMAKEOPT) -DMIDIEXPORT=yes

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
	cd linux && cmake .. -G "Unix Makefiles" -DMIDIEXPORT=yes

#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C android -j 4
	cp -f android/libs/armeabi/libmusicxml2.so android/

#===============================================================
# install
#===============================================================
install:
	cd $(OUTDIR) && cp $(TOOLS) $(HOME)/bin
