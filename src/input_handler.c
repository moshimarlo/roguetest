#include "input_handler.h"
#include "game_state.h"
#include "player.h"
#include <ncurses.h>

int handle_input(){
/*Signal:
     *0 - normal
     *1 - exit
     *2 - randomise test
     * */
    int state = AWAIT_INPUT;
    int ch = getch();

    switch(ch){
	case 'l':
	case KEY_RIGHT: //right
		player_move(1, 0);
		break; 
	case 'h':
	case KEY_LEFT: //left
		player_move(-1, 0);
		break;
	case 'k':
	case KEY_UP: //up
		player_move(0, -1);
		break;
	case 'j':
	case KEY_DOWN: //down
		player_move(0, 1);
		break;
	case 'y':
	//case KEY_A1://upper-left
	case KEY_HOME:
		player_move(-1, -1);
		break;
	case 'u':
	//case KEY_A3://upper-right
	case KEY_PPAGE:
		player_move(1, -1);
		break;
	case '5'://randomise
		state = RANDOMIZE;
		break;
	case KEY_B2:
		break;
	//case KEY_C1://lower-left
	case 'b':
	case KEY_END:
		player_move(-1, 1);
		break;
	//case KEY_C3://lower-right
	case 'n':
	case KEY_NPAGE:
		player_move(1, 1);
		break;
	case 'q': 
		state = QUIT;
		break;
    } 
    return state;
}
