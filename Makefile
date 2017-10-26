
SOURCEDIR=./src
HEADERDIR=./headers
BUILDDIR=./build

CC=gcc
LIBS=-lpthread -lrt
OBJ=$(BUILDDIR)/gamepadHandler.o $(BUILDDIR)/gamepadEventHandler.o

all: dir handler

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c 
	$(CC) -c -o $@ $<

handler: $(OBJ)
	$(CC) $(LIBS) -o $(BUILDDIR)/$@ $^

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rfv $(BUILDDIR)
