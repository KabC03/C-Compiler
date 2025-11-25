#include "Map.h"


typedef struct Item {
    void *key, *value;
    size_t keySize, valueSize;
} Item;
typedef struct Bucket {

    Vector items;

} Bucket;


static inline size_t internal_hash(const void *key, size_t keySize, size_t numBuckets) {

    size_t hash = 5381;
    for(size_t i = 0; i < keySize; i++) {
        hash = ((hash << 5) + hash) + ((uint8_t*)(key))[i];
    }

    return hash % numBuckets;
}

bool map_init(Map *map, size_t numBuckets) {
    vector_init(&(map->buckets), sizeof(Bucket));
    if(!vector_resize(&(map->buckets), numBuckets)) {
        return false;
    }
    for(size_t i = 0; i < numBuckets; i++) {
        Bucket *bucket = vector_get_index(&(map->buckets), i);
        vector_init(&(bucket->items), sizeof(Item));
    }

    return true;
}


void *map_insert(Map *map, const void *key, size_t keySize, const void *value, size_t valueSize) {

    size_t bucketIndex = internal_hash(key, keySize, vector_get_size(&(map->buckets)));
    Bucket *bucket = vector_get_index(&(map->buckets), bucketIndex);

    void *keyInMap = malloc(keySize);
    void *valueInMap = malloc(valueSize);
    if(!keyInMap || !valueInMap) {
        goto cleanup;
    }
    memcpy(keyInMap, key, keySize);
    memcpy(valueInMap, value, valueSize);
    Item new = {
        .key = keyInMap, .value = valueInMap,
        .keySize = keySize, .valueSize = valueSize
    };

    if(!vector_push_back(&(bucket->items), &new)) {
        goto cleanup;
    }

    return valueInMap;

    cleanup:
    if(keyInMap != NULL) {
        free(keyInMap);
    }
    if(valueInMap != NULL) {
        free(valueInMap);
    }
    return NULL;
}


void *map_find(Map *map, void *key, size_t keySize) {
    size_t hash = internal_hash(key, keySize, vector_get_size(&(map->buckets)));
    Bucket *bucket = vector_get_index(&(map->buckets), hash);

    for(size_t i = 0; i < vector_get_size(&(bucket->items)); i++) {
        Item *item = vector_get_index(&(bucket->items), i);
        if(item->keySize == keySize) {
            if(memcmp(item->key, key, keySize) == 0) {
                return item->value;
            }
        }
    }

    return NULL;
}

void *map_set(Map *map, void *key, size_t keySize, void *value, size_t valueSize) {

    size_t hash = internal_hash(key, keySize, vector_get_size(&(map->buckets)));
    Bucket *bucket = vector_get_index(&(map->buckets), hash);

    Item *item = NULL;
    for(size_t i = 0; i < vector_get_size(&(bucket->items)); i++) {
        Item *temp = vector_get_index(&(bucket->items), i);
        if(temp->keySize == keySize) {
            if(memcmp(temp->key, key, keySize) == 0) {
                item = temp;
                break;
            }
        }
    }

    void *newValue = malloc(valueSize); //Realloc copies old data (wasteful)
    if(!newValue) {
        return NULL;
    } else {
        memcpy(newValue, value, valueSize);
        free(item->value);
        item->value = newValue;
    }

    item->valueSize = valueSize;
    return item->value;
}

bool map_delete(Map *map, void *key, size_t keySize) {

    size_t hash = internal_hash(key, keySize, vector_get_size(&(map->buckets)));
    Bucket *bucket = vector_get_index(&(map->buckets), hash);

    for(size_t i = 0; i < vector_get_size(&(bucket->items)); i++) {
        Item *item = vector_get_index(&(bucket->items), i);
        if(item->keySize == keySize) {
            if(memcmp(item->key, key, keySize) == 0) {
                free(item->key);
                free(item->value);
                vector_swap_and_pop(&(bucket->items), i);
                return true;
            }
        }
    }

    return false;
}


void map_destroy(Map *map) {

    for(size_t i = 0; i < vector_get_size(&(map->buckets)); i++) {
        Bucket *bucket = vector_get_index(&(map->buckets), i);

        for(size_t j = 0; j < vector_get_size(&(bucket->items)); j++) {
            Item *item = vector_get_index(&(bucket->items), j);
            free(item->key);
            free(item->value);
        }
        vector_destroy(&(bucket->items));
    }
    vector_destroy(&(map->buckets));
    return;
}



