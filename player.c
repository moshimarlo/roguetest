#include "player.h"
#include "symbols.h"


void initPlayer(Player *player, int x, int y, int hp) {
    player->playerX = player->prevX = x;
    player->playerY = player->prevY = y;
    player->hp = hp;
}

void collisionTest(Player *player, int **map, int row, int col) {
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
    player->prevX = player->playerX;
    player->prevY = player->playerY;
}

void resetPlayerPos(Player *player) {
    player->playerX = player->prevX;
    player->playerY = player->prevY;
}
