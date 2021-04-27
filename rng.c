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
	int number;
	number = ((int)pcg32_boundedrand_r(&rng, end) + start);
	return number;
}
