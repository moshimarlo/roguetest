#include "player.h"
#include "map_generator.h"
#include "input_handler.h"
#include "symbols.h"
#include "monster.h"
#include "rng.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>

#define NOCLIP false

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
	return handle_input();
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
		if (!NOCLIP) reset_player_pos();
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
	char output[64];
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

void put_player(int x, int y)
{
	player->prev_x = x;
	player->prev_y = y;
	player->curr_x = x;
	player->curr_y = y;
}

void print_player_xy()
{
	char buf[DEBUG_WIN_WIDTH];
	snprintf(buf, DEBUG_WIN_WIDTH, "x: %d\ny: %d", player->curr_x, player->curr_y);
	mvwaddstr(debug_win,0,0,buf);
}

void reset_player_pos(void)
{
	player->curr_x = player->prev_x;
	player->curr_y = player->prev_y;
}

void draw_player(WINDOW *win, int screen_width, int screen_height, int cx, int cy, bool offset)
{
	if (offset)
		mvwaddch(win, player->curr_y-cy, player->curr_x-cx, PLAYER_SYMBOL);
	else
		mvwaddch(win, screen_height/2, screen_width/2, PLAYER_SYMBOL);
}
