#ifndef MAP_H
#define MAP_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Vector.h"

typedef struct Bucket Bucket;
typedef struct Map {
    Vector buckets;
} Map;


#endif
