#include "Hashmap.h"



typedef struct Item {

    void *key;
    void *value;

} Item;

typedef struct Bucket {

    Item *items;
    size_t top;
    size_t capacity;

} Bucket;


inline size_t internal_hash(const void *key, size_t keySize, size_t numBuckets) {

    size_t hash = 5381;
    for(size_t i = 0; i < keySize; i++) {
        hash = ((hash << 5) + hash) + ((uint8_t*)(key))[i];
    }

    return hash % numBuckets;
}


inline Item *internal_hashmap_find_item(Bucket *bucket, const void *key, size_t keySize) {

    for(size_t i = 0; i < bucket->top; i++) {

        if(memcmp((uint8_t*)(bucket->items[i].key), (uint8_t*)(key), 1) == 0) {
            //Check lowest byte first
            if(memcmp(bucket->items[i].key, key, keySize) == 0) {
                return &(bucket->items[i]);
            }
        }
    }
    return NULL;
}



bool hashmap_init(Hashmap *hashmap, size_t keySize, size_t valueSize, size_t numBuckets) {
    hashmap->keySize = keySize;
    hashmap->valueSize = valueSize;
    hashmap->numBuckets = numBuckets;

    if(numBuckets == 0) {
        return false;
    }

    hashmap->buckets = malloc(sizeof(Bucket) * numBuckets);
    if(hashmap->buckets == NULL) {
        return false;
    }

    Bucket bucket = {NULL, -1, -1};
    for(size_t i = 0; i < hashmap->numBuckets; i++) {
        hashmap->buckets[i] = bucket;
    }

    return true;
}

void hashmap_destroy(Hashmap *hashmap) {

    for(size_t i = 0; i < hashmap->numBuckets; i++) {

        Bucket *bucket = &(hashmap->buckets[i]);
        for(size_t j = 0; j < bucket->top; j++) {
            free(bucket->items[j].key);
            free(bucket->items[j].value);
        }   
        free(bucket->items);
    }
    free(hashmap->buckets);
    hashmap->buckets = NULL;
    hashmap->numBuckets = 0;
    return;
}


bool hashmap_delete(Hashmap *hashmap, const void *key) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);

    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {

        Item *last = &(bucket->items[bucket->top - 1]);

        if(item != last) {
            *item = *last; //Replace with top element
        }
        free(item->key);
        free(item->value);
        bucket->top--;
        return true;
    }
    //Not in map
    return false;
}


const void *hashmap_find(Hashmap *hashmap, const void *key) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);

    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {
        return item->value;
    }

    return NULL;
}


bool hashmap_set(Hashmap *hashmap, const void *key, const void *newValue) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);


    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {
        memcpy(item->value, newValue, hashmap->valueSize);
        return true;
    }

    return false;
}

bool hashmap_insert(Hashmap *hashmap, const void *key, const void *value) {

    if(hashmap_find(hashmap, key) != NULL) {
        return false;
    }

    const size_t expansionFactor = 2;
    size_t keySize = hashmap->keySize;
    size_t valueSize = hashmap->valueSize;
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);
    size_t bucketTop = bucket->top;

    void *keyPtr = malloc(keySize);
    void *valuePtr = malloc(valueSize);
    if(keyPtr == NULL || valuePtr == NULL) {
        if(keyPtr != NULL) {
            free(keyPtr);
        }
        if(valuePtr != NULL) {
            free(valuePtr);
        }
        return false;
    } else {
        memcpy(keyPtr, key, keySize);
        memcpy(valuePtr, value, valueSize);
    }

    if(bucket->top == bucket->capacity) {
        //Resize
        Item *itemTemp = realloc(bucket->items, sizeof(Item) * ((bucket->capacity * expansionFactor) + 1));
        if(itemTemp == NULL) {
            if(keyPtr != NULL) {
                free(keyPtr);
            }
            if(valuePtr != NULL) {
                free(valuePtr);
            }
            return false;
        }

        bucket->capacity = (bucket->capacity * expansionFactor) + 1;
        bucket->items = itemTemp;
    }
    bucket->items[bucketTop - 1].key = keyPtr;
    bucket->items[bucketTop - 1].value = valuePtr;
    bucket->top++;

    return true;
}

