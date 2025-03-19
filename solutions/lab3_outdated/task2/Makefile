CXX=gcc
CFLAGS=-Wall -std=c17

all: test

compile:
	$(CXX) $(CFLAGS) exercise.c -D_XOPEN_SOURCE=700 -o exercise.exe

run:
	./exercise.exe

clean:
	rm -rf exercise.exe

test: compile run