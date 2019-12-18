#!/bin/sh

if [ ! -d ./iosdir/libmusicxml2.xcodeproj  ]; then
   if [ ! -d iosdir  ]; then
        mkdir iosdir
   fi
	cd ./iosdir
	cmake .. -G Xcode -DIOS=yes
	cd ..
else
  echo "./libmusicxml/build/iosdir/libmusicxml2.xcodeproj already  exists"
fi

if [ ! -d ./libdir/libmusicxml2.xcodeproj  ]; then
	if [ ! -d libdir  ]; then
		mkdir libdir
	fi
	cd ./libdir
	cmake .. -G Xcode -DIOS=no
	cd ..
else
	echo "./libmusicxml/build/libdir/libmusicxml2.xcodeproj already  exists"
fi
