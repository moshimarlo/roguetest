#include "map_generator.h"
#include "rng.h"
#include "symbols.h"
#include "window.h"

#include <stdlib.h>
#include <string.h>

//Fill the map with walls, to be tunneled by randomizeMap
void init_map(int **map, int max_height, int max_width)
{
	for (int i = 0; i < max_width; i++) {
		for (int j = 0; j < max_height; j++) {
			*(* (map + j) + i) = NWALL;
		}
	}
}

//Called by demo.c
void randomize_map(int **map, int max_height, int max_width, Monster ** monsters,
		  int *monster_count, Player * player)
{
	int max_rooms = 30;
	*monster_count = 0;
	for (int i = 0; i < max_rooms; i++) {
		create_room(map, max_height, max_width, monsters, monster_count, player);
	}
}

//Called by randomizeMap()
void create_room(int **map, int max_height, int max_width, Monster ** monsters,
		int *monster_count, Player * player)
{
	int i, j;
	int min_room_width = 5;
	int max_room_width = 12;
	int min_room_height = 4;
	int max_room_height = 7;
	int room_width = get_rand(min_room_width, max_room_width);
	int room_height = get_rand(min_room_height, max_room_height);
	int x1 = get_rand(1, max_width - max_room_width - 1);
	int y1 = get_rand(1, max_height - max_room_height - 1);
	int x2 = x1 + room_width;
	int y2 = y1 + room_height;
	bool unoccupied = true;

	//Check if the room's maximum x and y coordinates would go out of bounds
	while (x2 > max_width || y2 > max_height) {
		x1 = get_rand(1, max_width - max_room_width - 1);
		y1 = get_rand(1, max_height - max_room_height - 1);
		x2 = x1 + room_width;
		y2 = y1 + room_height;
	}

	//Check if new room overlaps existing room
	for (i = x1; i < x2; i++) {
		for (j = y1; j < y2; j++) {
			if (*(*(map + j) + i) == NFLOOR) {
				unoccupied = false;
			}
		}
	}

	if (unoccupied) {
		for (i = x1; i < x2; i++) {
			for (j = y1; j < y2; j++) {
				bool isWall = false;
				//Pad the room with walls
				if (i == x1 || i == x2 - 1 || j == y1
				    || j == y2 - 1) {
					*(*(map + j) + i) = NWALL;
					isWall = true;
				} else {
					*(*(map + j) + i) = NFLOOR;
				}

				if (get_rand(0, 100) > 95 && !isWall) {
					if (*monster_count < MAXMONSTERS) {
						*(*(map + j) + i) = NMONSTER;
						add_monster(monsters, j, i,
							   NKOBOLD,
							   monster_count);
						*monster_count += 1;
					}
				}
				//TODO: fix this weird shit
				if (i == y2 - y1 && j == x2 - x1) {
					player_move(player, j, i);
				}
			}
		}
	}
}

void draw_map(TCOD_Console * window, int **map, int maxRow, int maxCol,
	     Monster ** monsters)
{
	int tValue;
	char tile = '?';
	//TODO: implement FOV
	//int maxFOV = 10;

	for (int i = 0; i < maxCol; i++) {
		for (int j = 0; j < maxRow; j++) {
			tValue = *(* (map + j) + i);
			switch (tValue) {
			case NFLOOR:
				tile = FLOOR;
				break;
			case NWALL:
				tile = WALL;
				break;
			case NDOOR:
				tile = DOOR;
				break;
			case NSTAIRCASE:
				tile = STAIRCASE;
				break;
			case NMONSTER:
				tile = get_monster_tile(j, i, monsters);
				break;
			}

			TCOD_console_set_char(window, i, j, tile);
		}
	}
}

void free_map(int **map, int max_height)
{
	for (int i = 0; i < max_height; i++) {
		free(map[i]);
	}
	free(map);
}
