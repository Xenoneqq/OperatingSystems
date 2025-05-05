CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:


test: compile run

clean:
	rm -rf client.exe
	rm -rf server.exe

compile:
	$(CXX) $(CFLAGS) client.c -o client.exe
	$(CXX) $(CFLAGS) server.c -o server.exe