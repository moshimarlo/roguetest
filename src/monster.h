#ifndef MONSTER_H
#define MONSTER_H

#include <stdbool.h>

#define MAX_MONSTERS MAP_WIDTH * MAP_HEIGHT

// Default HP values for monsters
#define KOBOLD_HP 10 
#define ORC_HP 20

// Default XP gain from killing monsters
#define KOBOLD_XP 5 
#define ORC_XP 10 

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

void load_monsters(void);
void check_dead(void);
void add_monster(int x, int y, int type);
void move_monsters(void);
void draw_monsters(void);

Monster *get_monster_at(int x, int y);
char get_monster_tile(int x, int y);
char set_symbol(int type);
void set_name(Monster *monster);
int set_monster_hp(int type);
void decrease_monster_hp(Monster* monster, int value);

void free_monsters();

#endif
