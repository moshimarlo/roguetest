#include "astar.h"
#include "rng.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>

#define MIN(a, b) (a <= b) ? a : b

static double d_value(int i, int j);
static double heuristic(node_t node, node_t goal);
static bool goal_reached(node_t current, node_t goal);
static bool in_open_set(heap_t* heap, node_t node);
static void reconstruct_path(node_t goal, point_t start, node_t* map, path_t* path, int max_x);

static void check_full(path_t* path)

// Debugging
static void print_path(path_t *path, int max_x, int max_y);
static void print_path_size(path_t *path);

// Heuristic calculated using octile distance
static double heuristic(node_t node, node_t goal)
{
	double dx = abs(node.x - goal.x);
	double dy = abs(node.y - goal.y);
	return dx + dy + (sqrt(2) - 2) * MIN(dx, dy);
}

static bool goal_reached(node_t current, node_t goal)
{
	return (current.x == goal.x && current.y == goal.y);
}

static bool in_open_set(heap_t* heap, node_t node)
{
	return in_heap(heap, node);
}

static void print_path(path_t *path, int max_x, int max_y)
{
	for (unsigned int i = 0; i < path->len; i++) {
		int x = path->points[i].x;
		int y = path->points[i].y;
		printf("x: %d, y: %d\n", x, y);
	}
	for (int i = 0; i < max_y; i++) {
		for (int j = 0; j < max_x; j++) {
			for (unsigned int n = 0; n < path->len; n++) {
				if (path->points[n].x == j && path->points[n].y == i) {
					printf("X");
				} else {
					printf(".");
				}
			}
		}
		printf("\n");
	}
}

static void print_path_size(path_t *path)
{
	printf("Length: %d\n", (int)path->len);
	printf("Max: %d\n", (int)path->max);
}

void free_path(path_t* path)
{
	free(path->points);
	free(path);
}

static void check_full(path_t* path)
{
	if (path->len > path->max) {
		path->max *= 2;
		path->points = realloc(path->points, sizeof(point_t)*path->max);
	}
}

path_t* init_path()
{
	path_t *path = malloc(sizeof(path_t));
	path->points = malloc(sizeof(point_t) * PATH_INITIAL_MAX);
	path->len = 0;
	path->max = PATH_INITIAL_MAX;
	return path;
}

static void reconstruct_path(node_t goal, point_t start, node_t* map, path_t* path, int max_x)
{
	node_t current = goal;
	path->len = 0;	
	path->max = PATH_INITIAL_MAX;
	path->points[path->len].x = current.x;
	path->points[path->len].y = current.y;
	while (current.x != start.x && current.y != start.y) {
		path->len++;
		check_full(path);
		current = map[(current.from_x) + max_x*(current.from_y)];
		path->points[path->len].x = current.x;
		path->points[path->len].y = current.y;
	}
}

static double d_value(int i, int j)
{
	return (i == 0 || j == 0) ? 1 : sqrt(2);
}

path_t* astar(point_t start_coords, point_t goal_coords, int max_x, int max_y)
{
	int s_x = start_coords.x;
	int s_y = start_coords.y;
	int g_x = goal_coords.x;
	int g_y = goal_coords.y;
	node_t start = { .x = s_x, .y = s_y, .from_x = -1, .from_y = -1, .f = 0, .g = 0, .h = 0 };
	node_t goal = { .x = g_x, .y = g_y, .from_x = -1, .from_y = -1, .f = 0, .g = 0, .h = 0 };

	node_t *map;
	map = malloc(sizeof(node_t) * max_x * max_y);

	for (int i = 0; i < max_x; i++) {
		for (int j = 0; j < max_y; j++) {
			if (i == start.x && j == start.y) {
				map[i + max_x*j].f = heuristic(start, goal);
				map[i + max_x*j].g = 0;
			} else {
				map[i + max_x*j].f = DBL_MAX;
				map[i + max_x*j].g = DBL_MAX;
			}
			map[i + max_x*j].x = i;
			map[i + max_x*j].y = j;
			map[i + max_x*j].from_x = 0;
			map[i + max_x*j].from_y = 0;
		}
	}

	path_t *path = init_path();

	heap_t *open_set = init_heap();
	heap_push(open_set, start);
	while (!heap_empty(open_set)) {
		node_t current = heap_peek(open_set);

		if (goal_reached(current, goal)) {
			reconstruct_path(current, start_coords, map, path, max_x);
			free(map);
			free_heap(open_set);
			return path;
		}

		// Remove current node from open set
		heap_pop(open_set);

		// For each neighbour of current
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// Check if on current node
				if (i == 0 && j == 0) continue;
				// Check if neighbour out of bounds
				if (current.x == 0 && i == -1) continue;
				if (current.y == 0 && j == -1) continue;
				if (current.x == max_x-1 && i == 1) continue;
				if (current.y == max_y-1 && j == 1) continue;
				node_t neighbour = map[(current.x+i) + max_x*(current.y+j)];
				double tentative_gscore = current.g + d_value(i, j);
				if (tentative_gscore < neighbour.g) {
					// This path to neighbour is better than previous
					map[(current.x+i) + max_x*(current.y+j)].from_x = current.x;
					map[(current.x+i) + max_x*(current.y+j)].from_y = current.y;
					map[(current.x+i) + max_x*(current.y+j)].g = tentative_gscore;
					map[(current.x+i) + max_x*(current.y+j)].f = tentative_gscore
								+ heuristic(neighbour, goal);
					neighbour = map[(current.x+i) + max_x*(current.y+j)];
					if (!in_open_set(open_set, neighbour)) {
						heap_push(open_set, neighbour);
					}
				}
			}
		}	
	}
	free(map);
	free_heap(open_set);
	return path;
}

int main(void)
{
	int max_x = 10;
	int max_y = 10;
	point_t start = { .x = 0, .y = 0 };
	point_t end = { .x = 9, .y = 9 };

	path_t *path = astar(start, end, max_x, max_y);
	print_path(path, max_x, max_y);
	print_path_size(path);

	free_path(path);
	return 0;
}
