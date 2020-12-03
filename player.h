#ifndef PLAYER_H
#define PLAYER_H
typedef struct Player{
    int playerX, playerY;
} Player;
#endif

void collisionTest(Player *player, int **map, int row, int col, int prevX, int prevY); 

void resetPlayerPos(Player *player, int prevX, int prevY); 
