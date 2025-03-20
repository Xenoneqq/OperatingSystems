CXX=gcc
CFLAGS=-Wall -std=c17

all: clean compile run

make: compile

test: compile run

clean:
	rm -rf ./res/isthisprogramming.txt
	rm -rf ./res/notsure.txt
	rm -rf ./res/pikachu.txt
	rm -rf res
	rm -rf exercise.exe

compile:
	$(CXX) $(CFLAGS) exercise.c -o exercise.exe

run:
	./exercise.exe