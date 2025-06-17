CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

test: compile

clean:
	rm -rf server.exe
	rm -rf client.exe

compile:
	$(CXX) $(CFLAGS) client.c -o client.exe
	$(CXX) $(CFLAGS) server.c -o server.exe