CC = gcc
SDL = -framework SDL2 -framework SDL2_image -framework SDL2_ttf
CFLAGS = -Wall -std=gnu99
EXEC = OSM_Render

SRC_PATH = src/
HEADER_PATH = header/
BIN_PATH = bin/

all: $(EXEC)

OSM_Render: $(BIN_PATH)main.o $(BIN_PATH)init.o $(BIN_PATH)draw.o 
	$(CC) $(SDL) -o $(BIN_PATH)$@ $^

main.o: $(HEADER_PATH)main.h $(HEADER_PATH)prototypes.h $(HEADER_PATH)defs.h
init.o: $(HEADER_PATH)prototypes.h
draw.o: $(HEADER_PATH)prototypes.h

$(BIN_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run:
	./$(BIN_PATH)$(EXEC)

clean:
	rm -rf $(BIN_PATH)*.o

tar: mrproper
	tar cf $(EXEC).tar README.txt Makefile bin/ src/ header/ INSTALL.sh REMOVE.sh resources/
mrproper: clean
	rm -rf $(BIN_PATH)$(EXEC)
