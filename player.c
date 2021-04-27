#include "player.h"
#include "symbols.h"
#include "monster.h"
#include "rng.h"
#include "window.h"
#include <stdio.h>

void initPlayer(Player * player, int x, int y, int hp)
{
	player->playerX = player->prevX = x;
	player->playerY = player->prevY = y;
	player->hp = hp;
}

//Called by demo.c
void collisionTest(Player * player, int **map, int row, int col,
		   Monster ** monsters)
{
	if (player->playerX > col - 1 || player->playerX < 0) {
		resetPlayerPos(player);
		return;
	}
	if (player->playerY > row - 1 || player->playerY < 0) {
		resetPlayerPos(player);
		return;
	}
	if (*(*(map + player->playerX) + player->playerY) == NWALL) {
		resetPlayerPos(player);
		return;
	}
	if (*(*(map + player->playerX) + player->playerY) == NMONSTER) {
		playerAttack(player, map, player->playerX, player->playerY,
			     monsters);
	}
	player->prevX = player->playerX;
	player->prevY = player->playerY;
}

//Called by collisionTest()
void playerAttack(Player * player, int **map, int x, int y, Monster ** monsters)
{
	Monster *target = getMonsterAt(x, y, monsters);
	int hitRoll = 3;
	target->hp -= hitRoll;
	char output[20];
	sprintf(output, "%d", target->hp);
	printToBuffer(debug_buffer, output);
	if (target->hp <= 0) {
		*(*(map + x) + y) = NFLOOR;
		target->alive = false;
		sprintf(output, "You killed a %c", target->symbol);
		printToBuffer(debug_buffer, output);
	} else {
		resetPlayerPos(player);
	}
}

void playerMove(Player * player, int x, int y)
{
	player->playerX = x;
	player->playerY = y;
}

void resetPlayerPos(Player * player)
{
	player->playerX = player->prevX;
	player->playerY = player->prevY;
}
