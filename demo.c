#include "pcg_basic.h"
#include "rng.h"
#include "symbols.h"
#include "input_handler.h"
#include "map_generator.h"
#include "player.h"
#include "monster.h"
#include "window.h"

#include <libtcod.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DELAY 20000
#define SCREEN_HEIGHT 50
#define SCREEN_WIDTH 80
#define WINDOW_HEIGHT 4 
#define GAMEWIN_HEIGHT SCREEN_HEIGHT - WINDOW_HEIGHT 

char debug_buffer[40];

int main(int argc, char *argv[])
{
	// Initialise randomisation
	init_rand();

	// Initialise player values
	Player player;
	Player *ptr_player = &player;
	init_player(ptr_player, 10, 10, 100);
	init_map(GAMEWIN_HEIGHT, SCREEN_WIDTH);

	// Initialise monsters
	int monster_count = 0;
	int *ptr_monster_count = &monster_count;
	Monster **monsters = malloc(sizeof(Monster*) * MAXMONSTERS);
	for (int i = 0; i < MAXMONSTERS; i++) {
		monsters[i] = malloc(sizeof(Monster));
	}

	load_monsters(monsters);

	// Input-related variables
	int input_sig = 0;

	// Initialise libtcod 
	int res = TCOD_console_init_root(SCREEN_WIDTH, SCREEN_HEIGHT, "roguetest",
					 false, TCOD_RENDERER_OPENGL);
	TCOD_console_set_default_background(NULL, TCOD_black);
	TCOD_console_set_default_foreground(NULL, TCOD_white);

	// Window
	game_win = TCOD_console_new(SCREEN_WIDTH, GAMEWIN_HEIGHT);
	debug_win = TCOD_console_new(SCREEN_WIDTH, WINDOW_HEIGHT);

	TCOD_console_set_char(game_win, ptr_player->curr_x,
			      ptr_player->curr_y, PLAYER_SYMBOL);


	// Draw the map
	draw_map(game_win, GAMEWIN_HEIGHT, SCREEN_WIDTH, monsters);
	TCOD_console_clear(game_win);
	TCOD_console_clear(debug_win);
	TCOD_console_clear(NULL);
	TCOD_console_blit(game_win, 0, 0, 0, 0, NULL, 0, 0, 1.0, 1.0);
	TCOD_console_blit(debug_win, 0, 0, 0, 0, NULL, 0, 48, 1.0, 1.0);
	TCOD_console_flush();

	// MAIN GAME LOOP
	while (input_sig != 1) {
		// Store player's previous position
		input_sig = 0;
		input_sig = handle_input(ptr_player);

		// 5 pressed - randomise the map
		if (input_sig == 2) {
			free_map(SCREEN_WIDTH);
			init_map(GAMEWIN_HEIGHT, SCREEN_WIDTH);
			// TODO: delete all monsters from array and reset count to
			// zero
			monster_count = 0;
		}

		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test(ptr_player, GAMEWIN_HEIGHT, SCREEN_WIDTH, monsters);

		// Clear screen after input
		TCOD_console_clear(game_win);
		TCOD_console_clear(debug_win);
		TCOD_console_clear(NULL);

		// Draw map
		draw_map(game_win, GAMEWIN_HEIGHT, SCREEN_WIDTH, monsters);

		print_buffer(debug_buffer, debug_win);

		// Draw character
		TCOD_console_set_char(game_win, ptr_player->curr_x,
				      ptr_player->curr_y, PLAYER_SYMBOL);

		TCOD_console_blit(game_win, 0, 0, 0, 0, NULL, 0, 0, 1.0, 1.0);
		TCOD_console_blit(debug_win, 0, 0, 0, 0, NULL, 0, 48, 1.0, 1.0);
		TCOD_console_flush();

		//usleep(DELAY);
	}

	free_map(SCREEN_WIDTH);
	free_monsters(monsters);
}
