#include "player.h"
#include "map_generator.h"
#include "input_handler.h"
#include "symbols.h"
#include "monster.h"
#include "rng.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>

// Local functions
void player_attack(int x, int y);
void reset_player_pos(void);

// Local variables
static Player *player;

void init_player(int x, int y, int hp)
{
	player = malloc(sizeof(Player));
	player->curr_x = player->prev_x = x;
	player->curr_y = player->prev_y = y;
	player->hp = hp;
}

void free_player(void)
{
	free(player);
}

int player_handle_input(void)
{
	return handle_input(player);
}

void collision_test(void)
{
	int max_width = get_map_width();
	int max_height = get_map_height();
	if (player->curr_x >= max_width || player->curr_x < 0) {
		reset_player_pos();
		return;
	}
	if (player->curr_y >= max_height || player->curr_y < 0) {
		reset_player_pos();
		return;
	}
	if (is_wall(player->curr_x, player->curr_y)) {
		//reset_player_pos(player);
		return;
	}
	if (is_monster(player->curr_x, player->curr_y)) {
		player_attack(player->curr_x, player->curr_y);
	}
	player->prev_x = player->curr_x;
	player->prev_y = player->curr_y;
}

void player_attack(int x, int y)
{
	Monster *target = get_monster_at(x, y);
	int hit_roll = 3;
	target->hp -= hit_roll;
	char output[20];
	sprintf(output, "%d", target->hp);
	//print_to_buffer(debug_buffer, output);
	if (target->hp <= 0) {
		set_tile(x, y, NFLOOR);
		target->alive = false;
		sprintf(output, "You killed a %s", target->name);
		//print_to_buffer(debug_buffer, output);
	} else {
		reset_player_pos();
	}
}

void get_player_xy(int *x, int *y)
{
	*x = player->curr_x;
	*y = player->curr_y;
}


void player_move(int x, int y)
{
	player->prev_x = player->curr_x;
	player->prev_y = player->curr_y;
	player->curr_x += x;
	player->curr_y += y;
}

void reset_player_pos(void)
{
	player->curr_x = player->prev_x;
	player->curr_y = player->prev_y;
}

void draw_player(WINDOW *win, int screen_width, int screen_height)
{
	mvwaddch(win, screen_height/2, screen_width/2, PLAYER_SYMBOL);
	//mvwaddch(win, player->curr_y, player->curr_x, PLAYER_SYMBOL);
}
