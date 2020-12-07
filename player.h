#include "monster.h"

#ifndef PLAYER_H
#define PLAYER_H
typedef struct Player {
    int playerX, playerY, prevX, prevY;
    int hp;
} Player;

void initPlayer(Player *player, int x, int y, int hp);

void collisionTest(Player *player, int **map, int row, int col, Monster **monsters, int *monsterCount); 

void playerAttack(Player *player, int **map, int x, int y, Monster **monsters, int *monsterCount); 

void playerMove(Player *player, int x, int y);

void resetPlayerPos(Player *player); 

#endif
