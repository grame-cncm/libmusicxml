
dtddir := ../../../dtds
dtds := $(wildcard $(dtddir)/[abcdilnpst]*.mod)
elts  := elements.txt
eltname := '[-a-zA-Z0-9_.:]*'

SRC := $(wildcard *.cpp) 
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all : $(elts) elements.h typedefs.h factory.cpp

test = xmltest

elements : $(elts)

$(test): $(OBJ)
	gcc $(OBJ) -lstdc++ -o $(TARGET)

$(elts) : $(dtds)
	grep  \<\!ELEMENT $(dtds) | \
	sed -e 's/\(..*\.mod\):<\!ELEMENT[ 	]*\($(eltname)\)[ 	]*.*/\2	\/\/ \1/' > $(elts)

elements.h : templates/elements.txt $(elts)
	templates/elements $(elts) constants > elements.h

factory.cpp : templates/factory.txt $(elts)
	templates/elements $(elts) map > factory.cpp

typedefs.h :  templates/typedefs.txt $(elts)
	templates/elements $(elts) types > typedefs.h

eltsclean :
	rm -f $(elts) elements.h factory.cpp typedefs.h

clean :
	rm -f $(test) $(OBJ)

dep:
	makedepend -- $(CXXFLAGS) -- $(SRC)
