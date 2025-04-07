CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

run:
	echo The program requires two consoles to run
	echo Use the following console commands ./catcher.exe ./sender.exe

test:
	echo two terminals requried open them and run it

clean:
	rm -rf catcher.exe
	rm -rf sender.exe

compile:
	$(CXX) $(CFLAGS) catcher.c -o catcher.exe
	$(CXX) $(CFLAGS) sender.c -o sender.exe