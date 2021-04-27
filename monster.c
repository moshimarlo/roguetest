#include "monster.h"
#include "symbols.h"

#include <stdlib.h>
#include <string.h>

static Monster filler;
static Monster *ptr_filler = &filler;
//static Monster kobold;
//static Monster *ptr_kobold = &kobold;
//static Monster orc;
//static Monster *ptr_orc = &orc;

void load_monsters()
{
	init_monster(ptr_filler, NFILLER);
}

//typedef struct Monster {
//    int x, y;     - set by addMonster
//    int type;     - set by initMonster
//    char symbol;  - set by setSymbol, called by initMonster
//    int hp;       - set by initMonster
//    bool alive;   - set by initMonster
//} Monster;
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
void add_monster(Monster ** monsters, int x, int y, int type, int *count)
{
	monsters[*count]->x = x;
	monsters[*count]->y = y;
	monsters[*count]->symbol = set_symbol(type);
	monsters[*count]->hp = set_monster_hp(type);
	monsters[*count]->alive = true;
}

//Called by playerAttack() in player.c
Monster *get_monster_at(int x, int y, Monster ** monsters)
{
	for (int i = 0; i < MAXMONSTERS; i++) {
		if (monsters[i]->x == x && monsters[i]->y == y) {
			return monsters[i];
		}
	}
	return ptr_filler;
}

char get_monster_tile(int x, int y, Monster ** monsters)
{
	Monster *monster = get_monster_at(x, y, monsters);
	return monster->symbol;
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

int set_monster_hp(int type)
{
	int hp = 1000;
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

void free_monsters(Monster ** monsters)
{
	for (int i = 0; i < MAXMONSTERS; i++) {
		free(monsters[i]);
	}
	free(monsters);
}
