CC = gcc
CFLAGS = -g -Wall
EXEC = OSM_Render 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBFLAGS = -lSDL2 -lxml2
endif
ifeq ($(UNAME_S),Darwin)
	LIBFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf  -framework libxml
endif

all:
	$(CC) $(CFLAGS) ./src/main.c -o ./bin/$(EXEC) $(LIBFLAGS)

run:
	./bin/$(EXEC)

clean:
	rm -rf ./bin/*

tar: mrproper
	tar cf $(EXEC).tar README.md Makefile bin/ src/ header/ include/ lib/ lib64/
