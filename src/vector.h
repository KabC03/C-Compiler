#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef struct Vector {
    size_t size;
    size_t capacity;
    size_t elementSize;

    uint8_t *data;
} Vector;


void vector_init(Vector *vector, size_t elementSize);
void vector_destroy(Vector *vector);
size_t vector_size(Vector *vector);
bool vector_resize(Vector *vector, size_t newSize);
const void *vector_at(Vector *vector, size_t index);
void vector_set(Vector *vector, size_t index, void *newItem);
const void *vector_find_first_occurrence(Vector *vector, void *item);
bool vector_push_back(Vector *vector, void *newItem);

#endif

