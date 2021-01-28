
APPL = msdl

OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlBisonParser.hpp msdlBisonParser.cpp msdlFlexLexer
#OBJ  = msslKeywords.o msdlFlexLexer.o msdlBisonParser.o
OBJ  = msdlFlexLexer.o msdlBisonParser.o

target : $(OUT)

# flex

FLEX = flex # --c++  # --trace --verbose # --debug

msdlFlexLexer.h : msdl.ll msdlBisonParser.hpp
	flex -o msdlFlexLexer.cpp msdl.ll

msdlFlexLexer.cpp : msdl.ll msdlBisonParser.hpp
	flex -o msdlFlexLexer.cpp msdl.ll

# bison

BISON = bison # -Wcounterexamples
PREFIX = msdl

msdlBisonParser.cpp : msdl.y
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.y

msdlBisonParser.hpp : msdl.y
	$(BISON) -d -o msdlBisonParser.cpp  -p $(PREFIX) msdl.y

# g++
# for tests, these symbols can be defined:
#   LEXER_MAIN  creates a main() function in msdlFlexLexer.cpp
#   PARSER_MAIN creates a main() function in msdlBisonParser.cpp

CXXFLAGS = -I..

msdlFlexLexer : msdlFlexLexer.cpp
	g++ -I ../../lib -I ../../elements -o msdlFlexLexer msdlFlexLexer.cpp -DLEXER_MAIN
#	g++ -I ../../lib -I ../../elements -o msdlFlexLexer.o msdlFlexLexer.cpp -DLEXER_MAIN
#	ld msslKeywords.o msdlFlexLexer.o -o msdlFlexLexer

msdlBisonParser : msdlBisonParser.cpp
	g++ -I ../../lib -I ../../elements -o msdlBisonParser msdlBisonParser.cpp -DPARSER_MAIN

msdl : $(OBJ)
	g++ $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)

# clean
clean:
	rm -f $(APPL) $(OUT) $(OBJ)
