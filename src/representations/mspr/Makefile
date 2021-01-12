
APPL = mspl
OUT  = mspllex.c++ msplparse.cpp msplparse.hpp
OBJ  = mspllex.o msplparse.o
CXXFLAGS = -I.. -DMAIN
YACC = bison
LEX = flex --trace
PREFIX := libmmspl

code : $(OUT)

mspl : $(OBJ)
	gcc $(DEBUG) $(OBJ) -lstdc++ -o $(APPL)

msplparse.cpp : mspl.y
	$(YACC) -d -o msplparse.cpp  -p $(PREFIX) mspl.y

msplparse.hpp : mspl.y
	$(YACC) -d -o msplparse.cpp  -p $(PREFIX) mspl.y

mspllex.c++ : mspl.l msplparse.hpp
	$(LEX)  -omspllex.c++ -I -P$(PREFIX) mspl.l

clean:
	rm -f $(APPL) $(OUT) $(OBJ)
