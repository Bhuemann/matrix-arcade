
SOURCEDIR=./src
HEADERDIR=./headers
BUILDDIR=./build

all: dir gamepadHandler.c

gamepadHandler.c: gamepadEventHandler.c
	gcc -I$(HEADERDIR) -c $(SOURCEDIR)/gamepadHandler.c -o $(BUILDDIR)/gamepadHandler.o

gamepadEventHandler.c:
	gcc -I$(HEADERDIR) -c $(SOURCEDIR)/gamepadEventHandler.c -o $(BUILDDIR)/gamepadEventHandler.o

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rfv $(BUILDDIR)
