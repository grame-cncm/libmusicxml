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