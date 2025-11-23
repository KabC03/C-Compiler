#include "Vector.h"





void vector_init(Vector *vector, size_t dataSize) {

    vector->capacity = 0;
    vector->top = 0;
    vector->data = NULL;
    vector->dataSize = dataSize;
    return;
}

void *vector_get_index(Vector *vector, size_t index) {

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

bool vector_swap(Vector *vector, size_t index1, size_t index2) { //NOTE - could use XOR trick

    void *temp = malloc(vector->dataSize);
    if(!temp) {
        return false;
    }
    void *item1 = vector_get_index(vector, index1);
    void *item2 = vector_get_index(vector, index2);
    memcpy(temp, item1, vector->dataSize);
    memcpy(item1, item2, vector->dataSize);
    memcpy(item2, temp, vector->dataSize);

    free(temp);
    return true;
}

size_t vector_find_element_index(Vector *vector, void *element) {
    for(size_t i = 0; i < vector_get_size(vector); i++) {
        void *item = vector_get_index(vector, i);
        if(memcmp(item, element, vector->dataSize) == 0) {
            return i;
        }
    }
    return -1;
}

bool vector_swap_and_pop(Vector *vector, size_t index) {
    return vector_swap(vector, index, vector_get_size(vector));
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


bool vector_data_is_equal(Vector *vector, void *data, size_t dataSize) {
    if(memcmp(vector->data, data, dataSize) == 0) {
        return true;
    }
    return false;
}

bool vector_set_index(Vector *vector, size_t index, void *item) {

    if(index >= vector->top) {
        return false;
    } 
    void *dest = ((uint8_t*)vector->data) + (index * vector->dataSize); 
    memcpy(dest, item, vector->dataSize);
    return true;
}


