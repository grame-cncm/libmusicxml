#!/bin/bash

cmake -G Xcode ../../../cmake 

make

open libmusicxml2.xcodeproj
