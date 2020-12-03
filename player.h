#ifndef PLAYER_H
#define PLAYER_H
typedef struct Player {
    int playerX, playerY, prevX, prevY;
    int hp;
} Player;
#endif

void initPlayer(Player *player, int x, int y, int hp);

void collisionTest(Player *player, int **map, int row, int col); 

void resetPlayerPos(Player *player); 
