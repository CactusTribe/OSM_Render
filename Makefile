CC = gcc
CFLAGS = -g -Wall 

LIB32= ./lib
LIB64= ./lib64
INCLUDE = ./include

# librairie en fonction de l'architecture
ARCH := $(shell getconf LONG_BIT)
ifeq ($(ARCH),64)
	CFLAGS+= -L $(LIB64)
else
	CFLAGS+= -L $(LIB32)
endif

CFLAGS+= -I $(INCLUDE)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBFLAGS = -lSDL2 -lxml2
endif
ifeq ($(UNAME_S),Darwin)
	LIBFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf
endif


all:
	$(CC) $(CFLAGS) ./src/main.c -o ./bin/main $(LIBFLAGS)

run:
	./bin/main

clean:
	rm -f ./bin/*
