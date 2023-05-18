CPP = g++
AR = ar
CPPFLAGS = -std=c++11 -O2 -fpic -g -Wall -Wno-unused

INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib

OBJECT = tgcpp.o
STATIC = libtgcpp.a
DYNAMIC = libtgcpp.so

all: $(STATIC) $(DYNAMIC)

$(OBJECT): tgcpp.cpp tgcpp.h
	$(CPP) -c $(CPPFLAGS) tgcpp.cpp

$(DYNAMIC): $(OBJECT)
	$(CPP) -shared -o $@ $^

$(STATIC): $(OBJECT)
	$(AR) rcs $@ $^

install:
	cp -f $(STATIC) $(LIBDIR)
	cp -f tgcpp.h $(INCDIR) 

clean:
	rm -rf $(STATIC) $(DYNAMIC) $(OBJECT)
