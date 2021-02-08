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
#OBJ  = msslKeywords.o msdlFlexLexer.o msdlBisonParser.o
OBJ  = msdlFlexLexer.o msdlBisonParser.o


# target
# ------------------------------------------------------

target : $(OUT)


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

msdlBisonParser.cpp : msdl.y
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.y

msdlBisonParser.hpp : msdl.y
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.y


# C++ compile
# ------------------------------------------------------

# for tests, these symbols can be defined:
#   LEXER_MAIN  creates a main() function in msdlFlexLexer.cpp
#   PARSER_MAIN creates a main() function in msdlBisonParser.cpp

CXXFLAGS = -I..

msdlFlexLexer : msdlFlexLexer.cpp
	g++ -c -I ../../lib -I ../../elements -o msdlFlexLexer.o msdlFlexLexer.cpp
#	g++ -I ../../lib -I ../../elements -o msdlFlexLexer msdlFlexLexer.cpp -DLEXER_MAIN
#	ld msslKeywords.o msdlFlexLexer.o -o msdlFlexLexer


msdlBisonParser : msdlBisonParser.cpp
	g++ -I ../../lib -I ../../elements -o msdlBisonParser msdlBisonParser.cpp -DPARSER_MAIN


#msdl : $(OBJ)
#	g++ $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)


# test
# ------------------------------------------------------

test:
	./msdlFlexLexer <../HelloWorld.msdl


# exeok
# ------------------------------------------------------

exeok : $(OUT)
	cp -p msdlFlexLexer ../msdlFlexLexer_OK


# copyup
# ------------------------------------------------------

copyup:
	cp -p msdlKeywords.h msdlKeywords.cpp msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp .. #  msdlBisonParser.cpp


# clean
# ------------------------------------------------------

clean:
	rm -f $(APPL) $(OUT) $(OBJ)
