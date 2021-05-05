#include "monster.h"
#include "player.h"

#include <libtcod.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

typedef struct rlmap_t {
	int **map;
	TCOD_map_t *fovmap;
} rlmap_t;

void init_map(int **map, int max_height, int max_width);

void randomize_map(TCOD_bsp_t *bsp, int **map, int max_height, int max_width);

bool create_room(TCOD_bsp_t *node, int **map);

void make_path(TCOD_bsp_t *bsp, rlmap_t *mstruct);

bool dijkstra(TCOD_bsp_t *curr_node, rlmap_t *mstruct);

void draw_map(TCOD_Console * window, int **map, int max_height, int max_width,
	     Monster ** monsters);

void free_map(int **map, int max_height);

#endif
