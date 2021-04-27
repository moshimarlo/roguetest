#include "input_handler.h"
#include "player.h"


int handleInput(Player* player){
/*Signal:
     *0 - normal
     *1 - exit
     *2 - randomise test
     * */
    int signal = 0;
    TCOD_key_t key;
    TCOD_event_t ev;

    ev = TCOD_sys_wait_for_event(TCOD_EVENT_ANY, &key, NULL, false);

    switch(key.vk){
        case TCODK_KP6: //right
            player->playerY++;
            break; 
        case TCODK_KP4: //left
            player->playerY--;
            break;
        case TCODK_KP8: //up
            player->playerX--;
            break;
        case TCODK_KP2: //down
            player->playerX++;
            break;
        case TCODK_KP7://upper-left
            player->playerY--;
            player->playerX--;
            break;
        case TCODK_KP9://upper-right
            player->playerY++;
            player->playerX--;
            break;
        case TCODK_5://randomise
            signal = 2;
            break;
        case TCODK_KP1://lower-left
            player->playerY--;
            player->playerX++;
            break;
        case TCODK_KP3://lower-right
            player->playerY++;
            player->playerX++;
            break;
        case TCODK_ESCAPE: 
            signal = 1;
            break;
    } 

    return signal;
}
