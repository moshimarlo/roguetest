#ifndef __ASTAR_H_
#define __ASTAR_H_

#include <stdlib.h>
#include "minheap.h"

#define PATH_INITIAL_MAX 32

typedef struct Point {
	int x, y;
} point_t;

typedef struct Path {
	point_t *points;
	size_t len;
	size_t max;
} path_t;

path_t* astar(point_t start_coords, point_t goal_coords, int max_x, int max_y);
void free_path(path_t* path);

#endif
