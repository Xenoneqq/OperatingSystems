all: compile

test: compile run

compile:
	gcc action.c -o action.o
	gcc signals.c -o signals.o

run:
	./signals.o
	./action.o

clean:
	rm -rf signals.o
	rm -rf action.o