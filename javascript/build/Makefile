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
binddir := ../src
folders := factory guido lib parser elements files interface operations	visitors
subprojects :=  $(patsubst %, $(srcdir)/%, $(folders))
sources = $(wildcard $(srcdir)/[^a]*/*.cpp )    # reject atescofo folder
bindsrc = $(wildcard $(binddir)/*.cpp) 
objects = $(patsubst $(srcdir)%, obj%, $(patsubst %.cpp,%.o,$(sources)) $(patsubst $(binddir)%.cpp, obj/binding%.o,$(bindsrc)))

GCC = emcc
CXXFLAGS += -std=c++11  $(addprefix -I, $(subprojects)) -O3

library = libmusicxml

OPTIONS :=  -s ALLOW_MEMORY_GROWTH=0 -s TOTAL_MEMORY=134217728 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0 -s MODULARIZE=1 -s EXPORT_NAME="'MusicXMLModule'"

WASM ?= no
ifeq ($(WASM), yes)
	target = ../$(library).wasm.js
else
	target = ../$(library).js
endif

all :
	make $(target)
	
##############################################################
# building the library as asm.js
##############################################################
../$(library).js : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $(target) $(OPTIONS) -s ASM_JS=1

##############################################################
# building the library as web assembly
##############################################################
../$(library).wasm.js : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $(target) $(OPTIONS) -s WASM=1


clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

##############################################################
# rules for building the library
##############################################################
#obj/binding/%.o: ../src/%.cpp
obj/binding/%.o: $(binddir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) -I../../src/interface $< -o $@

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/parser/xmlparse.o: $(srcdir)/parser/xmlparse.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c -Wno-deprecated-register $(CXXFLAGS) $< -o $@

# DO NOT DELETE
