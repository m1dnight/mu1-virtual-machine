FLAGS=-Wall

default: main

main.o: main.c 
	gcc $(FLAGS) -c main.c -o main.o

main: main.o
	gcc $(FLAGS) main.o -o main

clean:
	-rm -f main.o
	-rm -f main
