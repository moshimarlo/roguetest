#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int **map;
static int leaf_count;
static TCOD_map_t fovmap;
static TCOD_bsp_t *bsptree;
static rlmap_t mstruct;
static rlmap_t *mstruct_p;

// Allocate memory for map and fill with walls 
void init_map(int max_height, int max_width)
{
	map = malloc(sizeof(int*) * max_width);
	for (int i = 0; i < max_width; i++) {
		map[i] = malloc(sizeof(int) * max_height);
		for (int j = 0; j < max_height; j++) {
			map[i][j] = NWALL;
		}
	}

	// Initialise FOV
	fovmap = TCOD_map_new(max_width, max_height);
	TCOD_map_clear(fovmap, false, true);

	mstruct.map = map;
	mstruct.fovmap = &fovmap;
	mstruct_p = &mstruct;

	randomize_map(max_height, max_width); 

}

void randomize_map(int max_height, int max_width)
{
	bsptree = TCOD_bsp_new_with_size(0, 0, max_width, max_height);
	TCOD_bsp_split_recursive(bsptree, NULL, 5, 5, 5, 1.7f, 0.6f);
	leaf_count = 0;
	TCOD_bsp_t tmp = *bsptree;
	TCOD_bsp_traverse_post_order(&tmp, create_room, NULL);

	// Connect rooms
	make_path();
}

bool create_room(TCOD_bsp_t *node, void *user_data)
{
	if (TCOD_bsp_is_leaf(node)) {
		++leaf_count;
		int horiz_thickness = get_rand(1,3);
		int vert_thickness = get_rand(1,3); 
		int x1 = node->x + horiz_thickness;
		int y1 = node->y + vert_thickness;
		int x2 = node->x + node->w - horiz_thickness; 
		int y2 = node->y + node->h - vert_thickness;
		for (int i = x1; i <= x2; i++) {
			for (int j = y1; j <= y2; j++) {
				map[i][j] = NFLOOR;
			}
		}
	}
	return true;
}

void make_path()
{
	TCOD_bsp_t tmp = *bsptree;
	TCOD_bsp_traverse_in_order(&tmp, dijkstra, NULL);
}

// Compute dijkstra path connecting the rooms
bool dijkstra(TCOD_bsp_t *curr_node, void *user_data)
{
	char debug_buffer[40];

	static TCOD_bsp_t *prev_node = NULL;
	if (prev_node == NULL) {
		prev_node = curr_node;
		return true;
	}
	if (TCOD_bsp_is_leaf(curr_node)) {
		TCOD_dijkstra_t path = TCOD_dijkstra_new(*mstruct_p->fovmap, 1.41f);
		int root_x = prev_node->x/2;
		int root_y = prev_node->y/2;
		int dest_x = curr_node->x/2;
		int dest_y = curr_node->y/2;
		TCOD_dijkstra_compute(path, root_x, root_y);
		bool successful = TCOD_dijkstra_path_set(path, dest_x, dest_y);
		if (!successful) {
			strcpy(debug_buffer, "Dijkstra unsuccessful!");
			print_buffer(debug_buffer, debug_win);
		} else {
			for (int i = 0; i < TCOD_dijkstra_size(path); i++) {
				int curr_x, curr_y;
				TCOD_dijkstra_get(path, i, &curr_x, &curr_y);
				set_tile(curr_x, curr_y, NFLOOR);
			}
		}

	}
	prev_node = curr_node;
	return true;
}

void draw_map(TCOD_Console * window, int max_height, int max_width,
	      Monster ** monsters)
{
	char tile = '?';
	for (int i = 0; i < max_width; i++) {
		for (int j = 0; j < max_height; j++) {
			int tile_value = map[i][j];
			switch (tile_value) {
			case NFLOOR:
				tile = FLOOR;
				break;
			case NWALL:
				tile = WALL;
				break;
			case NDOOR:
				tile = DOOR;
				break;
			case NSTAIRCASE:
				tile = STAIRCASE;
				break;
			case NMONSTER:
				tile = get_monster_tile(i, j, monsters);
				break;
			}
			TCOD_console_set_char(window, i, j, tile);
		}
	}
}

void set_tile(int x, int y, int type)
{
	map[x][y] = type;
}

int get_tile(int x, int y)
{
	return map[x][y];
}

bool is_wall(int x, int y)
{
	if (map[x][y] == NWALL)
		return true;
	else
		return false;
}

bool is_monster(int x, int y)
{
	if (map[x][y] == NMONSTER)
		return true;
	else
		return false;
}

void free_map(int max_width)
{
	for (int i = 0; i < max_width; i++) {
		free(map[i]);
	}
	free(map);
}
