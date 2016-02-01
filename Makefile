all:
	gcc -Wall -L ./lib -I ./include ./src/main.c -o ./bin/main -lSDL2 -lxml2

run:
	./bin/main

clean:
	rm -f ./bin/*
