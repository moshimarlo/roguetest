#include "pcg_basic.h"
#include "rng.h"
#include "symbols.h"
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
	init_player(10, 10, 100);

	// Input-related variables
	int input_sig = 0;

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
	//wclear(game_win);
	//wclear(debug_win);

	/*draw_player(game_win, screen_width, screen_height);*/

	init_map();
	create_rooms();

	// Draw the map
	/*draw_map(game_win, screen_width, screen_height);*/
	/*wrefresh(game_win);*/
	/*wrefresh(debug_win);*/

	// MAIN GAME LOOP
	while (input_sig != 1) {
		wrefresh(game_win);
		wrefresh(debug_win);

		draw_map(game_win, screen_width, screen_height);
		draw_player(game_win, screen_width, screen_height);

		input_sig = 0;
		input_sig = player_handle_input();

		// 5 pressed - randomise the map
		if (input_sig == 2) {
			reset_map();
			// TODO: delete all monsters from array and reset count to
			// zero
		}

		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test();

		// Clear screen after input
		wclear(game_win);
		wclear(debug_win);

		// Draw map
		draw_map(game_win, screen_width, screen_height);

		//print_buffer(debug_buffer, debug_win);

		// Draw character
		draw_player(game_win, screen_width, screen_height);

		int p_x, p_y;
		char buf[DEBUG_WIN_WIDTH];
		get_player_xy(&p_x, &p_y);
		snprintf(buf, DEBUG_WIN_WIDTH, "x: %d\ny: %d", p_x, p_y);
		mvwaddstr(debug_win,0,0,buf);
		//mvwaddstr(debug_win,0,0,"Hi");
	}
	free_map();
	free_player();
	endwin();
	//free_monsters(monsters);
}
