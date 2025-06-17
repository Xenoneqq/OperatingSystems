CC=gcc
CFLAGS=-Wall -std=c17 -pthread

all: compile

compile:
	$(CC) $(CFLAGS) hospital.c -o hospital.o

run: compile
	./hospital.o 20 3

test: compile run

clean:
	rm -rf hospital.o
