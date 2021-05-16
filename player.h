#include <ncurses.h>

#ifndef PLAYER_H
#define PLAYER_H
typedef struct Player {
	int curr_x, curr_y, prev_x, prev_y;
	int hp;
} Player;

void init_player(int x, int y, int hp);

void draw_player(WINDOW *win, int screen_width, int screen_height);

int player_handle_input(void);

void get_player_xy(int *x, int *y);

void collision_test(void);

void player_attack(int x, int y);

void player_move(int x, int y);

void reset_player_pos(void);

void free_player(void);

#endif
