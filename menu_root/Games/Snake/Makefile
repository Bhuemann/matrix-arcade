


MATRIX_ROOT := ../../../matrix
MATRIX_INC := $(MATRIX_ROOT)/include
MATRIX_LIB := $(MATRIX_ROOT)/lib
MATRIX_LIB_NAME := rgbmatrix

MENU_ROOT := ../../..
MENU_INC := $(MENU_ROOT)/headers

SOURCE_DIR := ./src
HEADER_DIR := ./inc

CXXFLAGS := -std=c++14 -g -Og
CFLAGS := -std=c11 -g -Og

LIBS := -lpthread -L$(MATRIX_LIB) -lrt -lm -l$(MATRIX_LIB_NAME)


# macro with all the source names
SRCS := ./src/snake.cpp

# macro with all the object file names
OBJS := $(SRCS:.cpp=.o)

# always include a .PHONY statement for any targets
# that do not produce a file of the same name
.PHONY : all clean

# 'all' is the typical first target
all: snake

# implied compile rule/recipe
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -I$(MATRIX_INC) -I$(MENU_INC) -I$(HEADER_DIR) -c -o $@ $<


snake: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)


clean:
	rm $(OBJS)
	rm snake
