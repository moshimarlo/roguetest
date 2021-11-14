#ifndef __MINHEAP_H_
#define __MINHEAP_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
	int x, y, from_x, from_y;
	double f, g, h;
} node_t;

typedef struct Heap {
	// The int array which stores our values
	node_t* array;
	// Total number of values in the heap
	size_t n;
	// Maximum size of the array
	size_t max;
} heap_t;

// Memory-related functions
heap_t* init_heap(void);
void free_heap(heap_t *heap);

// Heap functions
void heap_push(heap_t* heap, node_t val);
node_t heap_pop(heap_t* heap);
node_t heap_peek(heap_t* heap);

// Debugging
size_t heap_len(heap_t* heap);
bool heap_empty(heap_t* heap);
bool in_heap(heap_t* heap, node_t node);
void print_heap(heap_t *heap);

#endif
