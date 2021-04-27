#include "window.h"
#include <string.h>

#define winHeight 3
#define winWidth 20
#define winx1 0
#define winy1 0

TCOD_Console *debug_win;
TCOD_Console *game_win;

TCOD_Console *initWindow()
{
	debug_win = TCOD_console_new(winWidth, winHeight);
	TCOD_console_set_default_background(debug_win, TCOD_black);
	TCOD_console_clear(debug_win);
	return debug_win;
}

void printToBuffer(char *buffer, char *input)
{
	strcpy(buffer, input);
}

void printBuffer(char *buffer, TCOD_Console * window)
{
	TCOD_console_printf(window, 0, 0, buffer);
}
