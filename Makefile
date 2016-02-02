CC = gcc
CFLAGS = -g -Wall

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBFLAGS = -lSDL2 -lxml2
endif
ifeq ($(UNAME_S),Darwin)
	LIBFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf
endif

all:
	$(CC) $(CFLAGS) -L ./lib -I ./include ./src/main.c -o ./bin/main $(LIBFLAGS)

run:
	./bin/main

clean:
	rm -f ./bin/*
