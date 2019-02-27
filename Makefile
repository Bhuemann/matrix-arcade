


MATRIX_INC=./matrix/include
MATRIX_LIB=./matrix/lib
MATRIX_LIB_NAME = rgbmatrix

SOURCE_DIR=./src
HEADER_DIR=./headers


CC=gcc
CXX=g++
CXXFLAGS=-std=c++14 -O3
CFLAGS=-std=c11 -O3

C_SRC = $(wildcard ./src/*.c)
CXX_SRC += $(wildcard ./src/*.cpp)
OBJ = $(patsubst %.c,%.o,$(C_SRC))
OBJ += $(patsubst %.cpp,%.o,$(CXX_SRC))

LIBS=-lpthread -L$(MATRIX_LIB) -lrt -lm -l$(MATRIX_LIB_NAME)


all: matrix-library led-matrix

matrix-library:
	$(MAKE) -C $(MATRIX_LIB)


led-matrix: $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(MATRIX_INC) -o $@ $^ $(LIBS) $(shell pkg-config --cflags --libs cairo)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(MATRIX_INC) -I$(HEADER_DIR) -c -o $@ $< $(LIBS) $(shell pkg-config --cflags --libs cairo)

%.o: %.c
	$(CC) $(CFLAGS) -I$(MATRIX_INC) -I$(HEADER_DIR) -c -o $@ $<



.PHONY: clean
clean:
	rm -f ./led-matrix
	rm -fv $(OBJ)
