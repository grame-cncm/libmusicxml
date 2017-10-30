#!/bin/bash

g++ \
  -Wall -DGCC -std=c++11 -ggdb3 \
  -o compilerVersionTe\
  compilerVersionTest.cpp

./compilerVersionTest
