### Hey, Emacs, ride the -*- Makefile -*- mode

## Author: petr.jelen@gmail.com
## Keywords: 

## definitions

LDLIBS = $(shell pkg-config --libs sword)

CXXFLAGS += $(shell pkg-config --cflags sword)

## static rules

.PHONY : all

all : mod2tex

.PHONY : clean

clean :
	-rm -f *~ mod2tex

### Makefile ends here
