#include "vector_test.h"
#include "vector_test.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../src/vector.h"

void vector_test_1(void) {
    Vector v;
    int arr[1000];
    for (int i = 0; i < 1000; ++i) arr[i] = i;

    // Init with element size
    vector_init(&v, sizeof(int));
    assert(vector_size(&v) == 0);

    // Push 1000 elements
    for (int i = 0; i < 1000; ++i) {
        assert(vector_push_back(&v, &arr[i]) == true);
        assert(((int*)vector_at(&v, i))[0] == i);
    }
    assert(vector_size(&v) == 1000);

    // Overwrite some elements
    int val = -12345;
    for (int i = 0; i < 1000; i += 100) {
        vector_set(&v, i, &val);
        assert(((int*)vector_at(&v, i))[0] == val);
    }

    // Find first occurrence of overwritten value
    const void *found = vector_find_first_occurrence(&v, &val);
    assert(found != NULL);
    assert(*(int*)found == val);

    // Resize down
    assert(vector_resize(&v, 10) == true);
    assert(vector_size(&v) == 10);

    // Resize up
    assert(vector_resize(&v, 2000) == true);
    assert(v.capacity == 2000);

    // Push more after resize
    for (int i = 10; i < 2000; ++i) {
        assert(vector_push_back(&v, &i) == true);
    }
    assert(vector_size(&v) == 2000);

    // Destroy and check
    vector_destroy(&v);
    assert(v.data == NULL);
    assert(v.size == 0);
    assert(v.capacity == 0);

    return;
}


