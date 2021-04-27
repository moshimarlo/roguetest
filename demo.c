#include "pcg_basic.h"
#include "rng.h"
#include "symbols.h"
#include "input_handler.h"
#include "map_generator.h"
#include "player.h"
#include "monster.h"
#include "window.h"

#include <libtcod.h>

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

	//Initialise libtcod 
	int row = 50;
	int col = 80;
	TCOD_console_init_root(80, 50, "roguetest", false, TCOD_RENDERER_OPENGL);
	TCOD_console_set_default_background(NULL, TCOD_black);
	TCOD_console_set_default_foreground(NULL, TCOD_white);


	//Window
	game_win = TCOD_console_new(80, 50);
	debug_win = TCOD_console_new(20, 3);

	TCOD_console_set_char(game_win, ptr_player->playerX, ptr_player->playerY, PLAYER_SYMBOL); 

	//Allow room for window
	row -= WINDOW_HEIGHT;

	//Initialise map
	int **map = malloc(sizeof(*map)*row);
	for (int i = 0; i < row; i++) {
	map[i] = malloc(sizeof(map[i])*col);
	}

	initMap(map, row, col);
	randomizeMap(map, row, col, monsters, ptr_monster_count, ptr_player);
	drawMap(game_win, map, row, col, monsters);

	TCOD_console_clear(game_win);
	TCOD_console_clear(debug_win);
	TCOD_console_blit(game_win, 0, 0, 0, 0, NULL, 0, 0, 1.0, 1.0);
	TCOD_console_blit(debug_win, 0, 0, 0, 0, NULL, 0, 48, 1.0, 1.0);

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
		
		/*If player tries to move outside the screen or into a wall, reset
		  coordinates to stored value*/
		collisionTest(ptr_player, map, row, col, monsters);

		//Clear screen after input
		TCOD_console_clear(game_win);
		TCOD_console_clear(debug_win);

		//Draw map
		drawMap(game_win, map, row, col, monsters);

		printBuffer(debug_buffer, debug_win);

		//Draw character
		TCOD_console_set_char(game_win, ptr_player->playerX, ptr_player->playerY, PLAYER_SYMBOL);

		TCOD_console_blit(game_win, 0, 0, 0, 0, NULL, 0, 0, 1.0, 1.0);
		TCOD_console_blit(debug_win, 0, 0, 0, 0, NULL, 0, 48, 1.0, 1.0);
	       
		usleep(DELAY);
    }	

    freeMap(map, row);
    freeMonsters(monsters);
}


