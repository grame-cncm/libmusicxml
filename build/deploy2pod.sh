#!/bin/bash

# Bash script for deploying built libraries and headers to Pod

if [ $# -lt 1 ]
then
	echo Usage: $0 path-to-pod-root 
	exit
else
	echo Checking if folder $1 actually exists...

	if [ ! -e "$1" ]; then
		echo Pod path $1 doesnot exist! Exiting... .
		exit
	fi
fi

##### Build XCode project if it doesn't exist
bash buildXCodeProject.sh 

##### Build iphoneos and iphonesimulator

libname="/libmusicxml2.a"
iphonebuildpath="$(pwd)/lib/Release/iphoneos$libname"
simulatorbuildpath="$(pwd)/lib/Release/iphonesimulator$libname"
universalpath="$(pwd)/lib/Release$libname"

xcodebuild -project iosdir/libmusicxml2.xcodeproj  -target musicxml2 -configuration Release -sdk iphoneos clean build CONFIGURATION_BUILD_DIR="lib/Release/iphoneos"
xcodebuild -project iosdir/libmusicxml2.xcodeproj  -target musicxml2 -configuration Release -arch x86_64 only_active_arch=no -sdk iphonesimulator clean build CONFIGURATION_BUILD_DIR="lib/Release/iphonesimulator"


##### Move to Pod location
# pod paths
podlibpath="$1/GuidoKit/Classes/Libs/libmusicxml$libname"
podsrcpath="$1/GuidoKit/Classes/Libs/libmusicxml"

##### Make Universal Lib and copy to Pod
# Check if Release-iphoneos and Release-iphonesimulator libraries exists

if [ -f "$iphonebuildpath" ]
then
	echo iPhoneOS lib exists!
else
	echo Build the iPhoneOS library in $iphonebuildpath ! Exiting... .
	exit
fi

if [ -f $simulatorbuildpath ]
then
	echo iPhoneSimulator lib exists!
else
	echo Build the simulator library in ./Release-iphonesimulator ! Exiting... .
	exit
fi

# Lipo them together
lipo -create $iphonebuildpath $simulatorbuildpath -output $universalpath

if [ $? -eq 0 ]; then
	echo Lipo built the Universal library! Copying to Pod.
else
	echo Lipo failed! Exiting... .
	exit
fi

cp -v $universalpath $podlibpath
if [ $? -eq 0 ]; then
	echo Universal Library copied to Pod $podlibpath
else
	echo Copy failed! Exiting... .
	exit
fi

##### Update src
localsrcpath="$(pwd)/../src"
rsync -avz --exclude 'antescofo' --exclude '.DS_Store' --exclude 'lilypond' $localsrcpath $podsrcpath


