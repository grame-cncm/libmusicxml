
APPL = msdl

OUT  = msdlFlexLexer.h msdlFlexLexer.cpp msdlParser.hpp msdlParser.cpp msdlFlexLexer
OBJ  = msdlFlexLexer.o msdlParser.o

target : $(OUT)

# flex
FLEX = flex # --c++  # --trace --verbose # --debug

msdlFlexLexer.h : msdl.ll msdlParser.hpp
	flex -+ -o msdlFlexLexer.cpp msdl.ll

msdlFlexLexer.cpp : msdl.ll msdlParser.hpp
	flex -+ -o msdlFlexLexer.cpp msdl.ll

# bison
BISON = bison # -Wcounterexamples
PREFIX = msdl

msdlParser.cpp : msdl.y
	$(BISON) -d -o msdlParser.cpp  -p $(PREFIX) msdl.y

msdlParser.hpp : msdl.y
	$(BISON) -d -o msdlParser.cpp  -p $(PREFIX) msdl.y

# g++
CXXFLAGS = -I.. -DMAIN -DLEX_ONLY

msdlFlexLexer : msdlFlexLexer.cpp
	g++ -o msdlFlexLexer msdlFlexLexer.cpp -DMAIN

msdl : $(OBJ)
	g++ $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)

# clean
clean:
	rm -f $(APPL) $(OUT) $(OBJ)
