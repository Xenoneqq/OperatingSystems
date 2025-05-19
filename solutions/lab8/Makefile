CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	./printer

test: compile run

clean:
	rm -rf printer

compile:
	$(CXX) $(CFLAGS) printer.c -o printer -lrt -pthread