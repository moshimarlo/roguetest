#include "player.h"
#include "symbols.h"
#include "monster.h"
#include "rng.h"
#include "window.h"

#include <ncurses.h>


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
        playerAttack(player, map, player->playerX, player->playerY, monsters, monsterCount);
    }
    player->prevX = player->playerX;
    player->prevY = player->playerY;
}

//Called by collisionTest()
void playerAttack(Player *player, int **map, int x, int y, Monster **monsters, int *monsterCount) {
    Monster *target = getMonsterAt(x, y, monsters, monsterCount); 
    int hitRoll = 3; 
    target->hp -= hitRoll;
    char output[10];
    sprintf(output, "%d", target->hp);
    mvwprintw(debug_win, 0, 0, output);
    if (target->hp <= 0) {
        *(*(map+x) + y) = NFLOOR;
    } else {
        resetPlayerPos(player);
    }
}

void playerMove(Player *player, int x, int y) {
    player->playerX = x;
    player->playerY = y;
}

void resetPlayerPos(Player *player) {
    player->playerX = player->prevX;
    player->playerY = player->prevY;
}
