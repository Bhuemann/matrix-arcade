
SOURCEDIR=./src
HEADERDIR=./headers
BUILDDIR=./build

all: dir gamepadEventHandler.c

gamepadEventHandler.c:
	gcc -I$(HEADERDIR) -c $(SOURCEDIR)/gamepadEventHandler.c -o $(BUILDDIR)/gamepadEventHandler.o

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rfv $(BUILDDIR)
