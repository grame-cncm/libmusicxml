#
# libmusicxml Javascript
# This makefile is intended to compile the libmusicxml library as a javascript library
# It is based on emscript that must be available from the command line
# see http://kripken.github.io/emscripten-site/ for more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#
# The TOTAL_MEMORY is the maximum memory allocated by emscripten. This value is specific (100663296) to be compliant with the asm.js and prevent warning at execution.
# It can be increased if error occured.
# 
# Compilation option -Oz is discouraged, it provoke undefined results and assertion errors at runtime.
#

srcdir := ../../src
folders := factory guido lib parser elements files interface operations	visitors
subprojects :=  $(patsubst %, $(srcdir)/%, $(folders))
sources = $(wildcard $(srcdir)/[^a]*/*.cpp src/*/*.cpp)		# antescofo folder ignored by [^a]*
objects = $(patsubst $(srcdir)%, obj%, $(patsubst %.cpp,%.o,$(sources)))

GCC = emcc
CXXFLAGS += -std=c++11  $(addprefix -I, $(subprojects)) -O3

library = libmusicxml
target = ../$(library).js

##############################################################
# building the library
##############################################################
$(target) : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=0 -s ASM_JS=1 -s TOTAL_MEMORY=100663296 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0

clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

##############################################################
# rules for building the library
##############################################################
obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/parser/xmlparse.o: $(srcdir)/parser/xmlparse.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c -Wno-deprecated-register $(CXXFLAGS) $< -o $@
