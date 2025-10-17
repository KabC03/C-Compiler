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






#endif // HASHMAP_H
