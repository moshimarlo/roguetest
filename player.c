#include "player.h"
#include "symbols.h"

void collisionTest(Player *player, int **map, int row, int col, int prevX, int prevY) {
    if(player->playerX > row-1 || player->playerX < 0) {
        resetPlayerPos(player, prevX, prevY);
    }
    if(player->playerY > col-1 || player->playerY < 0) {
        resetPlayerPos(player, prevX, prevY);
    }
    if (map[player->playerX][player->playerY] == NWALL) {
        resetPlayerPos(player, prevX, prevY);
    }
}

void resetPlayerPos(Player *player, int prevX, int prevY) {
    player->playerX = prevX;
    player->playerY = prevY;
}
