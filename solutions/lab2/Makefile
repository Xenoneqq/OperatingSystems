CXX=gcc
CFLAGS=-Wall -std=c17

all: clean collatz-static collatz-shared collatz-dynamic

collatz-static:
	$(CXX) $(CFLAGS) -c collatz.c -o collatz.o
	ar rcs libcollatz.a collatz.o
	$(CXX) $(CFLAGS) -o static.o -c main.c
	$(CXX) $(CFLAGS) -o static.exe static.o libcollatz.a

collatz-shared:
	$(CXX) $(CFLAGS) -shared -o libcollatz.so collatz.c
	$(CXX) $(CFLAGS) -o shared.exe main.c -L. libcollatz.so

collatz-dynamic:
	$(CXX) $(CFLAGS) -shared -o libcollatz.so -fPIC collatz.c
	$(CXX) $(CFLAGS) -o dynamic.exe main.c -ldl -DUSE_DYNAMIC

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