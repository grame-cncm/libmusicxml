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

APPL = msdl

OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp msdlFlexLexer

OBJ  = msdlFlexLexer.o msdlBisonParser.o

MAIN = NO

ifeq ($(MAIN),YES)
  OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp msdlFlexLexer
else
  OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp
endif

# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))


# target
# ------------------------------------------------------

target : $(OUT)
#	@echo "target: " $@  # print target name
#	@$(MAKE) -f $(THIS_FILE) other-target # invoke other target

#other-target:
#	@echo $@ # print target name



# scanner
# ------------------------------------------------------

FLEX = flex # --c++  # --trace --verbose # --debug

msdlFlexLexer.h : msdl.ll msdlBisonParser.hpp
	flex -o msdlFlexLexer.cpp msdl.ll

msdlFlexLexer.cpp : msdl.ll msdlBisonParser.hpp
	flex -o msdlFlexLexer.cpp msdl.ll


# parser
# ------------------------------------------------------

BISON = bison # -Wcounterexamples
PREFIX = msdl

msdlBisonParser.cpp : msdl.yy
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.yy

msdlBisonParser.hpp : msdl.yy
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.yy


# C++ compile
# ------------------------------------------------------

# for tests, these symbols can be defined:
#   LEXER_MAIN  creates a main() function in msdlFlexLexer.cpp
#   PARSER_MAIN creates a main() function in msdlBisonParser.cpp

CXXFLAGS = -I..

msdlFlexLexer : msdlFlexLexer.cpp
  ifeq ($(MAIN),YES)
		g++ -I ../../lib -I ../../elements -o msdlFlexLexer msdlFlexLexer.cpp -DLEXER_MAIN
  else
		g++ -c -I ../../lib -I ../../elements -o msdlFlexLexer.o msdlFlexLexer.cpp
  endif


msdlBisonParser : msdlBisonParser.cpp
	g++ -I ../../lib -I ../../elements -o msdlBisonParser msdlBisonParser.cpp -DPARSER_MAIN


#msdl : $(OBJ)
#	g++ $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)


# help
# ------------------------------------------------------

help:
	@echo "What you can do:"
	@echo "  build the lexer and parser code:                         make"
	@echo "  build the lexer and parser code and msdlFlexLexer:       make MAIN=YES"
	@echo "  test msdlFlexLexer:                                      make test"
	@echo "  copy the lexer and parser code to the containing folder: make copyup"
	@echo "  remove all generated files:                              make clean"


# exec
# ------------------------------------------------------

exec:
	@$(MAKE) -f $(THIS_FILE) MAIN=YES

# test
# ------------------------------------------------------

test:
	./msdlFlexLexer < Test.msdl


# keepexec
# ------------------------------------------------------

keepexec : $(OUT)
	cp -p msdlFlexLexer msdlFlexLexer_OK


# copyup
# ------------------------------------------------------

copyup:
	cp -p msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp .. #  msdlBisonParser.cpp


# clean
# ------------------------------------------------------

clean:
	rm -f $(APPL) $(OUT) $(OBJ)
