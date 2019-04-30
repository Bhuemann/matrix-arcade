


all: gamepad_example

gamepad_example:
	@echo "Make sure to have gamepadEventHandler.o and gamepadHandler.o binaries compiled with root makefile"
	gcc -g -Og -I../matrix/include -I../headers -o gamepad_example gamepad_example.c ../src/gamepadHandler.o ../src/gamepadEventHandler.o -lpthread -lrt
