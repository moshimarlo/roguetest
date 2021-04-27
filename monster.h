#include <stdbool.h>
#ifndef MONSTER_H
#define MONSTER_H
#define KOBOLD_HP 5
#define ORC_HP 20
typedef struct Monster {
	int x, y;
	int type;
	char symbol;
	int hp;
	bool alive;
} Monster;

typedef struct MonsterList {
	int numMonsters;
	Monster *monList[];
} MonsterList;

void loadMonsters();

void initMonster(Monster * monster, int type);

void addMonster(Monster ** monsters, int x, int y, int type, int *monsterCount);

Monster *getMonsterAt(int x, int y, Monster ** monsters);

char getMonsterTile(int x, int y, Monster ** monsters);

char setSymbol(int type);

int setMonsterHP(int type);

void freeMonsters(Monster ** monsters);

#endif
