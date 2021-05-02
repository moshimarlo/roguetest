#ifndef WINDOW_H_
#define WINDOW_H_
#include <libtcod.h>

extern TCOD_Console *debug_win;
extern TCOD_Console *game_win;

extern char debug_buffer[40];

void print_to_buffer(char *buffer, char *input);

void print_buffer(char *buffer, TCOD_Console * window);

TCOD_Console *init_window();

#endif
