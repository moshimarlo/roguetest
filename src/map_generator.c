#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"
#include "pcg_basic.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>


#define MAX_ROOMS 30
#define ROOM_MIN_WIDTH 5
#define ROOM_MAX_WIDTH 10
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_HEIGHT 5

// Local functions
static bool occupied(int x1, int y1, int x2, int y2);
static void add_room(int x1, int y1, int x2, int y2, int iter);
static void place_monsters(room_t room);
static void place_player(void);
static void place_stairs(void);
static void render_camera(int screen_width, int screen_height, int *cx, int *cy);
static void tunnel_horiz(int x1, int x2, int y);
static void tunnel_vert(int y1, int y2, int x);
static int area(room_t room);
static int max(int a, int b);
static int min(int a, int b);
static pos_t center(room_t room);

// Local variables
static int **map;
static room_t *room_list;
static int room_count;

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

void free_map(void)
{
	for (int i = 0; i < MAP_WIDTH; i++) {
		free(map[i]);
	}
	free(map);
	free(room_list);
}

void create_rooms(void)
{
        room_count = 0;
        for (int i = 0; i < MAX_ROOMS; i++) {
                int room_width = get_rand(ROOM_MIN_WIDTH, ROOM_MAX_WIDTH);
                int room_height = get_rand(ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT);
                int x1 = get_rand(1, MAP_WIDTH - room_width-1);
                int y1 = get_rand(1, MAP_HEIGHT - room_height-1);
                int x2 = x1 + room_width;
                int y2 = y1 + room_height;

		int tries = 0;
                while (occupied(x1, y1, x2, y2) && tries++ < 5) {
			x1 = get_rand(1, MAP_WIDTH - room_width - 1);
			y1 = get_rand(1, MAP_HEIGHT - room_height - 1);
			x2 = x1 + room_width;
			y2 = y1 + room_height;
                }
		if (tries < 5) {
			add_room(x1, y1, x2, y2, room_count);
			pos_t current = center(room_list[room_count]);
			// If not first room, make tunnel to previous room
			if (room_count > 0) {
				pos_t prev = center(room_list[room_count-1]);
				if (get_rand(1,2) == 1) {
					tunnel_horiz(prev.x, current.x, prev.y);
					tunnel_vert(prev.y, current.y, current.x);
				} else {
					tunnel_vert(prev.y, current.y, prev.x);
					tunnel_horiz(prev.x, current.x, current.y);
				}
			}
			place_monsters(room_list[room_count]);
			++room_count;
		}
        }
	place_player();
	place_stairs();
}

void place_monsters(room_t room)
{
	bool are_monsters = (get_rand(1,100) <= 50);
	if (are_monsters) {
		int max_monsters = area(room)/4;
		int num_monsters = get_rand(1, max_monsters);

		for (int i = 0; i < num_monsters-1; i++) {
			int x = get_rand(room.x1, room.x2);
			int y = get_rand(room.y1, room.y2);
			add_monster(x, y, NKOBOLD);
			map[x][y] = NMONSTER;
		}
	}
}

void add_room(int x1, int y1, int x2, int y2, int iter)
{
        room_list[iter].x1 = x1;
        room_list[iter].y1 = y1;
        room_list[iter].x2 = x2;
        room_list[iter].y2 = y2;

        for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			map[i][j] = NFLOOR;
		}
        }
}

int area(room_t room)
{
	return (room.x2-room.x1)*(room.y2-room.y1);
}

int max(int a, int b)
{
	return (a >= b) ? a : b;
}

int min(int a, int b)
{
	return (a <= b) ? a : b;
}

void tunnel_horiz(int x1, int x2, int y)
{
	for (int i = min(x1, x2); i < max(x1, x2); i++) {
		map[i][y] = NFLOOR;
	}
}

void tunnel_vert(int y1, int y2, int x)
{
	for (int i = min(y1, y2); i < max(y1, y2); i++) {
		map[x][i] = NFLOOR;
	}
}

pos_t center(room_t room)
{
	pos_t result = { .x = 0, .y = 0 };
	int x = (room.x2-room.x1)/2 + room.x1;
	int y = (room.y2-room.y1)/2 + room.y1;
	result.x = x;
	result.y = y;
	return result;
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
	return NULL;
}

bool occupied(int x1, int y1, int x2, int y2)
{
        for (int i = x1; i <= x2; i++) {
                for (int j = y1; j <= y2; j++) {
			assert(i >= 0);
			assert(j >= 0);
			assert(i < MAP_WIDTH);
			assert(j < MAP_HEIGHT);
			if (map[i][j] != NWALL) return true;
                }
        }
        return false;
}

void place_player(void)
{
	pos_t cent = center(room_list[0]);
	put_player(cent.x, cent.y);
}

void place_stairs(void)
{
	int room_num = get_rand(0, room_count-1);
	room_t stairs_room = room_list[room_num];
	int x = get_rand(stairs_room.x1, stairs_room.x2);
	int y = get_rand(stairs_room.y1, stairs_room.y2);
	set_tile(x, y, NSTAIRCASE);
}

void render_camera(int screen_width, int screen_height, int *cx, int *cy)
{
	int player_x, player_y;
	get_player_xy(&player_x, &player_y);
        int camera_x = player_x - (screen_width / 2);
        int camera_y = player_y - (screen_height / 2);

	if (camera_x >= MAP_WIDTH - screen_width)
		camera_x = MAP_WIDTH - screen_width;
	if (camera_y >= MAP_HEIGHT - screen_height)
		camera_y = MAP_HEIGHT - screen_height;
	if (camera_x < 0)
		camera_x = 0;
	if (camera_y < 0)
		camera_y = 0;
        *cx = camera_x;
        *cy = camera_y;
}

void draw_map(WINDOW *window, int screen_width, int screen_height)
{
	int cx, cy;
	render_camera(screen_width, screen_height, &cx, &cy);
	bool offset = (cx <= 0 || cy <= 0 || cx + screen_width >= MAP_WIDTH || cy + screen_height >= MAP_HEIGHT); 
	int screen_x = 0; 
	for (int i = cx; i < cx + screen_width; i++) {
		int screen_y = 0;
		for (int j = cy; j < cy + screen_height; j++) {
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
	draw_player(window, screen_width, screen_height, cx, cy, offset);
}

void reset_map(void)
{
	free_map();
	init_map();
	create_rooms();
}

int get_tile(int x, int y)
{
	return map[x][y];
}

void set_tile(int x, int y, int type)
{
	map[x][y] = type;
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

bool is_floor(int x, int y)
{
	return map[x][y] == NFLOOR;
}

bool is_staircase(int x, int y)
{
	return map[x][y] == NSTAIRCASE;
}

bool is_monster(int x, int y)
{
	return map[x][y] == NMONSTER;
}
