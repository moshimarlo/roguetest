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

#define DELAY 20000
#define SCREEN_HEIGHT 25 
#define SCREEN_WIDTH 80
#define WINDOW_HEIGHT 4 
#define GAMEWIN_HEIGHT SCREEN_HEIGHT

char debug_buffer[40];

int main(void)
{
	// Initialise randomisation
	init_rand();

	// Initialise player values
	Player player;
	Player *ptr_player = &player;
	init_player(ptr_player, 10, 10, 100);

	// Input-related variables
	int input_sig = 0;

	// Initialise curses
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	// Initialise windows
	game_win = newwin(GAMEWIN_HEIGHT, SCREEN_WIDTH, 0, 0);
	//debug_win = newwin(WINDOW_HEIGHT, SCREEN_WIDTH, GAMEWIN_HEIGHT, 0);

	mvwaddch(game_win, ptr_player->curr_y, ptr_player->curr_x, PLAYER_SYMBOL);

	init_map(SCREEN_WIDTH, GAMEWIN_HEIGHT);
	randomize_map(SCREEN_WIDTH, GAMEWIN_HEIGHT);

	// Draw the map
	draw_map(game_win, SCREEN_WIDTH, GAMEWIN_HEIGHT);
	wrefresh(game_win);
	//wrefresh(debug_win);

	// MAIN GAME LOOP
	while (input_sig != 1) {
		// Store player's previous position
		input_sig = 0;
		input_sig = handle_input(ptr_player);

		// 5 pressed - randomise the map
		if (input_sig == 2) {
			reset_map(SCREEN_WIDTH, GAMEWIN_HEIGHT);
			randomize_map(SCREEN_WIDTH, GAMEWIN_HEIGHT);
			// TODO: delete all monsters from array and reset count to
			// zero
		}

		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test(ptr_player, SCREEN_WIDTH, GAMEWIN_HEIGHT);

		// Clear screen after input
		wclear(game_win);
		//wclear(debug_win);

		// Draw map
		draw_map(game_win, SCREEN_WIDTH, GAMEWIN_HEIGHT);

		//print_buffer(debug_buffer, debug_win);

		// Draw character
		mvwaddch(game_win, ptr_player->curr_y, ptr_player->curr_x, PLAYER_SYMBOL);

		wrefresh(game_win);
		//wrefresh(debug_win);
	}
	free_map(SCREEN_WIDTH);
	endwin();
	//free_monsters(monsters);
}
