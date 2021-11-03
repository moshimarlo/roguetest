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

int main(void)
{
	// Initialise randomisation
	init_rand();

	// Initialise player values
	init_player(0, 0, 100);

	// Initialise monsters
	load_monsters();

	// Initialise map
	init_map();
	create_rooms();

	// Input-related variables
	int game_state = AWAIT_INPUT;

	// Initialise curses
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	int screen_width, screen_height;
	getmaxyx(stdscr, screen_height, screen_width);

	// Initialise windows
	int dbg_y = screen_height - DEBUG_WIN_HEIGHT;
	int dbg_x = screen_width - DEBUG_WIN_WIDTH;
	game_win = newwin(screen_height, screen_width, 0, 0);
	debug_win = newwin(DEBUG_WIN_HEIGHT, DEBUG_WIN_WIDTH, dbg_y, dbg_x);


	// MAIN GAME LOOP
	while (game_state != QUIT) {
		werase(game_win);
		werase(debug_win);

		// Draw map and other window elements
		draw_map(game_win, screen_width, screen_height);

		int p_x, p_y;
		char buf[DEBUG_WIN_WIDTH];
		get_player_xy(&p_x, &p_y);
		snprintf(buf, DEBUG_WIN_WIDTH, "x: %d\ny: %d", p_x, p_y);
		mvwaddstr(debug_win,0,0,buf);

		wrefresh(game_win);
		wrefresh(debug_win);


		// Get player input
		game_state = AWAIT_INPUT;
		game_state = player_handle_input();
		// 5 pressed - randomise the map
		if (game_state == RANDOMIZE) {
			reset_map();
			// TODO: delete all monsters from array and reset count to
			// zero
			free_monsters();
			load_monsters();
		}

		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test();
	}
	free_map();
	free_monsters();
	free_player();
	endwin();
}
