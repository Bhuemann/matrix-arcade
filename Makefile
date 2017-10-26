
SOURCEDIR=./src
HEADERDIR=./headers
BUILDDIR=./build

RGB_LIB_DISTRIBUTION=./rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

CC=gcc
CXX=g++
CXXFLAGS=-std=c++14
LIBS=-lpthread -L$(RGB_LIBDIR) -lrt -lm -l$(RGB_LIBRARY_NAME)
OBJ=$(BUILDDIR)/menu.o $(BUILDDIR)/gamepadEventHandler.o $(BUILDDIR)/gamepadHandler.o

all: dir $(OBJ) gameSystem

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

$(BUILDDIR)/menu.o: $(SOURCEDIR)/menu.cc $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -I$(RGB_INCDIR) -c -o $@ $< $(LIBS)

$(BUILDDIR)/gamepadHandler.o: $(SOURCEDIR)/gamepadHandler.c
	$(CC) -c -o $@ $<

$(BUILDDIR)/gamepadEventHandler.o: $(SOURCEDIR)/gamepadEventHandler.c
	$(CC) -c -o $@ $<

#$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cc
#	$(CXX) $(CXXFLAGS) -fpermissive -c -o $@ $<

gameSystem: $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(RGB_INCDIR) -o $(BUILDDIR)/$@ $^ $(LIBS)

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rfv $(BUILDDIR)

FORCE:
.PHONY: FORCE
