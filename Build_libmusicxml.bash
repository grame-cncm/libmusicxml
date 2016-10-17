#!/bin/bash

if [ $# -eq 1 ]; then
  DIR=$1
else
  DIR=.
fi

# Write all output to logfile
exec > $(basename $0).log 2>&1

#JMI cd ${DIR}/libmusicxml-git 

echo

echo "==> Building libmusicxml and the samples"
echo
echo

echo '--> cd cmake'
cd cmake
pwd
echo

if [ 1 -eq 1 ]; then
  echo '--> cmake .'
  echo
  cmake .
  echo
fi

if [ 1 -eq 1 ]; then
  echo '--> make clean'
  echo
  make clean
  echo
fi

echo '--> make'
echo
#make --trace
#make -i
make
#--debug=j
echo

echo '--> libmusicxml2'
echo
Find . libmusicxml2
pwd
ls -sal $PWD/libmusicxml2*
echo

echo '--> xml2lilypond'
echo
pwd
ls -sal $PWD/xml2lilypond
echo

if [ 1 -eq 1 ]; then
  cd ../samples
  pwd
  echo

  echo '--> ls -sal'
  echo
  ls -sal
  echo

  cd ../cmake
  pwd
  echo

  echo '--> ls -sal'
  echo
  ls -sal
  echo
fi


#grep -i error ${HOME}/Build_libmusicxml.bash.log
echo



exit

echo '--> sudo make install'
echo
sudo make install
echo

echo '--> find /usr -name "*libmusicxml*"'
echo
find /usr -name "*libmusicxml*"
echo



