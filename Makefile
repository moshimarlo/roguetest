# Makefile
CC=gcc
CFLAGS=-g
LDFLAGS=-lncurses
DEPS = input_handler.h map_generator.h pcg_basic.h player.h rng.h symbols.h 
OBJ = rng.o demo.o input_handler.o map_generator.o pcg_basic.o player.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

demo: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

clean:
	rm *.o demo
