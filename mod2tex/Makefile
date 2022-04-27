### Hey, Emacs, ride the -*- Makefile -*- mode

## Author: petr.jelen@gmail.com
## Keywords: 

## definitions

CXX = g++
LD = g++
LIBS = -lsword
LDFLAGS = $(LIBS)
CXXFLAGS = -I/usr/include/sword

target = mod2tex
objects = mod2tex.o

## static rules

.PHONY : all

all : $(target)

$(target) : $(objects)
	$(LD) -o $@ $< $(LDFLAGS)

$(objects) : %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY : clean

clean :
	-rm -f *~ $(objects) $(target)

## pattern rules

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $<

### Makefile ends here
