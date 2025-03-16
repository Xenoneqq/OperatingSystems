CXX=gcc
CFLAGS=-Wall -std=c17

all: clean collatz-static

collatz-static:
	gcc -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	gcc -c main.c -o main.o
	gcc main.o libcollatz.a -o main.exe

# Input the last line yourself
collatz-shared:
	gcc -shared -o libcollatz.so collatz.c
	gcc -o main.exe main.c -L. -lcollatz
	echo use this command before running...
	echo export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

collatz-dynamic:
	gcc -shared -o libcollatz.so -fPIC collatz.c
	gcc -o main.exe main.c -ldl -DUSE_DYNAMIC
	echo use this command before running...
	echo export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

clean:
	rm -f collatz.o
	rm -f libcollatz.a
	rm -f main.o
	rm -f main.exe
	rm -f libcollatz.so