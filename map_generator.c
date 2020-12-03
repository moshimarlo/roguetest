#include <ncurses.h>
#include "map_generator.h"
#include "rng.h"
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

//Fill the map with walls, to be tunneled by randomizeMap
void initMap(int **map, int maxRow, int maxCol) {
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            *(*(map+i) + j) = NWALL;
        }
    }
}

void randomizeMap(int **map, int maxRow, int maxCol) {
    int maxRooms = 30;
    for (int i = 0; i < maxRooms; i++) {
        createRoom(map, maxRow, maxCol); 
    }
}

void createRoom(int **map, int maxRow, int maxCol) {
    int i, j;
    int minWidth = 5;
    int minHeight = 3;
    int maxHeight = 6; 
    int maxWidth = 12;
    int roomWidth = getRand(minWidth,maxWidth);
    int roomHeight = getRand(minHeight,maxHeight);
    int x1 = getRand(1, maxRow-1);
    int y1 = getRand(1, maxCol-1);
    int x2 = x1 + roomHeight;
    int y2 = y1 + roomWidth;
    bool unoccupied = true; 
    
    //Check if the room's maximum x and y coordinates would go out of bounds
    while (x2 > maxRow || y2 > maxCol) {
        x1 = getRand(1, maxCol-1);
        y1 = getRand(1, maxRow-1);
        x2 = x1 + roomHeight;
        y2 = y1 + roomWidth;
    }

    //Check if new room overlaps existing room
    for (i = x1; i < x2; i++) {
        for (j = y1; j < y2; j++) {
            if (*(*(map+i) + j) == NFLOOR) {
                unoccupied = false;
            }
        }
    }

    if (unoccupied) {
        for (i = x1; i < x2; i++) {
            for (j = y1; j < y2; j++) {
                *(*(map+i) + j) = NFLOOR;
            }
        }
    }
}

void drawMap(int **map, int maxRow, int maxCol) {
    int tValue;
    char tile;
    int **ptr = &map[0]; 
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            tValue = map[i][j]; 
            if (tValue == NFLOOR) {
                tile = FLOOR;
            } else if (tValue == NWALL) {
                tile = WALL;
            } else if (tValue == NDOOR) {
                tile = DOOR;
            } else if (tValue == NSTAIRCASE) {
                tile = STAIRCASE;
            } else {
                tile = '?';
            }
            mvaddch(i, j, tile);
        }
    }
}

void freeMap(int **map, int maxRow) {
    for (int i = 0; i < maxRow; i++) {
        free(map[i]);
    }
    free(map);
}
