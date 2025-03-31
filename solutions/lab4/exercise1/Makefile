CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	./program.exe 10

test: compile run

clean:
	rm -rf program.exe

compile:
	$(CXX) $(CFLAGS) program.c -o program.exe