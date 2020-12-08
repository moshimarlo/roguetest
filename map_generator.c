#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

//Fill the map with walls, to be tunneled by randomizeMap
void initMap(int **map, int maxRow, int maxCol) {
    for (int i = 0; i < maxCol; i++) {
        for (int j = 0; j < maxRow; j++) {
            *(*(map+j) + i) = NWALL;
        }
    }
}

//Called by demo.c
void randomizeMap(int **map, int maxRow, int maxCol, Monster **monsters, int *monsterCount, Player *player) {
    int maxRooms = 30;
    *monsterCount = 0;
    for (int i = 0; i < maxRooms; i++) {
        createRoom(map, maxRow, maxCol, monsters, monsterCount, player); 
    }
}

//Called by randomizeMap()
void createRoom(int **map, int maxRow, int maxCol, Monster **monsters, int *monsterCount, Player *player) {
    int i, j;
    int minWidth = 5;
    int maxWidth = 12;
    int minHeight = 3;
    int maxHeight = 6; 
    int roomWidth = getRand(minWidth, maxWidth);
    int roomHeight = getRand(minHeight, maxHeight);
    int x1 = getRand(1, maxCol-maxWidth-1);
    int y1 = getRand(1, maxRow-maxHeight-1);
    int x2 = x1 + roomWidth;
    int y2 = y1 + roomHeight;
    bool unoccupied = true; 
    
    //Check if the room's maximum x and y coordinates would go out of bounds
    while (x2 > maxCol || y2 > maxRow) {
        x1 = getRand(1, maxCol-1);
        y1 = getRand(1, maxRow-1);
        x2 = x1 + roomWidth;
        y2 = y1 + roomHeight;
    }

    //Check if new room overlaps existing room
    for (i = x1; i < x2; i++) {
        for (j = y1; j < y2; j++) {
            if (*(*(map+j) + i) == NFLOOR) {
                unoccupied = false;
            }
        }
    }

    if (unoccupied) {
        for (i = x1; i < x2; i++) {
            for (j = y1; j < y2; j++) {
                if (getRand(0, 100) > 95) {
                    if (*monsterCount < MAXMONSTERS) {
                        *(*(map+j) + i) = NMONSTER;
                        addMonster(monsters, j, i, NKOBOLD, monsterCount);
                        *monsterCount += 1;
                    }
                } else {
                    *(*(map+j) + i) = NFLOOR;
                }
                //TODO: fix this weird shit
                if (i == y2 - y1 && j == x2 - x1) {
                    playerMove(player, j, i);
                }
            }
        }
        //while (monsterCount > 0) {
        //    monX = getRand(x1, x2-1);
        //    monY = getRand(y1, y2-1);
        //    *(*(map+monX) + monY) = NMONSTER;
        //    monsterCount--;
        //}
    }
}

void drawMap(WINDOW *window, int **map, int maxRow, int maxCol) {
    int tValue;
    char tile = '?';
    //TODO: implement FOV
    //int maxFOV = 10;
    
    for(int i = 0; i < maxCol; i++){
        for(int j = 0; j < maxRow; j++){
            tValue = *(*(map+j) + i); 
            switch (tValue) {
                case NFLOOR:
                    tile = FLOOR;
                    break;
                case NWALL:
                    tile = WALL;
                    break;
                case NDOOR:
                    tile = DOOR;
                    break;
                case NSTAIRCASE:
                    tile = STAIRCASE;
                    break;
                case NMONSTER:
                    tile = KOBOLD;
                    break;
            }
            mvwaddch(window, j, i, tile);
        }
    }
}

void freeMap(int **map) {
    free(map);
}
