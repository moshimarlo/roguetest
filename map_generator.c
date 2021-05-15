#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static int **map;
static int leaf_count;
static TCOD_map_t fovmap;
static TCOD_path_t path;
static room_t *room_list;

// Allocate memory for map and fill with walls 
void init_map(int max_width, int max_height)
{
	map = malloc(sizeof(int*) * max_width);
	for (int i = 0; i < max_width; i++) {
		map[i] = malloc(sizeof(int) * max_height);
		for (int j = 0; j < max_height; j++) {
			map[i][j] = NWALL;
		}
	}
	room_list = malloc(sizeof(room_t));

	// Initialise FOV
	fovmap = TCOD_map_new(max_width, max_height);
	TCOD_map_clear(fovmap, true, true);

	path = TCOD_path_new_using_map(fovmap, 1.0f);
}

void reset_map(int max_width, int max_height)
{
	for (int i = 0; i < max_width; i++) {
		for (int j = 0; j < max_height; j++) {
			map[i][j] = NWALL;
		}
	}
	TCOD_map_clear(fovmap, true, true);
}

// TODO: combine bsp functions
void randomize_map(int max_width, int max_height)
{
	// Create and split tree
	TCOD_bsp_t *bsptree = TCOD_bsp_new_with_size(0, 0, max_width, max_height);
	TCOD_bsp_split_recursive(bsptree, NULL, 5, 10, 5, 1.5f, 1.5f);

	// Initialise room list from leaf count

	// Create rooms from tree
	leaf_count = 1;
	TCOD_bsp_traverse_level_order(bsptree, create_room, NULL);

	// Connect rooms
	//find_nearest(leaf_count);

	//TCOD_bsp_delete(bsptree);
}

bool create_room(TCOD_bsp_t *node, void *data)
{
	if (TCOD_bsp_is_leaf(node)) {
		int horiz_thickness = get_rand(1,3);
		int vert_thickness = get_rand(1,3); 
		int x1 = node->x + horiz_thickness;
		int y1 = node->y + vert_thickness;
		int x2 = x1 + node->w - horiz_thickness;
		int y2 = y1 + node->h - vert_thickness;
		int width = x2 - x1;
		int height = y2 - y1;
		for (int i = x1; i < x2; i++) {
			for (int j = y1; j < y2; j++) {
				map[i][j] = NFLOOR;
			}
		}
		add_room(x1, y1, x2, y2, leaf_count);
		leaf_count++;	
	}
	return true;
}

void add_room(int x1, int y1, int x2, int y2, int iter)
{
	room_list = realloc(room_list, sizeof(room_t) * iter);
	room_list[iter-1].x1 = x1;
	room_list[iter-1].y1 = y1;
	room_list[iter-1].x2 = x2;
	room_list[iter-1].y2 = y2;
	room_list[iter-1].connected = false;
	room_list[iter-1].neighbour = NULL;
}


void find_nearest() {
	for (int room = 0; room < leaf_count; room++) {
		int x1 = room_list[room].x1;
		int y1 = room_list[room].y1;
		int x2 = room_list[room].x2;
		int y2 = room_list[room].y2;
		int width = x2 - x1;
		int height = y2 - y1;
		int middle_x = x1 + (x2/2);
		int middle_y = y1 + (y2/2);
		room_t *neighbour = NULL;
		for (int i = 0; i < 79; i++) {
			for (int j = 0; j < 45; j++) {
				neighbour = get_room(i, j, leaf_count);
				if (neighbour == NULL)
					continue;
				else {
					room_list[room].neighbour = neighbour;
					break;
				}
			}
			if (neighbour != NULL)
				break;
		}
	}
}

void connect_point(TCOD_path_t path, int x1, int y1, int x2, int y2)
{
	TCOD_path_compute(path, x1, y1, x2, y2); 
	for (int i = 0; i < TCOD_path_size(path); i++) {
		int curr_x, curr_y;
		TCOD_path_get(path, i, &curr_x, &curr_y);
		set_tile(curr_x, curr_y, NFLOOR);
		//TCOD_map_set_properties(fovmap, curr_x, curr_y, true, false);
	}
}

void draw_map(WINDOW *window, int max_width, int max_height)
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
				tile = get_monster_tile(i, j);
				break;
			}
			mvwaddch(window, j, i, tile);
		}
	}
}

room_t *get_room(int x, int y, int leaf_count)
{
	for (int i = 0; i < leaf_count; i++) {
		int x1 = room_list[i].x1;
		int y1 = room_list[i].y1;
		int x2 = room_list[i].x2;
		int y2 = room_list[i].y2;
		if (x1 <= x && x <= x2 && y1 <= y && y <= y2)
			return &room_list[i];
		else
			return NULL;
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
	TCOD_path_delete(path);
	TCOD_map_delete(fovmap);
	for (int i = 0; i < max_width; i++) {
		free(map[i]);
	}
	free(room_list);
	free(map);
}
