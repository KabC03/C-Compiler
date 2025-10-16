#include "Vector.h"


typedef struct Vector {

    void *data;
    size_t dataSize;
    size_t capacity;
    size_t top;

} Vector;


void vector_init(Vector *vector, size_t dataSize) {

    vector->capacity = 0;
    vector->top = 0;
    vector->data = NULL;
    vector->dataSize = dataSize;
    return;
}

const void *vector_get_index(Vector *vector, size_t index) {

    if(index >= vector->top) {
        return NULL;
    } 
    return ((uint8_t*)vector->data) + (index * vector->dataSize); 
}

bool vector_push_back(Vector *vector, void *item) {

    const size_t expansionFactor = 2;
    if(vector->top == vector->capacity) {
        //Reallocation required
        size_t newCapacity = ((vector->capacity * expansionFactor) + 1);
        void *temp = realloc(vector->data, vector->dataSize * newCapacity);
        if(temp == NULL) {
            return false;
        } else {
            vector->data = temp;
            vector->capacity = newCapacity;
        }
    }
    void *ptr = ((uint8_t*)vector->data) + (vector->top * vector->dataSize); 
    memcpy(ptr, item, vector->dataSize);
    vector->top++;
    return true;    
}

bool vector_resize(Vector *vector, size_t newSize) {
    if(newSize == 0) {
        vector_destroy(vector);
        return true;
    }
    void *temp = realloc(vector->data, newSize * vector->dataSize);
    if(temp == NULL) {
        return false;
    } else {
        vector->data = temp;
    }

    if(vector->top > newSize) {
        vector->top = newSize;
    }
    vector->capacity = newSize;
    return true;
}

size_t vector_get_size(Vector *vector) {
    return vector->top;
}

void vector_destroy(Vector *vector) {
    free(vector->data);
    vector->capacity = 0;
    vector->top = 0;
    vector->data = NULL;
    return;
}


bool vector_set_index(Vector *vector, size_t index, void *item) {

    if(index >= vector->top) {
        return false;
    } 
    void *dest = ((uint8_t*)vector->data) + (index * vector->dataSize); 
    memcpy(dest, item, vector->dataSize);
    return true;
}


