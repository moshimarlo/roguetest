#include "monster.h"
#include "player.h"

#include <libtcod.h>
#include <ncurses.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

typedef struct room_t {
	int x1, y1, x2, y2;
	bool connected;
	struct room_t *neighbour;
} room_t;

void init_map(int max_width, int max_height);

void reset_map(int max_width, int max_height);

void randomize_map(int max_width, int max_height);

bool create_room(TCOD_bsp_t *node, void *data);

void add_room(int x1, int y1, int x2, int y2, int iter);

void find_nearest();

void connect_point(TCOD_path_t path, int x1, int y1, int x2, int y2);

void draw_map(WINDOW *window, int max_height, int max_width);

room_t *get_room(int x, int y, int leaf_count);

void set_tile(int x, int y, int type);

int get_tile(int x, int y);

bool is_wall(int x, int y);

bool is_monster(int x, int y);

void free_map(int max_width);

#endif
