
system	 := $(shell uname -s)

subprojects := elements interface files lib parser visitors guido operations

SRC = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJ = $(SRC:.cpp=.o)
ifeq ($(system), Darwin)
LIB = -F../build -framework libmusicxml2 -lc++
else
LIB = -L../build -lmusicxml2
endif
CXXFLAGS := -stdlib=libc++ -O3 -Wall -Wno-overloaded-virtual -Wuninitialized $(addprefix -I../src/, $(subprojects))
INSTALLDIR := $(HOME)/bin

applications := xmlversion countnotes xmlread xmlclone xmliter xml2guido xml2midi readunrolled randomMusic xmltranspose partsummary

all : $(applications)

countnotes: countnotes.cpp 
	gcc $(CXXFLAGS) countnotes.cpp $(LIB) -o countnotes

xmlread: xmlread.cpp 
	gcc $(CXXFLAGS) xmlread.cpp $(LIB) -o xmlread

xmlclone: xmlclone.cpp 
	gcc $(CXXFLAGS) xmlclone.cpp $(LIB) -o xmlclone

xmliter : xmliter.cpp
	gcc $(CXXFLAGS) xmliter.cpp $(LIB) -o xmliter

xml2guido: xml2guido.cpp 
	gcc $(CXXFLAGS) xml2guido.cpp $(LIB) -o xml2guido

xml2midi: xml2midi.cpp 
	gcc $(CXXFLAGS) xml2midi.cpp $(LIB) -o xml2midi

readunrolled: readunrolled.cpp 
	gcc $(CXXFLAGS) readunrolled.cpp $(LIB) -o readunrolled

randomMusic: RandomMusic.cpp 
	gcc $(CXXFLAGS) RandomMusic.cpp $(LIB) -o randomMusic

xmltranspose: xmltranspose.cpp 
	gcc $(CXXFLAGS) xmltranspose.cpp $(LIB) -o xmltranspose

partsummary: partsummary.cpp 
	gcc $(CXXFLAGS) partsummary.cpp $(LIB) -o partsummary

xmlversion: xmlversion.cpp
	gcc $(CXXFLAGS) xmlversion.cpp $(LIB) -o xmlversion

clean :
	rm -f $(applications) $(OBJ)
	rm -rf *.dSYM

install :
	cp $(applications) $(INSTALLDIR)

dep:
	makedepend -- $(CXXFLAGS) -- $(SRC)

# DO NOT DELETE
