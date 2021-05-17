#include <stdbool.h>
#ifndef MONSTER_H
#define MONSTER_H

#define MAX_MONSTERS 30

#define KOBOLD_HP 10 
#define ORC_HP 20

typedef struct Monster {
	int x, y;
	int type;
	char symbol;
	char name[20];
	int hp;
	bool alive;
} Monster;

typedef struct MonsterList {
	int numMonsters;
	Monster *monList[];
} MonsterList;

void load_monsters();
void init_monster(Monster * monster, int type);
void add_monster(int x, int y, int type);

Monster *get_monster_at(int x, int y);
char get_monster_tile(int x, int y);
char set_symbol(int type);
void set_name(Monster *monster);
int set_monster_hp(int type);

void free_monsters(Monster * monsters);

#endif
