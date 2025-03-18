CXX=gcc
CFLAGS=-Wall -std=c17

all: character 

character:
	$(CXX) $(CFLAGS) exercise.c -lrt -o exercise.exe

chunk:
	$(CXX) $(CFLAGS) exercise.c -lrt -o exercise.exe -DUSE_CHUNK

run:
	./exercise.exe test_og.txt test_res.output

clean:
	rm -rf exercise.exe
	rm -rf test_res.output

test: character
	./exercise.exe test_og.txt test_res.output
	
test-character: character run

test-chunk: chunk run