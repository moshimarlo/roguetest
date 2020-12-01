#include "rng.h"
#include "pcg_basic.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>


pcg32_random_t rng;

void initRand() {
    //Set the seed for RNG
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 
    (intptr_t)&rng);
}

int getRand(int start, int end) {
    int number;
    number = ((int)pcg32_boundedrand_r(&rng, end) + start);
    return number;
}
