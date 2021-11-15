// Crude implementation of a binary (min) heap
// by Mikko
//
#include "minheap.h"
#include <stdio.h>

// Starting size of the array storing values of the heap
#define INITIAL_SIZE 64
// Left child of index i is i*2, so to make this a bit faster,
// shift i left.
#define LEFT_CHILD(i) (i << 1)
#define RIGHT_CHILD(i) (LEFT_CHILD(i) + 1)
// Same as above, but parent is i/2.
#define PARENT(i) (i >> 1)

#define MIN(a, b) (a <= b) ? a : b
#define MAX(a, b) (a >= b) ? a : b 

node_t error_node = { .x = -1, .y = -1, .f = 0, .g = 0 };

// Internal check to see if heap has reached its size limit and
// double capacity if so
static void check_full(heap_t *heap)
{
	if (heap->n >= heap->max) {
		heap->max *= 2;
		heap->array = realloc(heap->array, sizeof(node_t)*heap->max);
	}
}

heap_t* init_heap(void)
{
	heap_t *heap = malloc(sizeof(*heap));
	heap->array = malloc(sizeof(node_t) * INITIAL_SIZE);
	heap->n = 0;
	heap->max = INITIAL_SIZE;
	return heap;
}

void free_heap(heap_t *heap)
{
	free(heap->array);
	free(heap);
}

void heap_push(heap_t *heap, node_t val)
{
	++heap->n;
	check_full(heap);
	heap->array[heap->n] = val;

	int cur = heap->n;
	if (heap->n > 1) {
		while (cur > 1 && heap->array[PARENT(cur)].f > heap->array[cur].f) {
			node_t temp = heap->array[PARENT(cur)];
			heap->array[PARENT(cur)] = heap->array[cur];
			heap->array[cur] = temp;
			cur = PARENT(cur);
		}
	}
}

node_t heap_pop(heap_t *heap)
{
	int cur = 1;
	node_t ret = heap->array[cur];
	if (heap->n < 1) return error_node;
	heap->array[1] = heap->array[heap->n--];
	unsigned int left = LEFT_CHILD(cur);
	unsigned int right = RIGHT_CHILD(cur);

	while (heap->array[cur].f > heap->array[left].f
		|| heap->array[cur].f > heap->array[right].f) {
		// Find which of the two child nodes is smaller
		int min_val = MIN(heap->array[left].f, heap->array[right].f);
		int min_index = (heap->array[left].f == min_val) ? left : right;
		// Swap value of parent node with that of smaller child node
		node_t temp = heap->array[cur];
		heap->array[cur] = heap->array[min_index];
		heap->array[min_index] = temp;
		cur = min_index;
		left = LEFT_CHILD(cur);
		right = RIGHT_CHILD(cur);
		if (left > heap->n || right > heap->n) break;
	}
	return ret;
}

node_t heap_peek(heap_t *heap)
{
	if (heap->n >= 1) return heap->array[1];
	return error_node;
	//return (heap->n >= 1) ? *(heap->array[1]) : error_node;
}

size_t heap_len(heap_t *heap)
{
	return heap->n;
}

bool heap_empty(heap_t *heap)
{
	return (heap->n <= 0);
}

bool in_heap(heap_t *heap, node_t node)
{
	for (unsigned int i = 1; i <= heap->n; i++) {
		if (heap->array[i].x == node.x && heap->array[i].y == node.y) {
			return true;
		}
	}
	return false;
}

void print_heap(heap_t *heap)
{
	for (unsigned int i = 1; i <= heap->n; i++) {
		printf("%f ", heap->array[i].f);
	}
	printf("\n");
}
