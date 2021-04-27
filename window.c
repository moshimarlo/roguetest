#include "window.h"
#include <string.h>

#define winHeight 3
#define winWidth 20
#define winx1 0
#define winy1 0

TCOD_Console *debug_win;
TCOD_Console *game_win;

TCOD_Console *init_window()
{
	debug_win = TCOD_console_new(winWidth, winHeight);
	TCOD_console_set_default_background(debug_win, TCOD_black);
	TCOD_console_clear(debug_win);
	return debug_win;
}

void print_to_buffer(char *buffer, char *input)
{
	strcpy(buffer, input);
}

void print_buffer(char *buffer, TCOD_Console * window)
{
	TCOD_console_printf(window, 0, 0, buffer);
}
