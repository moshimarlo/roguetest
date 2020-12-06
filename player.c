#include "player.h"
#include "symbols.h"
#include "monster.h"
#include "rng.h"


void initPlayer(Player *player, int x, int y, int hp) {
    player->playerX = player->prevX = x;
    player->playerY = player->prevY = y;
    player->hp = hp;
}

//Called by demo.c
void collisionTest(Player *player, int **map, int row, int col, Monster **monsters, int *monsterCount) {
    if(player->playerX > row-1 || player->playerX < 0) {
        resetPlayerPos(player);
        return;
    }
    if(player->playerY > col-1 || player->playerY < 0) {
        resetPlayerPos(player);
        return;
    }
    if (map[player->playerX][player->playerY] == NWALL) {
        resetPlayerPos(player);
        return;
    }
    if (map[player->playerX][player->playerY] == NMONSTER) {
        attack(player, map, player->playerX, player->playerY, monsters, monsterCount);
    }
    player->prevX = player->playerX;
    player->prevY = player->playerY;
}

//Called by collisionTest()
void attack(Player *player, int **map, int x, int y, Monster **monsters, int *monsterCount) {
    Monster *target = getMonsterAt(x, y, monsters, monsterCount); 
    int hitRoll = getRand(5, 10);
    target->hp -= hitRoll;
    if (target->hp <= 0) {
        *(*(map+x) + y) = NFLOOR;
    }
}

void resetPlayerPos(Player *player) {
    player->playerX = player->prevX;
    player->playerY = player->prevY;
}
