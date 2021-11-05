#include "rng.h"
#include "pcg_basic.h"
#include <time.h>
#include <stdio.h>

static pcg32_random_t rng;

void init_rand(void)
{
	//Set the seed for RNG
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t) & printf,
			(intptr_t) & rng);
}

int get_rand(int start, int end)
{
	int distance = end - start;
	return (int)pcg32_boundedrand_r(&rng, distance+1) + start;
}

int roll_dice(int num_dice, int num_sides)
{
	int result = 0;
	for (int i = 0; i < num_dice; i++) {
		result += get_rand(1, num_sides);
	}
	return result;
}
