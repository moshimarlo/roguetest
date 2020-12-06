#include "monster.h"
#include "symbols.h"

#include <stdlib.h>
#include <string.h>

static Monster kobold;
static Monster *ptr_kobold = &kobold;

static MonsterList monsters;

void loadMonsters() {
    monsters.numMonsters = 0;
    monsters.monList = malloc(sizeof(Monster)*90);
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
        case NKOBOLD:
            hp = 10;
            break;
        case NORC:
            hp = 20;
            break;
    }
    monster->alive = true;
}

void addMonster(int x, int y, int type) {
    Monster tempMonster;
    Monster *ptr_monster;
    switch (type) {
        case NKOBOLD:
            tempMonster = kobold;
            ptr_monster = ptr_kobold; 
            break;
    }
    initMonster(ptr_monster, type);
    tempMonster.x = x;
    tempMonster.y = y;
    memcpy(&tempMonster, &monsters.monList[monsters.numMonsters], sizeof(Monster));
    monsters.numMonsters++;
}

void setSymbol(Monster *monster) {
    switch (monster->type) {
        case NKOBOLD:
            monster->symbol = KOBOLD;
    }
}

