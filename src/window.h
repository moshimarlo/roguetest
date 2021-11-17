#ifndef __WINDOW_H_
#define __WINDOW_H_

#include <ncurses.h>

#define DEBUG_WIN_HEIGHT 5
#define DEBUG_WIN_WIDTH 30
#define STATUS_WIN_WIDTH 20

extern WINDOW *debug_win;
extern WINDOW *viewport_win;
extern WINDOW *status_win;

extern char debug_buffer[DEBUG_WIN_WIDTH];

void print_buffer(char *buffer, WINDOW *window);
void print_status(void);
void print_to_debug(char *str, int x, int y); 

#endif
