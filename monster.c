#include "monster.h"
#include "symbols.h"

#include <stdlib.h>
#include <string.h>

static Monster filler;
static Monster *ptr_filler = &filler;
static Monster kobold;
static Monster *ptr_kobold = &kobold;
static Monster orc;
static Monster *ptr_orc = &orc;


void loadMonsters(Monster **monsters) {
    initMonster(ptr_filler, NFILLER);
    initMonster(ptr_kobold, NKOBOLD);
}


//typedef struct Monster {
//    int x, y;     - set by addMonster
//    int type;     - set by initMonster
//    char symbol;  - set by setSymbol, called by initMonster
//    int hp;       - set by initMonster
//    bool alive;   - set by initMonster
//} Monster;
void initMonster(Monster *monster, int type) {
    monster->type = type;
    setSymbol(monster);
    int hp = 0;
    switch (monster->type) {
        case NFILLER:
            hp = -1;
        case NKOBOLD:
            hp = 10;
            break;
        case NORC:
            hp = 20;
            break;
    }
    monster->hp = hp;
    monster->alive = true;
}

//Called by createRoom() in map_generator.c 
void addMonster(Monster **monsters, int x, int y, int type, int *count) {
    Monster *tempMonster;
    Monster *ptr_monster = monsters[*count];
    int isFiller = 0;
    switch (type) {
        case NFILLER:
            tempMonster = ptr_filler;
            isFiller = 1;
        case NKOBOLD:
            tempMonster = ptr_kobold;
            break;
        case NORC:
            tempMonster = ptr_orc;
    }

    initMonster(tempMonster, type);
    tempMonster->x = x;
    tempMonster->y = y;
    memcpy(&monsters[*count], &tempMonster,sizeof tempMonster);
}

//Called by attack() in player.c
Monster *getMonsterAt(int x, int y, Monster **monsters, int *monsterCount) {
    Monster errorMonster;
    Monster *ptr_error;
    for (int i = 0; i < *monsterCount; i++) {
        if (monsters[i]->x == x && monsters[i]->y == y) {
            return monsters[i];
        }
    }
    return ptr_orc;
}

void setSymbol(Monster *monster) {
    switch (monster->type) {
        case NKOBOLD:
            monster->symbol = KOBOLD;
        case NORC:
            monster->symbol = ORC;
    }
}

void freeMonsters() {
    for (int i = 0; i < 90; i++) {
        //free(&monsters.monList[i]);
    }
    //free(&monsters.monList);
}

