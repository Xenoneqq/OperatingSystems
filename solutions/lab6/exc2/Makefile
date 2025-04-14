CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	./sender.o

test: compile run

clean:
	rm -rf riemann.o
	rm -rf sender.o

compile:
	$(CXX) $(CFLAGS) riemann.c -o riemann.o
	$(CXX) $(CFLAGS) sender.c -o sender.o