#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <ncurses.h>

typedef struct Player {
	int curr_x, curr_y, prev_x, prev_y;
	int hp;
	int xp;
	int gold;
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
bool adjacent_to_player(int x, int y);

// Stats
int get_player_hp(void);
int get_player_xp(void);
int get_player_gold(void);
void player_inc_xp(int xp);

// Drawing
void draw_player(WINDOW *win, int screen_width, int screen_height, int cx, int cy, bool offset);

#endif
