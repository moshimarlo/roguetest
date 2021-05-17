#ifndef WINDOW_H_
#define WINDOW_H_
#include <ncurses.h>

#define DEBUG_WIN_HEIGHT 5
#define DEBUG_WIN_WIDTH 30

extern WINDOW *debug_win;
extern WINDOW *game_win;

extern char debug_buffer[40];

void print_to_buffer(char *buffer, char *input);

void print_buffer(char *buffer, WINDOW *window);

#endif
