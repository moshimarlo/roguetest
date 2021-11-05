#include "window.h"
#include <string.h>

WINDOW *debug_win;
WINDOW *game_win;

static void debug_print_error(void);

void print_buffer(char *buffer, WINDOW *window)
{
	mvwprintw(window, 0, 0, buffer);
}

void print_to_debug(char *str, int x, int y) {
	if (y < 0 || y >= DEBUG_WIN_HEIGHT || x < 0 || x >= DEBUG_WIN_WIDTH) {
		debug_print_error();
		return;
	}
	if (strlen(str) + x > DEBUG_WIN_WIDTH) {
		debug_print_error();
		return;
	}
	char buf[DEBUG_WIN_WIDTH];
	snprintf(buf, DEBUG_WIN_WIDTH, "%s", str);
	mvwaddstr(debug_win, y, x, buf);
}

void debug_print_error(void) {
	char buf[DEBUG_WIN_WIDTH];
	snprintf(buf, DEBUG_WIN_WIDTH, "ERROR PRINTING");
	mvwaddstr(debug_win, 0, 0, buf);
}
