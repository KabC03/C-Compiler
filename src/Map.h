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


bool map_init(Map *map, size_t numBuckets);
void *map_insert(Map *map, void *key, size_t keySize, void *value, size_t valueSize);
void *map_find(Map *map, void *key, size_t keySize);
void *map_set(Map *map, void *key, size_t keySize, void *value, size_t valueSize);
bool map_delete(Map *map, void *key, size_t keySize);
void map_destroy(Map *map);


#endif
