#include "input_handler.h"
#include "player.h"
#include <ncurses.h>

int handle_input(Player* player){
/*Signal:
     *0 - normal
     *1 - exit
     *2 - randomise test
     * */
    int signal = 0;
    int ch = getch();

    switch(ch){
	case KEY_RIGHT: //right
		player_move(1, 0);
		break; 
	case KEY_LEFT: //left
		player_move(-1, 0);
		break;
	case KEY_UP: //up
		player_move(0, -1);
		break;
	case KEY_DOWN: //down
		player_move(0, 1);
		break;
	//case KEY_A1://upper-left
	case KEY_HOME:
		player_move(-1, -1);
		break;
	//case KEY_A3://upper-right
	case KEY_PPAGE:
		player_move(1, -1);
		break;
	case '5'://randomise
		signal = 2;
		break;
	case KEY_B2:
		break;
	//case KEY_C1://lower-left
	case KEY_END:
		player_move(-1, 1);
		break;
	//case KEY_C3://lower-right
	case KEY_NPAGE:
		player_move(1, 1);
		break;
	case 'q': 
		signal = 1;
		break;
    } 
    return signal;
}
