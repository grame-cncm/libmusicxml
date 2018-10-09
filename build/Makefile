#
# 
#

.PHONY : macos ios macc++11 android

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
LINUX := linux
MSVC   ?= "Visual Studio 14 2015"
MSVC64 := $(MSVC) Win64

# try to detect Visual Studio version used
# ifeq ($(TARGET), windows)
#  MSVC ?= C:\Program Files (x86)\Microsoft Visual Studio 12.0
#  ifneq ($(wildcard "$(MSVC)"), "") 
#   WIN32VS ?= Visual Studio 12 2013
#   WIN64VS ?= $(WIN32VS) Win64
#   endif
#  else
#  MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0
#  ifneq ($(wildcard "$(MSVC)"), "") 
#   WIN32VS ?= Visual Studio 10
#   WIN64VS ?= $(WIN32VS) Win64
#  endif
# endif

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
	@echo "   js        build a javascript library"	
	@echo "the platform targets is automatically evaluated by the default target."	
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

#===============================================================
# building the library on Mac OS X
#===============================================================
.PHONY : $(OUTDIR)/Release/libmusicxml2.framework $(IOS)/Release-iphoneos/libmusicxml2.a

macos : $(MACX)/libmusicxml2.xcodeproj libmusicxml2.framework libmusicxml2.tgz

libmusicxml2.framework :  $(MACX)/libmusicxml2.xcodeproj maclib
	rm -rf libmusicxml2.framework
	cp -R macos/Release/libmusicxml2.framework .

maclib :
	xcodebuild -jobs 8 -project $(MACX)/libmusicxml2.xcodeproj -target ALL_BUILD -configuration Release

$(MACX)/libmusicxml2.xcodeproj : CMakeLists.txt
	[ -d $(MACX) ] || mkdir $(MACX)
	cd $(MACX) && cmake .. -G Xcode $(CMAKEOPT)  -Wno-dev

libmusicxml2.tgz : libmusicxml2.framework
	tar czf libmusicxml2.tgz libmusicxml2.framework


#===============================================================
# building the library for iOS
#===============================================================
$(IOS) : $(IOS)/libmusicxml2.xcodeproj libmusicxml2.a

libmusicxml2.a : $(IOS)/libmusicxml2.xcodeproj $(IOS)/Release-iphoneos/libmusicxml2.a
	cp $(IOS)/Release-iphoneos/libmusicxml2.a .

$(IOS)/Release-iphoneos/libmusicxml2.a :
	xcodebuild -jobs 8 -project $(IOS)/libmusicxml2.xcodeproj -target ALL_BUILD -configuration Release
 
$(IOS)/libmusicxml2.xcodeproj : CMakeLists.txt
	[ -d $(IOS) ] || mkdir $(IOS)
	cd $(IOS) && cmake .. -G Xcode -DMIDIEXPORT=no -DIOS=yes


#===============================================================
# building the library on windows
#===============================================================
.PHONY : win32 win64 win32/Release/libmusicxml2.dll win64/Release/libmusicxml264.dll

windows :
	make win32
	make win64

win32 : win32/libmusicxml2.sln libmusicxml2.dll libmusicxml2.lib 

libmusicxml2.lib : libmusicxml2.dll

libmusicxml2.dll : win32/Release/libmusicxml2.dll
	cp win32/Release/libmusicxml2.dll .
	cp win32/Release/libmusicxml2.lib .

win32/Release/libmusicxml2.dll : win32/libmusicxml2.sln
	cmake --build win32 --config Release -- /maxcpucount:4 

win32/libmusicxml2.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(MSVC)" $(CMAKEOPT)


win64 : win64/libmusicxml2.sln libmusicxml264.dll libmusicxml264.lib 

libmusicxml264.lib : libmusicxml264.dll
libmusicxml264.dll : win64/Release/libmusicxml264.dll
	cp win64/Release/libmusicxml264.dll .
	cp win64/Release/libmusicxml264.lib .

win64/Release/libmusicxml264.dll : win64/libmusicxml2.sln
	cmake --build win64 --config Release -- /maxcpucount:4 

win64/libmusicxml2.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(MSVC)" $(CMAKEOPT)

#===============================================================
# building the library on linux
#===============================================================
.PHONY : $(LINUX) libmusicxml2.so

linux : $(OUTDIR)/Makefile libmusicxml2.so

libmusicxml2.so :  $(OUTDIR)/Makefile  $(OUTDIR)/libmusicxml2.so
	rm -rf libmusicxml2*.so*
	make -C $(OUTDIR)
	cp -R $(OUTDIR)/libmusicxml2*.so* .

$(OUTDIR)/libmusicxml2.so : 
	make -C $(OUTDIR)
	
$(OUTDIR)/Makefile : CMakeLists.txt
	[ -d $(OUTDIR) ] || mkdir $(OUTDIR)
	cd $(OUTDIR) && cmake .. -G "Unix Makefiles" -DMIDIEXPORT=yes

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
