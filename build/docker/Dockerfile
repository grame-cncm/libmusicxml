FROM ubuntu:bionic

ENV LD_LIBRARY_PATH=/usr/local/lib

RUN \
  apt-get update && \
  apt-get install -y git cmake g++ 

# g++-8 could be installed but it'll be available as g++8 and thus not found by cmake 
# you'll have to tell cmake explicitely or to rename g++8 to g++ 


RUN mkdir grame
WORKDIR /grame

RUN git clone --depth 1 https://github.com/grame-cncm/libmusicxml.git
WORKDIR /grame/libmusicxml/build
RUN  cmake . && make -j 4 && make install
WORKDIR /grame

