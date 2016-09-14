
files  = $(wildcard *.xml)
gmn := $(patsubst %.xml, %.gmn, $(files))
XML2GUIDO := xml2guido
#XML2GUIDO := ../../../build/macos/Release/xml2guido


all : $(gmn)

clean :
	rm -f $(gmn)

%.gmn: %.xml
	$(XML2GUIDO) $< > $@

