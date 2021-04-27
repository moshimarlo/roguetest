#include "monster.h"

#ifndef PLAYER_H
#define PLAYER_H
typedef struct Player {
	int curr_x, curr_y, prev_x, prev_y;
	int hp;
} Player;

void init_player(Player * player, int x, int y, int hp);

void collision_test(Player * player, int **map, int row, int col,
		   Monster ** monsters);

void player_attack(Player * player, int **map, int x, int y,
		  Monster ** monsters);

void player_move(Player * player, int x, int y);

void reset_player_pos(Player * player);

#endif
