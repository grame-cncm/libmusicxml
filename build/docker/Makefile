#
# tools to build and use a docker image with libmusicxml
#

VERSION := 3.1.4
NAME    := libmusicxml:$(VERSION)

all:
	@echo building $(NAME) docker image...
	docker build -t $(NAME) .

list:
	docker images

run:
	docker run -t -i $(NAME) bash

xml2guido: XML  ?= sample.gmn
xml2guido: DIR  := $(shell dirname $(XML))
xml2guido: FILE := $(shell basename $(XML))
xml2guido: OUT  := $(shell basename $(FILE) .xml).gmn
xml2guido:
	docker run -t -w /grame/shared -v $(DIR):/grame/shared $(NAME) xml2guido -o $(OUT) $(FILE)

rmstopped:
	@echo remove all stopped containers
	docker rm $(shell docker ps -a -q)

clean:
	docker image rm -f libmusicxml:$(VERSION)
