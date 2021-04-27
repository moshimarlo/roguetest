#include "monster.h"
#include "player.h"

#include <libtcod.h>

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

void initMap(int **map, int maxRow, int maxCol);

void randomizeMap(int **map, int maxRow, int maxCol, Monster ** monsters,
		  int *monsterCount, Player * player);

void createRoom(int **map, int maxRow, int maxCol, Monster ** monsters,
		int *monsterCount, Player * player);

void drawMap(TCOD_Console * window, int **map, int maxRow, int maxCol,
	     Monster ** monsters);

void freeMap(int **map, int maxRow);

#endif
