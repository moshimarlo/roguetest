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
    ptr_filler->x = 0;
    ptr_filler->y = 0;
    initMonster(ptr_kobold, NKOBOLD);
    initMonster(ptr_orc, NORC);
    
    for (int i = 0; i < MAXMONSTERS; i++) {
        monsters[i] = malloc(sizeof(Monster*));
        *monsters[i] = *ptr_kobold;
    }
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
    int hp = 20;
    switch (monster->type) {
        case NFILLER:
            hp = 20;
            break;
        case NKOBOLD:
            hp = 20;
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
    Monster newMonster;
    Monster *ptr_monster = &newMonster;
    initMonster(ptr_monster, type);
    memcpy(&monsters[*count], &ptr_kobold, sizeof(Monster*));
    (*monsters[*count]).x = x;
    (*monsters[*count]).y = y;
}

//Called by attack() in player.c
Monster *getMonsterAt(int x, int y, Monster **monsters, int *monsterCount) {
    for (int i = 0; i < MAXMONSTERS; i++) {
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
            break;
        case NORC:
            monster->symbol = ORC;
            break;
    }
}

void freeMonsters(Monster **monsters) {
    for (int i = 0; i < MAXMONSTERS; i++) {
        //free(&monsters.[i]);
    }
    //free(&monsters.monList);
}

