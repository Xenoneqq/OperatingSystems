CC=gcc
CFLAGS=-Wall -std=c17 -pthread

all: compile

compile:
	$(CC) $(CFLAGS) threads.c -o threads.o

run: compile
	./threads.o

test: compile run

clean:
	rm -f threads.o
