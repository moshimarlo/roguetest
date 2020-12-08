#include "rng.h"
#include "pcg_basic.h"
#include "symbols.h"
#include "input_handler.h"
#include "map_generator.h"
#include "player.h"
#include "monster.h"
#include "window.h"

#include <ncurses.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DELAY 20000
#define WINDOW_HEIGHT 2 

char debug_buffer[20];

int main(int argc, char *argv[]) {
    //Initialise randomisation
    initRand();

    //Initialise player values
    Player player;
    Player *ptr_player = &player;
    initPlayer(ptr_player, 10, 10, 100);

    //Initialise monsters
    int monsterCount = 0;
    int *ptr_monster_count = &monsterCount;
    Monster **monsters = malloc(sizeof(**monsters)*MAXMONSTERS);
    for (int i = 0; i < MAXMONSTERS; i++) {
        monsters[i] = malloc(sizeof(*monsters));
    }

    loadMonsters(monsters);

    //Input-related variables
    int inputSig = 0;

    //Debug output
    //char debug_buffer[20];

    //Initialise curses
    int row;
    int col;
    initscr();
    getmaxyx(stdscr,row,col);
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);


    //Window
    game_win = newwin(row-WINDOW_HEIGHT, col, 0, 0); 
    debug_win = newwin(WINDOW_HEIGHT, col, row-WINDOW_HEIGHT, 0);

    mvwaddch(game_win, ptr_player->playerY, ptr_player->playerX, PLAYER_SYMBOL);

    //Allow room for window
    row -= WINDOW_HEIGHT;

    //Initialise map
    int **map = malloc(sizeof(*map)*row);
    for (int i = 0; i < row; i++) {
        map[i] = malloc(sizeof(map[i])*col);
    }
    //map = (int**)malloc(sizeof(*map)*row);
    //for (int i = 0; i < row; i++) {
    //    map[i] = (int*)malloc(sizeof(map[i]) * col);
    //}

    initMap(map, row, col);
    randomizeMap(map, row, col, monsters, ptr_monster_count, ptr_player);
    drawMap(game_win, map, row, col, monsters);

    wrefresh(game_win);
    wrefresh(debug_win);
    
    //MAIN GAME LOOP
    while (inputSig != 1){
        char str1[20] = "Hello";
        printToBuffer(debug_buffer, str1);
        //Store player's previous position
        inputSig = 0;
        inputSig = handleInput(ptr_player);
        
        if (inputSig == 2) {
            initMap(map, row, col);
            //TODO: delete all monsters from array and reset count to zero
            monsterCount = 0;
            randomizeMap(map, row, col, monsters, ptr_monster_count, ptr_player);
        }
        
        //If player tries to move outside the screen or into a wall, reset
        //coordinates to stored value
        collisionTest(ptr_player, map, row, col, monsters);

        //Clear screen after input
        wclear(game_win);
        wclear(debug_win);

        //Draw map
        drawMap(game_win, map, row, col, monsters);

        printBuffer(debug_buffer, debug_win);

        //Draw character
        mvwaddch(game_win, ptr_player->playerX, ptr_player->playerY, PLAYER_SYMBOL); 

        wrefresh(game_win);
        wrefresh(debug_win);
       
        usleep(DELAY);
    }	

    freeMap(map, row);
    freeMonsters(monsters);
    endwin();
}


