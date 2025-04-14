all: compile

test: compile run

compile:
	gcc sorter.c -o sorter.o

run:
	./sorter.o

clean:
	rm -rf sorter.o
	rm -rf sorted.txt