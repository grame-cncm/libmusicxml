#!/bin/sh

if [ ! -d ./ios/libmusicxml2.xcodeproj  ]; then
   if [ ! -d iosdir  ]; then
        mkdir iosdir
   fi
	cd ./ios
	cmake .. -G Xcode -DIOS=yes
	cd ..
else
  echo "./libmusicxml/build/ios/libmusicxml2.xcodeproj already  exists"
fi

if [ ! -d ./macos/libmusicxml2.xcodeproj  ]; then
	if [ ! -d macos  ]; then
		mkdir macos
	fi
	cd ./macos
	cmake .. -G Xcode -DIOS=no
	cd ..
else
	echo "./libmusicxml/build/ios/libmusicxml2.xcodeproj already  exists"
fi
