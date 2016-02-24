CC = gcc
CFLAGS = -g -Wall -std=c99 `xml2-config --cflags` `xml2-config --libs`
EXEC = OSM_Render

SRC_DIR = src
BIN_DIR = bin
EXEC_FILE= $(BIN_DIR)/$(EXEC)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBFLAGS = -lSDL2 -lxml2 -lSDL2_gfx
endif
ifeq ($(UNAME_S),Darwin)
	LIBFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework libxml
endif




# Solution provisoire
SRC_FILES= $(wildcard $(SRC_DIR)/**.c) $(wildcard $(SRC_DIR)/**/**.c) $(wildcard $(SRC_DIR)/**/**/**.c) $(wildcard $(SRC_DIR)/**/**/**/**.c) 
OBJ_FILES= $(patsubst %.c, %.o,  $(subst $(SRC_DIR), $(BIN_DIR),$(SRC_FILES)))

all: $(EXEC_FILE)

$(EXEC_FILE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFLAGS)

$(OBJ_FILES): $(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

run:
	$(EXEC_FILE)

clean:
	rm -rf $(BIN_DIR)/*

tar: clean
	tar cf "$(EXEC).tar" README.md Makefile $(BIN_DIR) $(SRC_DIR) 
