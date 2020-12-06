#include <ncurses.h>

#include "rng.h"
#include "pcg_basic.h"
#include "symbols.h"
#include "input_handler.h"
#include "map_generator.h"
#include "player.h"
#include "monster.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DELAY 20000
#define MAXMONSTERS 90;

int main(int argc, char *argv[]) {
    //Initialise randomisation
    initRand();

    //Initialise player values
    Player player;
    Player *ptr_player = &player;
    initPlayer(ptr_player, 10, 10, 100);

    //Initialise monsters
    loadMonsters();

    //Input-related variables
    int ch;
    int inputSig = 0;

    //Initialise curses
    int row;
    int col;
    initscr();
    getmaxyx(stdscr,row,col);
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    mvprintw(ptr_player->playerY, ptr_player->playerX, PLAYER_SYMBOL);

    //Initialise map
    int **map;
    map = (int**)malloc(sizeof(*map)*row);
    for (int i = 0; i < row; i++) {
        map[i] = (int*)malloc(sizeof(map[i]) * col);
    }
    initMap(map, row, col);
    randomizeMap(map, row, col);
    drawMap(map, row, col);
    
    //MAIN GAME LOOP
    while (inputSig != 1){
        //Store player's previous position
        int prevX = ptr_player->playerX;
        int prevY = ptr_player->playerY;
        inputSig = handleInput(ptr_player);
        
        if (inputSig == 2) {
            initMap(map, row, col);
            randomizeMap(map, row, col);
        }
        
        //If player tries to move outside the screen or into a wall, reset
        //coordinates to stored value
        collisionTest(ptr_player, map, row, col);

        //Clear screen after input
        clear();

        //Draw map
        drawMap(map, row, col);

        //Draw character
        mvprintw(ptr_player->playerX, ptr_player->playerY, PLAYER_SYMBOL); 

        refresh();
       
        usleep(DELAY);
    }	

    freeMap(map, row);
    endwin();
}


