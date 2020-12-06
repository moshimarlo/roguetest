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
    Monster *monList[];
} MonsterList;

void loadMonsters(Monster **monsters);

void initMonster(Monster *monster, int type);

void addMonster(Monster **monsters, int x, int y, int type, int *monsterCount);

Monster *getMonsterAt(int x, int y, Monster **monsters, int *monsterCount); 

void setSymbol(Monster *monster);

void freeMonsters();

#endif
