#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct Vector {

    void *data;
    size_t dataSize;
    size_t capacity;
    size_t top;

} Vector;


void vector_init(Vector *vector, size_t dataSize);
bool vector_push_back(Vector *vector, void *item);
bool vector_resize(Vector *vector, size_t newSize);
size_t vector_get_size(Vector *vector);
size_t vector_find_element_index(Vector *vector, void *element);
bool vector_swap(Vector *vector, size_t index1, size_t index2);
bool vector_swap_and_pop(Vector *vector, size_t index);
void *vector_get_index(Vector *vector, size_t index);
bool vector_data_is_equal(Vector *vector, void *data, size_t dataSize);
void vector_destroy(Vector *vector);
bool vector_set_index(Vector *vector, size_t index, void *item);

#endif // VECTOR_H

