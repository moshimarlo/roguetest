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

void loadMonsters()
{
	initMonster(ptr_filler, NFILLER);
}

//typedef struct Monster {
//    int x, y;     - set by addMonster
//    int type;     - set by initMonster
//    char symbol;  - set by setSymbol, called by initMonster
//    int hp;       - set by initMonster
//    bool alive;   - set by initMonster
//} Monster;
void initMonster(Monster * monster, int type)
{
	monster->type = type;
	setSymbol(type);
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
void addMonster(Monster ** monsters, int x, int y, int type, int *count)
{
	monsters[*count]->x = x;
	monsters[*count]->y = y;
	monsters[*count]->symbol = setSymbol(type);
	monsters[*count]->hp = setMonsterHP(type);
	monsters[*count]->alive = true;
}

//Called by playerAttack() in player.c
Monster *getMonsterAt(int x, int y, Monster ** monsters)
{
	for (int i = 0; i < MAXMONSTERS; i++) {
		if (monsters[i]->x == x && monsters[i]->y == y) {
			return monsters[i];
		}
	}
	return ptr_filler;
}

char getMonsterTile(int x, int y, Monster ** monsters)
{
	Monster *monster = getMonsterAt(x, y, monsters);
	return monster->symbol;
}

char setSymbol(int type)
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

int setMonsterHP(int type)
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

void freeMonsters(Monster ** monsters)
{
	for (int i = 0; i < MAXMONSTERS; i++) {
		free(monsters[i]);
	}
	free(monsters);
}
