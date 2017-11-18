SOURCEDIR=./src
HEADERDIR=./headers
BUILDDIR=./build
FONTSDIR=./fonts

RGB_LIB_DISTRIBUTION=./matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

CC=gcc
CXX=g++
CXXFLAGS=-std=c++14 -Os
CFLAGS=-std=c11 -Os
LIBS=-lpthread -L$(RGB_LIBDIR) -lrt -lm -l$(RGB_LIBRARY_NAME)
OBJ=$(BUILDDIR)/menu.o $(BUILDDIR)/led_matrix.o $(BUILDDIR)/gamepadEventHandler.o $(BUILDDIR)/gamepadHandler.o $(BUILDDIR)/fonts.o

all: dir gameSystem

$(BUILDDIR)/led_matrix.o: $(SOURCEDIR)/led_matrix.cpp $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -I$(RGB_INCDIR) -c -o $@ $< $(LIBS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

gameSystem: $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(RGB_INCDIR) -o $(BUILDDIR)/$@ $^ $(LIBS)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

FORCE:
.PHONY: FORCE

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rfv $(BUILDDIR)
	rm -fv $(RGB_LIBDIR)/*.o
