# Makefile
CC=gcc
CFLAGS=-g #-Wall -Wextra #-pedantic
LDFLAGS=`pkg-config libtcod --cflags --libs` -lncurses
DEPS = input_handler.h map_generator.h pcg_basic.h player.h rng.h symbols.h monster.h window.h
OBJ = rng.o demo.o input_handler.o map_generator.o pcg_basic.o player.o monster.o window.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS) 

demo: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

.PHONY: clean
clean:
	rm *.o
	rm -f demo
	rm -f log*

.PHONY: val
val:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./demo
