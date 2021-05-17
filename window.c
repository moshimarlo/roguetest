#include "window.h"
#include <string.h>

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
