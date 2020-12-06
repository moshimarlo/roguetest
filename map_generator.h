#include "monster.h"

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

void initMap(int **map, int maxRow, int maxCol);

void randomizeMap(int **map, int maxRow, int maxCol, Monster **monsters, int *monsterCount);

void createRoom(int **map, int maxRow, int maxCol, Monster **monsters, int *monsterCount); 

void drawMap(int **map, int maxRow, int maxCol);

void freeMap(int **map, int maxRow);


#endif
