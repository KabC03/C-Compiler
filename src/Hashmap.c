#include "Hashmap.h"

typedef struct Bucket {

    Item *items;
    size_t top;
    size_t capacity;

} Bucket;

typedef struct Item {

    void *key;
    void *value;

} Item;




inline size_t internal_hash(void *key, size_t keySize, size_t numBuckets) {

    size_t hash = 5381;
    for(size_t i = 0; i < keySize; i++) {
        hash = ((hash << 5) + hash) + i;
    }

    return hash % numBuckets;
}


inline Item *internal_hashmap_find_item(Bucket *bucket, void *key, size_t keySize) {

    for(size_t i = 0; i < bucket->top; i++) {

        if(memcmp((uint8_t*)(&(bucket->items[i].key)), (uint8_t*)(key), 1 == 0)) {
            //Check lowest byte first
            if(memcmp(&(bucket->items[i].key), key, keySize) == 0) {
                return &(bucket->items[i]);
            }
        }
    }
    return NULL;
}



void hashmap_init(Hashmap *hashmap, size_t keySize, size_t valueSize) {
    hashmap->keySize = keySize;
    hashmap->valueSize = valueSize;
    hashmap->buckets = NULL;

    Bucket bucket = {NULL, NULL, 0, 0};
    for(size_t i = 0; i < hashmap->numBuckets; i++) {
        hashmap->buckets[i] = bucket;
    }

    return;
}

void hashmap_destroy(Hashmap *hashmap) {

    for(size_t i = 0; i < hashmap->numBuckets; i++) {

        Bucket bucket = hashmap->buckets[i];
        free(bucket.items);
    }
    free(hashmap->buckets);
    hashmap->buckets = NULL;
}


bool hashmap_delete(Hashmap *hashmap, void *key) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);

    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {
        *item = bucket->items[bucket->top]; //Replace with top element
        return true;
    }
    //Not in map
    return false;
}


const void *hashmap_find(Hashmap *hashmap, void *key) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);

    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {
        return item->value;
    }

    return false;
}


bool hashmap_set(Hashmap *hashmap, void *key, void *newValue) {
    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);


    Item *item = internal_hashmap_find_item(bucket, key, hashmap->keySize);
    if(item != NULL) {
        memcpy(item->value, newValue, hashmap->valueSize);
        return true;
    }

    return false;
}

bool hashmap_insert(Hashmap *hashmap, void *key, void *value) {

    if(hashmap_find(hashmap, key) != NULL) {
        return false;
    }

    const size_t expansionFactor = 2;
    size_t keySize = hashmap->keySize;
    size_t valueSize = hashmap->valueSize;
    size_t numBuckets = hashmap->numBuckets;

    size_t index = internal_hash(key, hashmap->keySize, hashmap->numBuckets);
    Bucket *bucket = &(hashmap->buckets[index]);
    size_t bucketTop = bucket->top;


    if(bucket->top == bucket->capacity) {
        //Resize
        void *itemTemp = malloc(sizeof(Item) * ((bucket->capacity * expansionFactor) + 1));

        bucket->items = itemTemp;
        bucket->capacity = (bucket->capacity * expansionFactor) + 1;
    }

    memcpy(bucket->items + (bucketTop * keySize), key, sizeof(Item));
    bucket->top++;

    return true;
}

