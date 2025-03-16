CXX=gcc
CFLAGS=-Wall -std=c17

all: clean collatz-static collatz-shared collatz-dynamic

collatz-static:
	gcc -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	gcc -o static.o -c main.c
	gcc -o static.exe static.o libcollatz.a

collatz-shared:
	gcc -shared -o libcollatz.so collatz.c
	gcc -o shared.exe main.c -L. libcollatz.so

collatz-dynamic:
	gcc -shared -o libcollatz.so -fPIC collatz.c
	gcc -o dynamic.exe main.c -ldl -DUSE_DYNAMIC

clean:
	rm -f collatz.o
	rm -f libcollatz.a
	rm -f main.o
	rm -f static.o
	rm -f static.exe
	rm -f shared.exe
	rm -f dynamic.exe
	rm -f libcollatz.so

make test:
	./static.exe
	./shared.exe
	./dynamic.exe