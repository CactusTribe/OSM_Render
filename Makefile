CC = gcc
CFLAGS = -g -Wall
EXEC = OSM_Render 

SRC_DIR = ./src
BIN_DIR = ./bin
EXEC_FILE= $(BIN_DIR)/$(EXEC)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBFLAGS = -lSDL2 -lxml2
endif
ifeq ($(UNAME_S),Darwin)
	LIBFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework libxml
endif

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addprefix $(BIN_DIR)/, $(notdir $(SRC_FILES:.c=.o)))

all: $(EXEC_FILE)

$(EXEC_FILE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFLAGS)

$(OBJ_FILES): $(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

run:
	$(EXEC_FILE)

clean:
	rm -rf $(BIN_DIR)/*

tar: mrproper
	tar cf $(EXEC).tar README.md Makefile $(BIN_DIR) $(SRC_DIR) 
