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
#define WINDOW_HEIGHT 2

char debug_buffer[40];

int main(int argc, char *argv[])
{
	// Initialise randomisation
	init_rand();

	// Initialise player values
	Player player;
	Player *ptr_player = &player;
	init_player(ptr_player, 10, 10, 100);

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
	int screen_height = 50;
	int screen_width = 80;
	TCOD_console_init_root(80, 50, "roguetest", false,
			       TCOD_RENDERER_OPENGL);
	TCOD_console_set_default_background(NULL, TCOD_black);
	TCOD_console_set_default_foreground(NULL, TCOD_white);

	// Window
	game_win = TCOD_console_new(80, 50);
	debug_win = TCOD_console_new(20, 3);

	TCOD_console_set_char(game_win, ptr_player->curr_x,
			      ptr_player->curr_y, PLAYER_SYMBOL);

	// Allow room for window
	screen_height -= WINDOW_HEIGHT;

	// Initialise map
	int **map = malloc(sizeof(int*) * screen_width);
	for (int i = 0; i < screen_width; i++) {
		map[i] = malloc(sizeof(int) * screen_height);
	}

	TCOD_bsp_t *bsptree = TCOD_bsp_new_with_size(0, 0, screen_width, screen_height);
	TCOD_bsp_split_recursive(bsptree, NULL, 5, 5, 5, 1.7f, 0.6f);

	// Initialise FOV
	TCOD_map_t fovmap = TCOD_map_new(screen_width, screen_height);
	TCOD_map_clear(fovmap, false, true);

	init_map(map, screen_height, screen_width);
	randomize_map(bsptree, map, screen_height, screen_width); 

	// Connect rooms
	rlmap_t mstruct;
	rlmap_t *mstruct_p = &mstruct;
	mstruct_p->map = &map;
	mstruct_p->fovmap = &fovmap;
	make_path(bsptree, mstruct_p);

	// Draw the map
	draw_map(game_win, map, screen_height, screen_width, monsters);
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
			TCOD_map_clear(fovmap, false, true);
			init_map(map, screen_height, screen_width);
			// TODO: delete all monsters from array and reset count to
			// zero
			monster_count = 0;
			bsptree = TCOD_bsp_new_with_size(0, 0, screen_width, screen_height);
			TCOD_bsp_split_recursive(bsptree, NULL, 5, 5, 5, 1.7f, 0.6f);
			randomize_map(bsptree, map, screen_height, screen_width); 
		}

		/* If player tries to move outside the screen or into a wall, reset
		 * coordinates to stored value */
		collision_test(ptr_player, map, screen_height, screen_width, monsters);

		// Clear screen after input
		TCOD_console_clear(game_win);
		TCOD_console_clear(debug_win);
		TCOD_console_clear(NULL);

		// Draw map
		draw_map(game_win, map, screen_height, screen_width, monsters);

		print_buffer(debug_buffer, debug_win);

		// Draw character
		TCOD_console_set_char(game_win, ptr_player->curr_x,
				      ptr_player->curr_y, PLAYER_SYMBOL);

		TCOD_console_blit(game_win, 0, 0, 0, 0, NULL, 0, 0, 1.0, 1.0);
		TCOD_console_blit(debug_win, 0, 0, 0, 0, NULL, 0, 48, 1.0, 1.0);
		TCOD_console_flush();

		usleep(DELAY);
	}

	free_map(map, screen_height);
	free_monsters(monsters);
}
