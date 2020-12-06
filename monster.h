#include <stdbool.h>
#ifndef MONSTER_H 
#define MONSTER_H
typedef struct Monster {
    int x, y;
    int type;
    char symbol;
    int hp;
    bool alive;
} Monster;

typedef struct MonsterList {
    int numMonsters;
    Monster **monList;
} MonsterList;

void loadMonsters();

void initMonster(Monster *monster, int type);

void addMonster(int x, int y, int type);

void setSymbol(Monster *monster);

void freeMonsters();

#endif
