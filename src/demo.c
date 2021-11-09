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
	print_to_debug("Please increase size to at least 80x30", 0, 1);
	refresh();
	wrefresh(debug_win);
	getch();
	endwin();
}

void process_new_state(int state)
{
	switch (state) {
	case RANDOMIZE:
		free_monsters();
		load_monsters();
		reset_map();
		break;
	case DESCEND:
		if (player_on_stairs()) {
			free_monsters();
			load_monsters();
			reset_map();
		}
		break;
	}
}

int main(void)
{
	// Initialise curses
	int screen_width, screen_height;
	init_curses(&screen_width, &screen_height);

	// Initialise windows
	int dbg_y = screen_height - DEBUG_WIN_HEIGHT;
	int dbg_x = screen_width - DEBUG_WIN_WIDTH;
	int viewport_width = screen_width-DEBUG_WIN_WIDTH;
	int viewport_height = screen_height;
	int status_y = 0;
	int status_x = viewport_width+1;
	viewport_win = newwin(viewport_height, viewport_width, 0, 0);
	status_win = newwin(screen_height, screen_width-status_x, status_y, status_x);
	//debug_win = newwin(DEBUG_WIN_HEIGHT, DEBUG_WIN_WIDTH, dbg_y, dbg_x);

	int game_state;
	bool window_too_small = (screen_width < 80 || screen_height < 30);

	if (window_too_small) {
		game_state = QUIT;
		quit_too_small();
	}

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
		werase(viewport_win);
		werase(status_win);
		//werase(debug_win);

		check_dead();

		// Draw map and other window elements
		draw_map(viewport_win, viewport_width, viewport_height);
		//print_player_xy();
		wborder(status_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, 0, 0, 0, 0);
		print_status();

		refresh();
		wrefresh(viewport_win);
		wrefresh(status_win);
		//wrefresh(debug_win);

		// Get player input
		game_state = AWAIT_INPUT;
		game_state = player_handle_input();

		// If input causes a change in state, e.g. player has descended
		// stairs to a new level, perform relevant functions 
		process_new_state(game_state);
		
		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test();
	}
	free_map();
	free_monsters();
	free_player();
	endwin();
}
