#include "input_handler.h"
#include "player.h"

int handle_input(Player* player){
/*Signal:
     *0 - normal
     *1 - exit
     *2 - randomise test
     * */
    int signal = 0;
    TCOD_key_t key;
    TCOD_event_t ev;

    ev = TCOD_sys_wait_for_event(TCOD_EVENT_ANY, &key, NULL, false);

    if (key.pressed) {
	    switch(key.vk){
		case TCODK_KP6: //right
		    player->curr_x++;
		    break; 
		case TCODK_KP4: //left
		    player->curr_x--;
		    break;
		case TCODK_KP8: //up
		    player->curr_y--;
		    break;
		case TCODK_KP2: //down
		    player->curr_y++;
		    break;
		case TCODK_KP7://upper-left
		    player->curr_x--;
		    player->curr_y--;
		    break;
		case TCODK_KP9://upper-right
		    player->curr_x++;
		    player->curr_y--;
		    break;
		case TCODK_5://randomise
		    signal = 2;
		    break;
		case TCODK_KP1://lower-left
		    player->curr_x--;
		    player->curr_y++;
		    break;
		case TCODK_KP3://lower-right
		    player->curr_x++;
		    player->curr_y++;
		    break;
		case TCODK_ESCAPE: 
		    signal = 1;
		    break;
		case TCODK_CHAR:
		    switch (key.c) {
			case 'q':
				signal = 1;
				break;
		    }
	    } 
    }
    return signal;
}
