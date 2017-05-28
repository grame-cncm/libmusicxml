#!/bin/bash

function usage ()
  {
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo "  $0 [cmake] [clean] [install]"
    echo
    exit 1
  }

CMAKE=0
CLEAN=0
INSTALL=0


# Analyze the parameters if any
# -----------------------------------------

#echo '$@'" = $@"
#echo '$#'" = $#"
#echo

for PARAM in "$@"; do
  case $PARAM in
    "cmake" )
      #echo "cmake found"
      CMAKE=1
      ;;

    "clean" )
      #echo "clean found"
      CLEAN=1
      ;;
    
    "install" )
      #echo "install found"
      INSTALL=1
      ;;
    
    *)
      usage $PARAM
      ;;
  esac
done

echo "User choices:"
echo "  CMAKE = $CMAKE"
echo "  CLEAN = $CLEAN"
echo


# Write all output to logfile
# -----------------------------------------

exec > $(basename $0).log 2>&1

echo


# Let's go!
# -----------------------------------------

echo "==> date is:"
date
echo

echo "==> Building libmusicxml and the samples"
echo
echo

echo '--> cd cmake'
cd cmake
pwd
echo


# Should 'cmake' be run?
# -----------------------------------------

if [ $CMAKE ]; then # run cmake after filenames changes
  echo '--> cmake .'
  echo
  cmake .
  echo
fi


# Should 'make clean' be run?
# -----------------------------------------

if [ $CLEAN ]; then # make everything
  echo '--> make clean'
  echo
  make clean
  echo
fi


# Run 'make'
# -----------------------------------------

echo '--> make'
echo
#make --trace
#make -i
make
#--debug=j
echo


# Show the results
# -----------------------------------------

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


# Should 'make install' be run?
# -----------------------------------------

if [ $INSTALL ]; then
  echo '--> sudo make install'
  echo
  sudo make install
  echo
  
  echo '--> find /usr -name "*libmusicxml*"'
  echo
  find /usr -name "*libmusicxml*"
  echo
fi
