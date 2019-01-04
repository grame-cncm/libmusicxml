#!/bin/sh

if [ ! -d ./ios/libmusicxml2.xcodeproj  ]; then
   if [ ! -d ios  ]; then
        mkdir ios
   fi
	cd ./ios
	cmake .. -G Xcode -DMIDIEXPORT=no -DC++11=yes -DIOS=yes
	cd ..
else
  echo "./libmusicxml/build/ios/libmusicxml2.xcodeproj already  exists"
fi

if [ ! -d ./macos/libmusicxml2.xcodeproj  ]; then
	if [ ! -d macos  ]; then
		mkdir macos
	fi
	cd ./macos
	cmake .. -G Xcode -DMIDIEXPORT=no -DC++11=yes -DIOS=no
	cd ..
else
	echo "./libmusicxml/build/ios/libmusicxml2.xcodeproj already  exists"
fi
