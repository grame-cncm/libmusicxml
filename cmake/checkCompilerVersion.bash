#!/bin/bash

set -x

g++ \
  -Wall -DGCC -std=c++11 -ggdb3 \
  -o compilerVersionCheck \
  compilerVersionCheck.cpp

echo

./compilerVersionCheck

echo
