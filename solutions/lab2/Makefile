CXX=gcc
CFLAGS=-Wall -std=c17

all: clean collatz-static

collatz-static:
	gcc -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	gcc -o main.o -c main.c
	gcc -o main.exe main.o libcollatz.a

# Input the last line yourself
collatz-shared:
	gcc -shared -o libcollatz.so collatz.c
	gcc -o main.exe main.c -L. libcollatz.so

collatz-dynamic:
	gcc -shared -o libcollatz.so -fPIC collatz.c
	gcc -o main.exe main.c -ldl -DUSE_DYNAMIC

clean:
	rm -f collatz.o
	rm -f libcollatz.a
	rm -f main.o
	rm -f main.exe
	rm -f libcollatz.so