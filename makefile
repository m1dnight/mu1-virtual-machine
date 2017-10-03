FLAGS=-Wall -I .
CC=gcc
DEPS=debug.h endianness.h
OBJ=debug.o endianness.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mu1vm: $(OBJ)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f mu1vm
