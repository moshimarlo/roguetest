#include "window.h"
#include <string.h>

WINDOW *debug_win;
WINDOW *game_win;

void print_buffer(char *buffer, WINDOW *window)
{
	mvwprintw(window, 0, 0, buffer);
}
