CXX=gcc
CFLAGS=-Wall -std=c17

all: collatz-static main

collatz-static:
	gcc -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	gcc -c main.c -o main.o
	gcc main.o libcollatz.a -o main.exe

# You might need to input the last line yourself into terminal
collatz-shared:
	gcc -shared -o libcollatz.so collatz.c
	gcc -o main.exe main.c -L. -lcollatz
	export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

clean:
	rm -f collatz.o
	rm -f libcollatz.a
	rm -f main.o
	rm -f main.exe
	rm -f libcollatz.so