#include "vector.h"

//Initialise a vector
void vector_init(Vector *vector, size_t elementSize) {
    vector->size = 0;
    vector->capacity = 0;
    vector->elementSize = elementSize;
    vector->data = NULL;
    return;
}


//Destroy a vector and associated memory
void vector_destroy(Vector *vector) {
    vector->size = 0;
    vector->capacity = 0;
    free(vector->data);
    return;
}


//Get size of vector
size_t vector_size(Vector *vector) {
    return vector->size;
}


//Resize a vector to a new size
bool vector_resize(Vector *vector, size_t newSize) {
    uint8_t *data = vector->data;
    size_t size = vector->size;
    size_t elementSize = vector->elementSize;
    if(realloc(data, elementSize * newSize) == NULL) {
        return false;
    }

    if(size > newSize) {
        vector->size = newSize;
    }
    vector->capacity = newSize;
    return true;
}


//Get the item at a specific index in the vector
const void *vector_at(Vector *vector, size_t index) {
    size_t elementSize = vector->elementSize;
    return vector->data[elementSize * index];
}

//Set the item at a specific index in the vector
void vector_set(Vector *vector, size_t index, void *newItem) {
    size_t elementSize = vector->elementSize;
    memcpy(vector->data[elementSize * index], newItem, elementSize);
    return;
}

//Find the first occurance of item in the vector and return a pointer to it
const void *vector_find_first_occurance(Vector *vector, void *item) {
    size_t size = vector->size;
    size_t elementSize = vector->elementSize;

    for(size_t i = 0; i < size; i++) {

        const void *vectorAt = vector_at(vector, i);
        if(memcmp(vectorAt, item, elementSize) == 0) {
            return vector_at;
        }
    }
    return NULL;
}


//Push a new item to the back of the vector, resizing if necessary
bool vector_push_back(Vector *vector, void *newItem) {
    size_t capacity = vector->capacity;
    size_t size = vector->size;
    size_t elementSize = vector->elementSize;
    if(size + 1 > capacity) {
        vector_resize(vector, size + 1);
    }
    memcpy(vector->data[elementSize * (size + 1)], newItem, elementSize);
    vector->size++;
    return true;
}

