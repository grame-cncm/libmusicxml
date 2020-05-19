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

LILY     ?= yes
BRAILLE  ?= yes
MSR     := no
srcdir  := ../src
binddir := ./src
npmdir  := ./npm

ifeq ($(LILY), yes)
	MSR = yes
endif
ifeq ($(BRAILLE), yes)
	MSR = yes
endif

folders := factory guido lib parser elements elements/xml files interface operations	visitors
ifeq ($(MSR), yes)
	folders := $(folders) utilities musicxml msr oah
endif

ifeq ($(LILY), yes)
	folders := $(folders) lpsr lilypond
endif
ifeq ($(BRAILLE), yes)
	folders := $(folders) bsr braille
endif

sources := $(foreach dir,$(folders), $(wildcard $(srcdir)/$(dir)/*.cpp))
subprojects :=  $(patsubst %, $(srcdir)/%, $(folders))
bindsrc = $(wildcard $(binddir)/*.cpp) 
objects = $(patsubst $(srcdir)%, obj%, $(patsubst %.cpp,%.o,$(sources)) $(patsubst $(binddir)%.cpp, obj/binding%.o,$(bindsrc)))

GCC = emcc
CXXFLAGS += -std=c++11  -DXML $(addprefix -I, $(subprojects)) -O3

library = libmusicxml

OPTIONS :=  -s ALLOW_MEMORY_GROWTH=0 -s TOTAL_MEMORY=134217728 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0 -s MODULARIZE=1 -s EXPORT_NAME="'MusicXMLModule'"

target = $(library).js

##############################################################
all :
	make $(target)

##############################################################
help :
	@echo "libmusicxml wasm library generation"
	@echo "   all (default): build the wasm library,"	
	@echo "   clean        : remove output and object files,"	
	@echo "   depend       : recompute dependencies,"	
	@echo "   npmlib       : update lib in npm"	
	@echo "   test         : run npm test"	
	@echo "   npm          : publish to npm (use with caution),"	
	@echo "                  see the npm folder,"	
	@echo 	
	@echo "Options:"
	@echo "   LILY      include lilypond part when set to 'yes' (default is $(LILY))"
	@echo "   BRAILLE   include braille part when set to 'yes' (default is $(BRAILLE))"

dbg:
	@echo $(objects)

##############################################################
# publishing to npm
##############################################################
npm: $(npmdir)/$(library).js
	npm publish $(npmdir) --access public

test: $(npmdir)/$(library).js
	cd npm && npm run test

npmlib: $(npmdir)/$(library).js

$(npmdir)/$(library).js : $(library).js
	cp $(library).js   $(npmdir)
	cp $(library).wasm $(npmdir)


##############################################################
# building the library as web assembly
##############################################################
$(library).js : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $@ $(OPTIONS) -s WASM=1

clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

##############################################################
# rules for building the library
##############################################################
obj/binding/%.o: $(binddir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) -I../../src/interface $< -o $@

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@


# DO NOT DELETE
