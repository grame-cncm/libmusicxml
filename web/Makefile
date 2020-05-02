
MAKE ?= make

DIST := ../docs
FONTDIR := $(DIST)/font
CSSDIR  := $(DIST)/css
TSFOLDER := src
TSLIB	 := $(TSFOLDER)/lib
LXMLTS  := libmusicxml.ts libmusicxml.d.ts
TSFILES := $(wildcard src/*.ts) $(wildcard src/*.js) $(LXMLTS:%=$(TSLIB)/%)

CSS    := css/lxmlweb.css

LIBOUT   := extern.min.js
CSSOUT   := bootstrap.min.css
OUT      := $(LIBOUT:%=$(DIST)/lib/%) $(CSSOUT:%=$(DIST)/css/%)
GUIDOLIB := $(DIST)/lib/libGUIDOEngine.js
LXMLLIB  := $(DIST)/lib/libmusicxml.js
LXMLNODE := node_modules/@grame/libmusicxml

.PHONY: css

all:
	$(MAKE) tslibs
	$(MAKE) libs
	$(MAKE) ts
	$(MAKE) css
	$(MAKE) readme
#	git checkout $(DIST)/CNAME


###########################################################################
help:
	@echo "============================================================"
	@echo "                   Online libmusicxml tools"
	@echo "============================================================"
	@echo "Available targets are:"
	@echo "  all (default): generates the site into $(DIST)"
	@echo "  clean        : remove the minified files"
	@echo "========== Development targets"
	@echo "  ts           : build the typescript version"
	@echo "  tslibs       : update $(TSLIB) folder from nodes_modules"
	@echo "  libs         : update $(DIST)/lib folder from nodes_modules"
	@echo "  css          : update $(DIST)/css folder from nodes_modules"
	@echo "  readme       : generates README.html from README.md"
	@echo "============================================================"


###########################################################################
ts : $(TSLIB) $(DIST)/lxmlconverter.js

$(DIST)/lxmlconverter.js : $(TSFILES)
	cd $(TSFOLDER) && tsc

tslibs: $(TSLIB)
	cp $(LXMLNODE)/libmusicxml.ts $(TSLIB)
	cp $(LXMLNODE)/libmusicxml.d.ts $(TSLIB)

libs: $(DIST)/lib
	cp node_modules/jquery/dist/jquery.min.js $(DIST)/lib
	cp node_modules/bootstrap/dist/js/bootstrap.min.js $(DIST)/lib
	cp $(LXMLNODE)/libmusicxml.js 		$(DIST)/lib
	cp $(LXMLNODE)/libmusicxml.wasm 	$(DIST)/lib

$(TSLIB):
	mkdir $(TSLIB)

$(DIST)/lib:
	mkdir $(DIST)/lib

test:
	@echo $(TSFILES)
	
css:
	cp node_modules/bootstrap/dist/css/bootstrap.min.css* $(DIST)/css/ 

readme:
	$(MAKE) README.html

README.html : README.md
	echo "<!DOCTYPE html><html><xmp>" > README.html
	cat README.md  >> README.html
	echo "</xmp> <script src=http://strapdownjs.com/v/0.2/strapdown.js></script> </html>"  >> README.html

###########################################################################	
clean :
	rm -f $(TSLIB)/*
	rm -f $(DIST)/lib/*
	rm -f $(DIST)/css/bootstrap.*
	rm -f $(DIST)/lxmlconverter.js
	