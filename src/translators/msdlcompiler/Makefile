
APPL = msdl

OUT  = msdlLex.cpp msdlParse.cpp msdlParse.hpp
OBJ  = msdlLex.o msdlParse.o
EXE  = msdl

CXXFLAGS = -I.. -DMAIN -DLEX_ONLY

LEX = flex   # --trace --verbose # --debug

YACC = bison # -Wcounterexamples

PREFIX := msdl

code : $(EXE)

msdlLex.cpp : msdl.l msdlParse.hpp
	$(LEX)     --header-file=msdlLex.hpp -o msdlLex.cpp -I -P$(PREFIX) msdl.l

msdlParse.cpp : msdl.y
	$(YACC) -d -o msdlParse.cpp  -p $(PREFIX) msdl.y

msdlParse.hpp : msdl.y
	$(YACC) -d -o msdlParse.cpp  -p $(PREFIX) msdl.y

msdl : $(OBJ)
	g++ $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)

clean:
	rm -f $(APPL) $(OUT) $(OBJ)
