#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Fill the map with walls, to be tunneled by randomizeMap
void init_map(int **map, int max_height, int max_width)
{
	for (int i = 0; i < max_width; i++) {
		for (int j = 0; j < max_height; j++) {
			map[i][j] = NWALL;
		}
	}
}

//Called by demo.c
void randomize_map(TCOD_bsp_t *bsp, int **map, int max_height, int max_width)
{
	TCOD_bsp_t tmp = *bsp;
	TCOD_bsp_traverse_post_order(&tmp, create_room, map);
}

bool create_room(TCOD_bsp_t *node, int **map)
{
	if (TCOD_bsp_is_leaf(node)) {
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

void make_path(TCOD_bsp_t *bsp, rlmap_t *mstruct)
{
	TCOD_bsp_t tmp = *bsp;
	TCOD_bsp_traverse_in_order(&tmp, dijkstra, mstruct);
}

bool dijkstra(TCOD_bsp_t *curr_node, rlmap_t *mstruct)
{
	char debug_buffer[40];
	static int count = 0;
	static TCOD_bsp_t *prev_node = NULL;
	if (count == 0) {
		prev_node = curr_node;
		return true;
	}
	if (TCOD_bsp_is_leaf(curr_node)) {
		count++;
		TCOD_dijkstra_t path = TCOD_dijkstra_new(mstruct->fovmap, 1.41f);
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
				mstruct->map[curr_x][curr_y] = NFLOOR;
			}
		}

	}
	prev_node = curr_node;
	return true;
}

void draw_map(TCOD_Console * window, int **map, int max_height, int max_width,
	     Monster ** monsters)
{
	int tile_value;
	char tile = '?';
	for (int i = 0; i < max_width; i++) {
		for (int j = 0; j < max_height; j++) {
			tile_value = *(* (map + i) + j);
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

void free_map(int **map, int max_height)
{
	for (int i = 0; i < max_height; i++) {
		free(map[i]);
	}
	free(map);
}
