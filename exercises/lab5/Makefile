
all: compile

test: compile run

run:
	./sigself.exe
	./sigtest.exe

compile:
	gcc sigself.c -o sigself.exe
	gcc sigtest.c -o sigtest.exe

clean:
	rm -rf sigself.exe
	rm -rf sigtest.exe