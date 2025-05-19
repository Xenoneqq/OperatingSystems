all: compile

clean:
	rm -rf client.exe
	rm -rf server.exe

compile:
	gcc client.c -o client.exe
	gcc server.c -o server.exe

run:
	./server.exe
	./client.exe

test: compile run