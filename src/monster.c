#include "monster.h"
#include "astar.h"
#include "symbols.h"
#include "window.h"
#include "map_generator.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Monster *monsters;
static int monster_count;

void load_monsters(void)
{
	monsters = malloc(sizeof(Monster)*MAX_MONSTERS);
	Monster filler = { .x = 0,
			.y = 0,
			.type = NFILLER,
			.symbol = FILLER,
			.name = "Filler",
			.hp = 1000,
			.alive = true };
	monster_count = 0;
	for (int i = 0; i < MAX_MONSTERS; i++) {
		monsters[i] = filler;
	}
}

void check_dead(void)
{
	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (monsters[i].hp <= 0 && monsters[i].alive == true) {
			monsters[i].alive = false;
			set_tile(monsters[i].x, monsters[i].y, NFLOOR);
			int xp_gain = 0;
			switch (monsters[i].type) {
			case NKOBOLD:
				xp_gain = KOBOLD_XP;
				break;
			case NORC:
				xp_gain = ORC_XP;
				break;
			}
			player_inc_xp(xp_gain);
		}
	}
}

void draw_monsters(void)
{
	for (int i = 0; i < monster_count; i++) {
		if (monsters[i].alive) {
			set_tile(monsters[i].x, monsters[i].y, NMONSTER);
		}
	}
}

void move_monsters(void)
{
	int map_width = get_map_width();
	int map_height = get_map_height();
	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (monsters[i].hp >= 0 && monsters[i].alive == true) {
			int player_x, player_y;
			get_player_xy(&player_x, &player_y);
			point_t start = { .x = monsters[i].x, .y = monsters[i].y };
			point_t end = { .x = player_x, .y = player_y };
			path_t *path = astar(start, end, map_width, map_height);
			monsters[i].x = path->points[path->len].x;
			monsters[i].y = path->points[path->len].y;
			//set_tile(monsters[i].x, monsters[i].y, NMONSTER);
			//monsters[i].x += 1;
			//monsters[i].y += 1;
			free_path(path);
		}
	}
}

//Called by createRoom() in map_generator.c 
void add_monster(int x, int y, int type)
{
	monsters[monster_count].x = x;
	monsters[monster_count].y = y;
	monsters[monster_count].type = type;
	monsters[monster_count].symbol = set_symbol(type);
	set_name(&monsters[monster_count]);
	monsters[monster_count].hp = set_monster_hp(type);
	monsters[monster_count].alive = true;
	++monster_count;
}

//Called by player_attack() in player.c
Monster *get_monster_at(int x, int y)
{
	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (monsters[i].x == x && monsters[i].y == y) {
			return &monsters[i];
		}
	}
	return NULL;
}

char get_monster_tile(int x, int y)
{
	if (get_monster_at(x,y) != NULL) {
		return get_monster_at(x,y)->symbol;
	} else {
		return FILLER;
	}
}

char set_symbol(int type)
{
	char symbol = '?';
	switch (type) {
	case NKOBOLD:
		symbol = KOBOLD;
		break;
	case NORC:
		symbol = ORC;
		break;
	}
	return symbol;
}

void set_name(Monster *monster)
{
	switch (monster->type) {
	case NKOBOLD:
		strcpy(monster->name, SKOBOLD);
		break;
	case NORC:
		strcpy(monster->name, SORC);
		break;
	}
}

int set_monster_hp(int type)
{
	int hp = 999;
	switch (type) {
	case NKOBOLD:
		hp = KOBOLD_HP;
		break;
	case NORC:
		hp = ORC_HP;
		break;
	}
	return hp;
}

void free_monsters()
{
	free(monsters);
}
