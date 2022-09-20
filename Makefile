# Makefile
CC=gcc
CFLAGS=-g -pg -Wall -Wextra -pedantic
LDFLAGS=-lncurses -pg

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .
SRC := $(wildcard $(SRC_DIR)/*.c)
DEP := $(wildcard $(SRC_DIR)/*.h)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN := $(BIN_DIR)/demo

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/$(BIN)
	rm -f *log

.PHONY: val
val:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind.log \
         ./$(BIN)
