CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	./riemann.o 1
	./riemann.o 10
	./riemann.o 100
	./riemann.o 1000

test: compile run

clean:
	rm -rf riemann.o

compile:
	$(CXX) $(CFLAGS) riemann.c -o riemann.o