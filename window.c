#include "window.h"
#include <string.h>

#define winHeight 3
#define winWidth 20
#define winx1 0
#define winy1 0

WINDOW *debug_win;
WINDOW *game_win;

void print_to_buffer(char *buffer, char *input)
{
	strcpy(buffer, input);
}

void print_buffer(char *buffer, WINDOW *window)
{
	mvwprintw(window, 0, 0, buffer);
}
