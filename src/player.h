#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>

typedef struct Player {
	int curr_x, curr_y, prev_x, prev_y;
	int hp;
	int xp;
	bool confused; 
} Player;

// Memory allocation
void init_player(int x, int y, int hp);
void free_player(void);

// Input
int player_handle_input(void);
void collision_test(void);

// Movement
void get_player_xy(int *x, int *y);
void player_move(int x, int y);
void put_player(int x, int y);
void print_player_xy();
bool player_on_stairs(void);

// Drawing
void draw_player(WINDOW *win, int screen_width, int screen_height, int cx, int cy, bool offset);

#endif
