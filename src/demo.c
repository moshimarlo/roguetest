#include "pcg_basic.h"
#include "rng.h"
#include "symbols.h"
#include "game_state.h"
#include "input_handler.h"
#include "map_generator.h"
#include "player.h"
#include "monster.h"
#include "window.h"

#include <ncurses.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define GRAPHICAL 1

void init_curses(int *width, int *height)
{
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, *height, *width);
}

void quit_too_small()
{
	print_to_debug("Window too small!", 0, 0);
	print_to_debug("Increase size to at least 80x30", 0, 1);
	refresh();
	wrefresh(debug_win);
	getch();
	endwin();
}

bool new_level(int state)
{
	// FILE *fd = fopen("aslog", "a");
	switch (state) {
	case RANDOMIZE:
		// fprintf(fd, "#######################################\n");
		// fprintf(fd, "#######################################\n");
		// fclose(fd);
		return true;
	case DESCEND:
		if (player_on_stairs()) {
			// fprintf(fd, "#######################################\n");
			// fprintf(fd, "#######################################\n");
			// fclose(fd);
			return true;
		}
		break;
	}
	return false;
}

int main(void)
{
	int game_state = AWAIT_INPUT;
#if GRAPHICAL
	// Initialise curses
	int screen_width, screen_height;
	init_curses(&screen_width, &screen_height);

	// Initialise windows
	int viewport_height = screen_height;
	int viewport_width = screen_width-STATUS_WIN_WIDTH;
	int viewport_y = 0;
	int viewport_x = 0;
	int status_height = screen_height;
	int status_width = STATUS_WIN_WIDTH;
	int status_y = 0;
	int status_x = screen_width-STATUS_WIN_WIDTH;
	viewport_win = newwin(viewport_height, viewport_width, viewport_y, viewport_x);
	status_win = newwin(status_height, status_width, status_y, status_x);
	//debug_win = newwin(DEBUG_WIN_HEIGHT, DEBUG_WIN_WIDTH, dbg_y, dbg_x);

	bool window_too_small = (screen_width < 80 || screen_height < 30);

	if (window_too_small) {
		game_state = QUIT;
		quit_too_small();
	}
#endif // GRAPHICAL

	// Initialise randomisation
	init_rand();

	// Initialise player values
	init_player(0, 0, 100);

	// Initialise monsters
	load_monsters();

	// Initialise map
	init_map();
	create_rooms();

	// MAIN GAME LOOP
	while (game_state != QUIT) {
#if GRAPHICAL
		werase(viewport_win);
		werase(status_win);
#endif // GRAPHICAL

		check_dead();

#if GRAPHICAL
		// Draw map and other window elements
		draw_map(viewport_win, viewport_width, viewport_height);
		draw_monsters();
		box(status_win, ACS_VLINE, ACS_HLINE);
		print_status();

		refresh();
		wrefresh(viewport_win);
		wrefresh(status_win);
#endif // GRAPHICAL

		// Get player input
		game_state = AWAIT_INPUT;
		game_state = player_handle_input();


		// If input causes a change in state, e.g. player has descended
		// stairs to a new level, perform relevant functions 
		if (new_level(game_state)) {
			reset_map();
			continue;
		}
		
		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test();
		move_monsters();
	}
	free_map();
	free_monsters();
	free_player();
	endwin();
}
