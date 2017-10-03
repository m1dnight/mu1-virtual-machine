# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
FLAGS=-Wall -g
CC=gcc

# Typing `make` in the console will execute the `all` rule.
default: mu1vm

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

mu1vm: mu1vm.o debug.o  endianness.o memory.o conditionals.o
	$(CC) $(FLAGS) -o mu1vm mu1vm.o debug.o endianness.o memory.o conditionals.o


.PHONY: clean

clean:
	-rm -f *.o
	-rm -f mu1vm
