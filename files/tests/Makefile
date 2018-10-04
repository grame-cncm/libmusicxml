
files  = $(wildcard *.xml)
gmn := $(patsubst %.xml, gmn/%.gmn, $(files))

all : $(gmn)

clean :
	rm -f $(gmn)

gmn/%.gmn: %.xml
	@[ -d gmn ] || mkdir gmn
	xml2guido $< > $@


