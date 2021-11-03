#include "monster.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Monster *monsters;
static int monster_count;

void load_monsters()
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

void init_monster(Monster * monster, int type)
{
	monster->type = type;
	set_symbol(type);
	int hp = 0;
	switch (monster->type) {
	case NFILLER:
		hp = 1000;
		break;
	case NKOBOLD:
		hp = 5;
		break;
	case NORC:
		hp = 15;
		break;
	}
	monster->hp = hp;
	monster->alive = true;
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
