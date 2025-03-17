CXX=gcc
CFLAGS=-Wall -std=c17

all: compile

compile:
	$(CXX) $(CFLAGS) exercise.c -lrt -o exercise.exe
	
run:
	./exercise.exe test_og.txt test_res.output

clean:
	rm -rf exercise.exe
	rm -rf test_res.output

test: compile
	./exercise.exe test_og.txt test_res.output