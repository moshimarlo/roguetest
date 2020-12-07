#ifndef WINDOW_H_
#define WINDOW_H_
#include <ncurses.h>


extern WINDOW *debug_win;
extern WINDOW *game_win;

extern char debug_buffer[20];

void printToBuffer(char* buffer, char* input); 

void printBuffer(char* buffer, WINDOW *window); 

WINDOW *initWindow();

#endif
