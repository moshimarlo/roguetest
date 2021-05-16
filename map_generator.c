#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#define MAX_ROOMS 30
#define ROOM_MIN_WIDTH 5
#define ROOM_MAX_WIDTH 10
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_HEIGHT 10

static int **map;
static int leaf_count;
static room_t *room_list;

// Allocate memory for map and fill with walls 
void init_map(void)
{
	map = malloc(sizeof(int*) * MAP_WIDTH);
	for (int i = 0; i < MAP_WIDTH; i++) {
		map[i] = malloc(sizeof(int) * MAP_HEIGHT);
		for (int j = 0; j < MAP_HEIGHT; j++) {
			map[i][j] = NWALL;
		}
	}
	room_list = malloc(sizeof(room_t) * MAX_ROOMS);
}

void reset_map(void)
{
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			map[i][j] = NWALL;
		}
	}
}

bool occupied(int x1, int y1, int x2, int y2)
{
        for (int i = x1; i <= x2; i++) {
                for (int j = y1; j <= y2; j++) {
			assert(i >= 0);
			assert(j >= 0);
			assert(i < MAP_WIDTH);
			assert(j < MAP_HEIGHT);
			if (map[i][j] == NFLOOR)
				return true;
                }
        }
        return false;
}

void create_rooms()
{
        int room_count = 0;
        for (int i = 0; i < MAX_ROOMS; i++) {
                int room_width = get_rand(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
                int room_height = get_rand(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);
                int x1 = get_rand(1, MAP_WIDTH - room_width-1);
                int y1 = get_rand(1, MAP_HEIGHT - room_height-1);
                int x2 = x1 + room_width;
                int y2 = y1 + room_height;

                while (occupied(x1, y1, x2, y2)) {
			x1 = get_rand(1, MAP_WIDTH - room_width - 1);
			y1 = get_rand(1, MAP_HEIGHT - room_height - 1);
			x2 = x1 + room_width;
			y2 = y1 + room_height;
                }
                add_room(x1, y1, x2, y2, room_count);
        }
}

void add_room(int x1, int y1, int x2, int y2, int iter)
{
        room_list[iter].x1 = x1;
        room_list[iter].y1 = y1;
        room_list[iter].x2 = x2;
        room_list[iter].y2 = y2;

        for (int i = x1; i < x2; i++) {
		for (int j = y1; j < y2; j++) {
			map[i][j] = NFLOOR;
		}
        }
}

void render_camera(int player_x, int player_y, int screen_width, int screen_height, int *cx, int *cy)
{
        int camera_x = player_x - screen_width / 2;        
        int camera_y = player_y - screen_height / 2;

        if (camera_x > MAP_WIDTH - screen_width - 1)
		camera_x = MAP_WIDTH - screen_width - 1;
        if (camera_y > MAP_HEIGHT - screen_height - 1)
		camera_y = MAP_HEIGHT - screen_height - 1;
        if (camera_x < 0)
		camera_x = 0;
        if (camera_y < 0)
		camera_y = 0;
        *cx = camera_x;
        *cy = camera_y;
}

void draw_map(WINDOW *window, int screen_width, int screen_height)
{
	int player_x, player_y;
	get_player_xy(&player_x, &player_y);
	int cx, cy;
	render_camera(player_x, player_y, screen_width, screen_height, &cx, &cy);
	int screen_x = 0; 
	for (int i = cx; i < cx + screen_width; i++) {
		int screen_y = 0;
		for (int j = cy; j < cy + screen_height-1; j++) {
			char tile = '?';
			int tile_value;
			if (i < 0 || i >= MAP_WIDTH || j < 0 || j >= MAP_HEIGHT) {
				tile_value = NBOUNDS;
			} else {
				tile_value = map[i][j];
			}
			switch (tile_value) {
			case NBOUNDS:
				tile = BOUNDS;
				break;
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
			mvwaddch(window, screen_y, screen_x, tile);
			++screen_y;
		}
		++screen_x;
	}
}

room_t *get_room(int x, int y)
{
	for (int i = 0; i < MAX_ROOMS; i++) {
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

int get_map_width(void)
{
	return MAP_WIDTH;
}

int get_map_height(void)
{
	return MAP_HEIGHT;
}

bool is_wall(int x, int y)
{
	return map[x][y] == NWALL;
}

bool is_monster(int x, int y)
{
	return map[x][y] == NMONSTER;
}

void free_map(void)
{
	for (int i = 0; i < MAP_WIDTH; i++) {
		free(map[i]);
	}
	free(map);
	free(room_list);
}
