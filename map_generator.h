#include "monster.h"
#include "player.h"

#include <ncurses.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

typedef struct room_t {
	int x1, y1, x2, y2;
} room_t;

void init_map(void);

void reset_map(void);

void create_rooms(void);

void place_player(void);

void add_room(int x1, int y1, int x2, int y2, int iter);

void render_camera(int player_x, int player_y, int screen_width, int screen_height, int *cx, int *cy);

void render_camera_test(int player_x, int player_y, int screen_width, int screen_height, int *cx, int *cy);

void draw_map(WINDOW *window, int screen_width, int screen_height);

room_t *get_room(int x, int y);

void set_tile(int x, int y, int type);

int get_tile(int x, int y);

int get_map_width(void);

int get_map_height(void);

bool is_wall(int x, int y);

bool is_monster(int x, int y);

void free_map(void);

#endif
