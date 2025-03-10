CXX=gcc
CFLAGS=-Wall -std=c17
all: countdown

countdown:
	$(CXX) $(CFLAGS) -o countdown.exe countdown.c

test: countdown
	./countdown.exe

debug:
	$(CXX) $(CFLAGS) -g -o countdown.exe countdown.c
	gdb ./countdown.exe

clean:
	rm countdown.exe

.PHONY: clean all