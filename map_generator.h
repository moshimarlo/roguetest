#include "monster.h"
#include "player.h"

#include <libtcod.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

void init_map(int **map, int max_height, int max_width);

void randomize_map(int **map, int max_height, int max_width, Monster ** monsters,
		  int *monster_count, Player * player);

void create_room(int **map, int max_height, int max_width, Monster ** monsters,
		int *monster_count, Player * player);

void draw_map(TCOD_Console * window, int **map, int max_height, int max_width,
	     Monster ** monsters);

void free_map(int **map, int max_height);

#endif
