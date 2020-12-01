#include <ncurses.h>
#include "map_generator.h"
#include "rng.h"
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

void initMap(int **map, int maxRow, int maxCol) {
    //int **temp_arr;
    //temp_arr = malloc(sizeof(int *) * maxRow); 
    //for (int i = 0; i < maxRow; i++) {
    //    temp_arr[i] = (int*)malloc(sizeof(int *) * maxCol);
    //    for (int j = 0; j < maxCol; j++) {
    //        temp_arr[i][j] = 1;
    //    }
    //}
    //map = temp_arr;
    //freeMap(temp_arr, maxRow);
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            *(*(map+i) + j) = 0;
        }
    }
}

void randomizeMap(int **map, int maxRow, int maxCol) {
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            *(*(map+i) + j) = getRand(0, 3);
        }
    }
    //int roomWidth, roomHeight;
    //int sectionWidth = maxRow/6;
    //int sectionHeight = maxCol/6;
    ////Iterate through each section of the map (6 by 6)
    ////and make a random room
    //for (int i = 0; i < sectionWidth*6; i += sectionWidth) {
    //    for (int j = 0; j < sectionHeight*6; j += sectionHeight) {
    //        createRoom(map, i, j, i + sectionWidth, j + sectionHeight);    

    //    }
    //}
}

void createRoom(int **map, int sectMinX, int sectMinY, int sectMaxX, int sectMaxY) {
    int i, j;
    int roomWidth = getRand((sectMinX),sectMaxX);
    int roomHeight = getRand((sectMinY),sectMaxY);
    int roomZeroX = sectMaxX - roomWidth;
    int roomZeroY = sectMaxY - roomHeight;
    int **newRoom = malloc(sizeof(*newRoom)*roomWidth);
    for (i = 0; i < roomWidth; i++) {
        newRoom[i] = malloc(sizeof(newRoom[i])*roomHeight);
    }
    for (i = 0; i < roomHeight; i++) {
        for (j = 0; j < roomWidth; j++) {
            if (i == 0) {
                //*(*(map+(roomZeroX+i)) + (roomZeroY+j)) = 1;
            } else if (j == 0) {
                //*(*(map+(roomZeroX+i)) + (roomZeroY+j)) = 1;
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
    //free(ptr);
}

void freeMap(int **map, int maxRow) {
    for (int i = 0; i < maxRow; i++) {
        free(map[i]);
    }
    free(map);
}
