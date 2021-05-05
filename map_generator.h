#include "monster.h"
#include "player.h"

#include <libtcod.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

typedef struct rlmap_t {
	int **map;
	TCOD_map_t *fovmap;
} rlmap_t;

void init_map(int max_height, int max_width);

void randomize_map(int max_height, int max_width);

bool create_room(TCOD_bsp_t *node, void *user_data);

void make_path();

bool dijkstra(TCOD_bsp_t *curr_node, void *user_data);

void draw_map(TCOD_Console * window, int max_height, int max_width,
	      Monster ** monsters);

void set_tile(int x, int y, int type);

int get_tile(int x, int y);

bool is_wall(int x, int y);

bool is_monster(int x, int y);

void free_map(int max_width);

#endif
