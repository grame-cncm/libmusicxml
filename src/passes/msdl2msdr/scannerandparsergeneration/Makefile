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

# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

MAIN = NO           # LEXER or PARSER

# determine OUT
ifeq ($(MAIN),YES)
  OUT  = msdlScanner.h msdlScanner.cpp msdlParser.h msdlParser.cpp msdlScanner
else
  OUT  = msdlScanner.h msdlScanner.cpp msdlParser.h msdlParser.cpp msdlParser
endif

# determine INCS
INCS = \
  -I ../../../lib \
  -I ../../../elements \
  -I ../../../representations/msdr \

#CXXFLAGS = -I..


# target
# ------------------------------------------------------

target : $(THIS_FILE) $(OUT)
	ls -salGT *
#	@echo "target: " $@  # print target name
#	@$(MAKE) -f $(THIS_FILE) other-target # invoke other target

#other-target:
#	@echo $@ # print target name


# help
# ------------------------------------------------------

help:
	@echo "What you can do:"
	@echo "  build the scanner and parser code:                         make"
	@echo "  make the scanner or parser code executable:                make MAIN=LEXER|PARSER"
	@echo "  test msdl scanner and or parser:                           make test"
	@echo "  copy the scanner and parser code to the containing folder: make copyup"
	@echo "  remove all generated files:                                make clean"


# driver
# ------------------------------------------------------

msdlDriver.o: msdlDriver.cpp msdlDriver.h msdlParser.h
	g++ -c $(INCS) \
	  msdlDriver.cpp


# scanner
# ------------------------------------------------------

msdlScanner.cpp msdlScanner.h : $(THIS_FILE) msdl.ll msdlParser.h
	flex \
	  --header-file=msdlScanner.h \
	  --outfile=msdlScanner.cpp \
	  msdl.ll

# LEXER_MAIN enables the main() function in msdlScanner.cpp

msdlScanner.o: msdlScanner.cpp msdlParser.h msdlDriver.h
	g++ -c $(INCS) \
	  msdlScanner.cpp

msdlScanner : msdlScanner.cpp
  ifeq ($(MAIN),LEXER)
		g++ $(INCS) \
      -DLEXER_MAIN \
      -o msdlScanner \
      msdlScanner.cpp
  else
		g++ -c $(INCS) \
      -o msdlScanner \
      msdlScanner.cpp
  endif


# parser
# ------------------------------------------------------

msdlParser.cpp msdlParser.h : $(THIS_FILE) msdl.yy
	bison \
	  --defines=msdlParser.h \
	  --output-file=msdlParser.cpp \
	  msdl.yy

# PARSER_MAIN enables the main() function in msdlParser.cpp

msdlParser.o: msdlParser.cpp msdlScanner.h msdlDriver.h
	g++ -c $(INCS) \
	  msdlParser.cpp

msdlParser : msdlParser.h msdlParser.cpp
  ifeq ($(MAIN),PARSER)
		g++ $(INCS) \
      -DPARSER_MAIN \
      -o msdlParser \
      msdlParser.cpp
  else
		g++ -c $(INCS) \
      -o msdlParser \
      msdlParser.cpp
  endif


# test
# ------------------------------------------------------

msdlTest: msdlDriver.o msdlScanner.o msdlParser.o msdlTest.o
	g++ $(INCS) \
	  -o msdlTest \
	  msdlDriver.o msdlScanner.o msdlParser.o msdlTest.o

msdlTest.o: msdlTest.cpp
	g++ -c$(INCS) \
	  msdlTest.cpp

test : msdlTest
	./msdlTest < Test.msdl
#	./msdlScanner < Test.msdl


# exec
# ------------------------------------------------------

.PHONY : exec

exec :
	@$(MAKE) -f $(THIS_FILE) MAIN=YES

# keepexec
# ------------------------------------------------------

.PHONY : keepexec

keepexec : $(OUT)
	cp -p msdlScanner msdlFlexLexer_OK


# copyup
# ------------------------------------------------------

.PHONY : copyup

copyup :
	cp -p msdlScanner.h msdlScanner.cpp msdlParser.h .. #  msdlParser.cpp


# clean
# ------------------------------------------------------

.PHONY : clean

clean :
	rm -f $(OUT) location.hh msdlDriver.o msdlTest.o
#	rm -f $(OUT) $(OBJ)
