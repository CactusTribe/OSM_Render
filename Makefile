all:
	gcc -Wall -L ./lib -I ./include ./src/main.c -o ./bin/main -lSDL2 -lxml2

run:
	./bin/main

clean:
	rm -f ./main

#all: draw.o init.o main.o
#	gcc -lSDL2-2.0 -g ./bin/*.o -L ./lib -I ./include  -o main

#	gcc ./bin/*.o -o blabla  -lSDL2 

#draw.o: ./src/draw.c
#	gcc -lSDL2-2.0 -c ./src/draw.c -o ./bin/draw.o 

#init.o: ./src/init.c 
#	gcc -lSDL2-2.0 -c ./src/init.c -o ./bin/init.o 

#main.o: ./src/main.c
#	gcc -lSDL2-2.0 -c ./src/main.c -o ./bin/main.o 





#	gcc -c `sdl-config --cflags` ./src/main.c -o ./bin/main.o 
