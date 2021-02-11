#
#  MusicXML Library
#  Copyright (C) Grame 2006-2013
#
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at http://mozilla.org/MPL/2.0/.

#  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
#  research@grame.fr
#


# Makefile for MSDL scanner and parser generation


# variables
# ------------------------------------------------------

#OBJ  = msdlFlexLexer.o msdlBisonParser.o

MAIN = NO           # LEXER or PARSER

# determine OUT
ifeq ($(MAIN),YES)
  OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp msdlFlexLexer
else
  OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp msdlBisonParser
endif

# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

#CXXFLAGS = -I..


# target
# ------------------------------------------------------

target : $(OUT)
	ls -salGT *
#	@echo "target: " $@  # print target name
#	@$(MAKE) -f $(THIS_FILE) other-target # invoke other target

#other-target:
#	@echo $@ # print target name


# help
# ------------------------------------------------------

help:
	@echo "What you can do:"
	@echo "  build the lexer and parser code:                         make"
	@echo "  make the lexer or parser code executable:                make MAIN=LEXER|PARSER"
	@echo "  test msdlFlexLexer:                                      make test"
	@echo "  copy the lexer and parser code to the containing folder: make copyup"
	@echo "  remove all generated files:                              make clean"


# scanner
# ------------------------------------------------------

msdlFlexLexer.h : msdl.ll msdlBisonParser.hpp
	flex \
	  -I \
	  -o msdlFlexLexer.cpp \
	  --prefix=msdl \
	  msdl.ll

# LEXER_MAIN enables the main() function in msdlFlexLexer.cpp

msdlFlexLexer : msdlFlexLexer.cpp
  ifeq ($(MAIN),LEXER)
		g++  \
      -DLEXER_MAIN \
      -I ../../../lib \
      -I ../../../elements \
      -o msdlFlexLexer \
      msdlFlexLexer.cpp
  else
		g++ -c\
      -I ../../../lib \
      -I ../../../elements \
      -o msdlFlexLexer \
      msdlFlexLexer.cpp
  endif


# parser
# ------------------------------------------------------

msdlBisonParser.hpp : msdl.yy
	bison \
	  --defines=msdlBisonParser.hpp \
	  -o msdlBisonParser.cpp \
	  -Dapi.prefix={msdl} \
	  msdl.yy

# PARSER_MAIN enables the main() function in msdlBisonParser.cpp

msdlBisonParser : msdlBisonParser.hpp msdlBisonParser.cpp
  ifeq ($(MAIN),PARSER)
		g++ \
      -DPARSER_MAIN \
      -I ../../../lib \
      -I ../../../elements \
      -I ../../../representations/msdr \
      -o msdlBisonParser \
      msdlBisonParser.cpp
  else
		g++ -c \
      -I ../../../lib \
      -I ../../../elements \
      -I ../../../representations/msdr \
      -o msdlBisonParser \
      msdlBisonParser.cpp
  endif


# test
# ------------------------------------------------------

msdlTest: msdlDriver.o msdlBisonParser.o msdlFlexLexer.o msdlTest.o
	g++ -o msdlTest msdlDriver.o msdlBisonParser.o msdlFlexLexer.o msdlTest.o

msdlDriver.o: msdlDriver.cpp msdlDriver.h msdlBisonParser.hpp
	g++ -c msdlDriver.cpp

msdlTest.o: msdlTest.cpp
	g++ -c msdlTest.cpp

test : msdlTest
	./msdlTest < Test.msdl
#	./msdlFlexLexer < Test.msdl


# exec
# ------------------------------------------------------

.PHONY : exec

exec :
	@$(MAKE) -f $(THIS_FILE) MAIN=YES

# keepexec
# ------------------------------------------------------

.PHONY : keepexec

keepexec : $(OUT)
	cp -p msdlFlexLexer msdlFlexLexer_OK


# copyup
# ------------------------------------------------------

.PHONY : copyup

copyup :
	cp -p msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp .. #  msdlBisonParser.cpp


# clean
# ------------------------------------------------------

.PHONY : clean

clean :
	rm -f $(OUT) location.hh msdlDriver.o msdlTest.o
#	rm -f $(OUT) $(OBJ)
