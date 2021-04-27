#ifndef WINDOW_H_
#define WINDOW_H_
#include <libtcod.h>


extern TCOD_Console *debug_win;
extern TCOD_Console *game_win;

extern char debug_buffer[20];

void printToBuffer(char* buffer, char* input); 

void printBuffer(char* buffer, TCOD_Console *window); 

TCOD_Console *initWindow();

#endif
