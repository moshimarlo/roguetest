#ifndef ROOM_H_
#define ROOM_H_
typedef struct Room {
   int x1, y1, x2, y2;
   Room *next;
} Room;

void createRoom(int **map, int maxRow, int maxCol);


#endif
