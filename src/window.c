#include "window.h"
#include "player.h"
#include <string.h>
#include <stdio.h>

WINDOW *debug_win;
WINDOW *viewport_win;
WINDOW *status_win;

static void debug_print_error(void);
static char status_buffer[STATUS_WIN_WIDTH];

void print_buffer(char *buffer, WINDOW *window)
{
	mvwprintw(window, 0, 0, buffer);
}

void print_status(void)
{
	int hp = get_player_hp();
	int xp = get_player_xp();
	snprintf(status_buffer, STATUS_WIN_WIDTH, "HP: %d", hp);
	mvwprintw(status_win, 1, 1, "%s", status_buffer);
	snprintf(status_buffer, STATUS_WIN_WIDTH, "XP: %d", xp);
	mvwprintw(status_win, 2, 1, status_buffer);
	mvwprintw(status_win, 3, 1, status_buffer);
	mvwprintw(status_win, 4, 1, status_buffer);
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
