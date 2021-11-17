#ifndef __MAP_GENERATOR_H_
#define __MAP_GENERATOR_H_

#include <ncurses.h>

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

typedef struct room_t {
	int x1, y1, x2, y2;
} room_t;

typedef struct pos_t {
	int x, y;
} pos_t;

// Memory allocation
void init_map(void);
void free_map(void);

// Rooms
void create_rooms(void);
room_t *get_room(int x, int y);

// Drawing
void draw_map(WINDOW *window, int screen_width, int screen_height);

// Map
void reset_map(void);
int get_tile(int x, int y);
void set_tile(int x, int y, int type);
int get_map_width(void);
int get_map_height(void);
bool is_wall(int x, int y);
bool is_floor(int x, int y);
bool is_staircase(int x, int y);
bool is_monster(int x, int y);
bool out_of_bounds(int x, int y);

#endif
