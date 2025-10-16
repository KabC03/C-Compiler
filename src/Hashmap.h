#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



typedef struct Hashmap Hashmap;


void hashmap_init(Hashmap *hashmap, size_t keySize, size_t valueSize);
void hashmap_destroy(Hashmap *hashmap);
bool hashmap_insert(Hashmap *hashmap, void *key, void *value);
bool hashmap_delete(Hashmap *hashmap, void *key);
const void *hashmap_find(Hashmap *hashmap, void *key);
bool hashmap_set(Hashmap *hashmap, void *key, void *newValue);




#endif // HASHMAP_H
