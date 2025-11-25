#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef struct Bucket Bucket;
typedef struct Hashmap {

    size_t keySize;
    size_t valueSize;

    Bucket *buckets;
    size_t numBuckets;
    //Use array of arrays, avoid pointer chasing

} Hashmap;



bool hashmap_init(Hashmap *hashmap, size_t keySize, size_t valueSize, size_t numBuckets);
void hashmap_destroy(Hashmap *hashmap);
bool hashmap_delete(Hashmap *hashmap, const void *key);
const void *hashmap_find(Hashmap *hashmap, const void *key);
bool hashmap_set(Hashmap *hashmap, const void *key, const void *newValue);
bool hashmap_insert(Hashmap *hashmap, const void *key, const void *value);


#endif // HASHMAP_H
