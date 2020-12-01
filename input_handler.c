#include "input_handler.h"
#include <ncurses.h>
#include "player.h"


int handleInput(Player* player){
/*Signal:
     *0 - normal
     *1 - exit
     *2 - randomise test
     * */
    int signal = 0;
    int ch;
    ch = getch();

    switch(ch){
        case KEY_RIGHT:
            player->playerY++;
            break; 
        case KEY_LEFT:
            player->playerY--;
            break;
        case KEY_UP:
            player->playerX--;
            break;
        case KEY_DOWN:
            player->playerX++;
            break;
        case KEY_A1://upper-left
            player->playerY--;
            player->playerX--;
            break;
        case KEY_HOME://upper-left?
            player->playerY--;
            player->playerX--;
            break;
        case KEY_A3://upper-right
            player->playerY++;
            player->playerX--;
            break;
        case KEY_PPAGE://pg-up
            player->playerY++;
            player->playerX--;
            break;
        case '5'://center
            signal = 2;
            break;
        case KEY_C1://lower-left
            player->playerY--;
            player->playerX++;
            break;
        case KEY_END://end
            player->playerY--;
            player->playerX++;
            break;
        case KEY_C3://lower-right
            player->playerY++;
            player->playerX++;
            break;
        case KEY_NPAGE://pg-down
            player->playerY++;
            player->playerX++;
            break;
        case 'q': 
            signal = 1;
            break;
    } 

    return signal;
}
