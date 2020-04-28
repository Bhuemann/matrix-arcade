
JSON_INC=./json/single_include/nlohmann
MATRIX_INC=./matrix/include
MATRIX_LIB=./matrix/lib
MATRIX_LIB_NAME = rgbmatrix

SOURCE_DIR=./src
HEADER_DIR=./headers


export CC=gcc
export CXX=g++
CXXFLAGS=-std=c++14 -g -Og
CFLAGS=-std=c11 -g -Og

C_SRC := $(wildcard ./src/*.c)
CXX_SRC += $(wildcard ./src/*.cpp)
OBJ := $(patsubst %.c,%.o,$(C_SRC))
OBJ += $(patsubst %.cpp,%.o,$(CXX_SRC))


LIBS = -lpthread -L$(MATRIX_LIB) -lrt -lm -l$(MATRIX_LIB_NAME)


all: matrix-library json matrix-arcade

matrix-library:
	$(MAKE) -C $(MATRIX_LIB)
	
json:
	mkdir -p json/build
	cmake -S json/ -B json/build
	$(MAKE) -C json/

matrix-arcade: $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(MATRIX_INC) -I$(HEADER_DIR) -I$(JSON_INC) -o $@ $^ $(LIBS) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(MATRIX_INC) -I$(HEADER_DIR) -I$(JSON_INC) -c -o $@ $< 

%.o: %.c
	$(CC) $(CFLAGS) -I$(MATRIX_INC) -I$(HEADER_DIR) -c -o $@ $<



.PHONY: clean
clean:
	rm -f ./led-matrix
	rm -fv $(OBJ)
