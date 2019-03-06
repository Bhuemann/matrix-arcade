


MATRIX_ROOT= ../../../matrix
MATRIX_INC=$(MATRIX_ROOT)/include
MATRIX_LIB=$(MATRIX_ROOT)/lib
MATRIX_LIB_NAME = rgbmatrix

MENU_ROOT=../../..
MENU_INC=$(MENU_ROOT)/headers

SOURCE_DIR=./src
HEADER_DIR=./inc

CXXFLAGS=-std=c++14 -g -Og
CFLAGS=-std=c11 -g -Og

LIBS= -lpthread -L$(MATRIX_LIB) -lrt -lm -l$(MATRIX_LIB_NAME)

all:
	g++ $(CXXFLAGS) -I$(MATRIX_INC) -I$(MENU_INC) -I$(HEADER_DIR) -c -o ./src/snake.o ./src/snake.cpp 
	g++ $(CXXFLAGS) -o Snake ./src/snake.o $(LIBS)

