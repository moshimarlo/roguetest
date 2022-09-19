#include "astar.h"
#include "minheap.h"
#include "rng.h"
#include "map_generator.h"

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a, b) (a <= b) ? a : b

static double d_value(int i, int j);
static double heuristic(node_t node, node_t goal);
static bool goal_reached(node_t* current, node_t goal);
static bool in_open_set(heap_t* heap, node_t node);
static void reconstruct_path(point_t start, node_t* goal, node_t** map, path_t* path);

static void check_full(path_t* path);

// Heuristic calculated using octile distance
static double heuristic(node_t node, node_t goal)
{
	double dx = abs(node.x - goal.x);
	double dy = abs(node.y - goal.y);
	return dx + dy + (sqrt(2) - 2) * MIN(dx, dy);
}

static bool goal_reached(node_t* current, node_t goal)
{
	return (current->x == goal.x && current->y == goal.y);
}

static bool in_open_set(heap_t* heap, node_t node)
{
	return in_heap(heap, node);
}

void free_path(path_t* path)
{
	free(path->points);
	free(path);
}

static void check_full(path_t* path)
{
	if (path->len >= path->max) {
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

static void reconstruct_path(point_t start, node_t* goal, node_t** map, path_t* path)
{
	FILE *fp = fopen("aslog", "a");
	node_t current = *goal;
	path->len = 0;
	path->max = PATH_INITIAL_MAX;
	path->points[path->len].x = current.x;
	path->points[path->len].y = current.y;
	fprintf(fp, "------------------------------------------------------\n");
	fprintf(fp, "START: (%2d, %2d)\n", start.x, start.y);
	fprintf(fp, "GOAL: (%2d, %2d)\n", goal->x, goal->y);
	while (current.x != start.x && current.y != start.y) {
		++path->len;
		check_full(path);
		assert(current.from_x >= 0 && current.from_x < get_map_width());
		assert(current.from_y >= 0 && current.from_y < get_map_height());
		int x_distance = abs(current.x - current.from_x);
		int y_distance = abs(current.y - current.from_y);
		assert(x_distance <= 1 && y_distance <= 1);
		current = map[current.from_x][current.from_y];
		//assert(!get_monster_at(current.x, current.y));
		path->points[path->len].x = current.x;
		path->points[path->len].y = current.y;
		fprintf(fp, "#%2d: (%2d, %2d)\n", (int)path->len, current.x, current.y);
	}
	fclose(fp);
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
	node_t start = { .x = s_x, .y = s_y, .from_x = 0, .from_y = 0, .f = 0, .g = 0 };
	node_t goal = { .x = g_x, .y = g_y, .from_x = 0, .from_y = 0, .f = 0, .g = 0 };

	node_t **map;
	map = malloc(sizeof(node_t*) * max_x);
	for (int i = 0; i < max_x; i++) {
		map[i] = malloc(sizeof(node_t) * max_y);
		for (int j = 0; j < max_y; j++) {
			map[i][j].passable = false;
			map[i][j].x = i;
			map[i][j].y = j;
			map[i][j].from_x = 0;
			map[i][j].from_y = 0;
			map[i][j].f = DBL_MAX;
			map[i][j].g = DBL_MAX;
			if (is_floor(i, j)) {
				map[i][j].passable = true;
				if (i == start.x && j == start.y) {
					map[i][j].f = heuristic(start, goal);
					map[i][j].g = 0;
				}
			} 
		}
	}
	path_t *path = init_path();

	heap_t *open_set = init_heap();
	heap_push(open_set, start);
	while (!heap_empty(open_set)) {
		node_t *current = heap_peek(open_set);

		if (goal_reached(current, goal)) {
			reconstruct_path(start_coords, current, map, path);
			for (int i = 0; i < max_x; i++) free(map[i]);
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
				if (out_of_bounds(current->x+i, current->y+j)) continue;
				if (!map[current->x+i][current->y+j].passable) continue;
				node_t neighbour = map[current->x+i][current->y+j];
				double tentative_gscore = current->g + d_value(i, j);
				if (tentative_gscore < neighbour.g && neighbour.passable) {
					// This path to neighbour is better than previous
					map[current->x+i][current->y+j].from_x = current->x;
					map[current->x+i][current->y+j].from_y = current->y;
					map[current->x+i][current->y+j].g = tentative_gscore;
					map[current->x+i][current->y+j].f = tentative_gscore
								+ heuristic(neighbour, goal);
					neighbour = map[current->x+i][current->y+j];
					if (!in_open_set(open_set, neighbour)) {
						heap_push(open_set, neighbour);
					}
				}
			}
		}	
	}
	// Suitable path not found
	for (int i = 0; i < max_x; i++) free(map[i]);
	free(map);
	free_heap(open_set);
	free_path(path);
	return NULL;
}
