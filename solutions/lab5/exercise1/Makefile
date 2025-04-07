CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	./exercise.exe ignore

test: compile
	./exercise.exe ignore
	./exercise.exe handle
	./exercise.exe mask

clean:
	rm -rf exercise.exe

compile:
	$(CXX) $(CFLAGS) exercise.c -o exercise.exe