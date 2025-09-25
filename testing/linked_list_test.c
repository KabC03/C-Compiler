#include "linked_list_test.h"
#include <assert.h>

void linked_list_test_1(void) {

    List list;
    int test_data[] = {42, -1, 999, 0, 12345};
    int buffer = 0;

    // Test initialization
    list_init(&list, sizeof(int));
    assert(list_length(&list) == 0);

    // Test push and peek
    for (size_t i = 0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
        int *node = list_push(&list, &test_data[i]);
        assert(node != NULL);
        assert(*(int*)list_peek_front(&list) == test_data[i]);
        assert(list_length(&list) == i + 1);
    }

    // Test list_at
    for (size_t i = 0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
        int *val = (int*)list_at(&list, i);
        assert(val != NULL);
        assert(*val == test_data[sizeof(test_data)/sizeof(test_data[0]) - 1 - i]);
    }

    // Test delete_index (delete middle element)
    size_t mid = list_length(&list) / 2;
    int mid_val = *(int*)list_at(&list, mid);
    list_delete_index(&list, mid);
    assert(list_length(&list) == sizeof(test_data)/sizeof(test_data[0]) - 1);
    for (size_t i = 0; i < list_length(&list); i++) {
        assert(*(int*)list_at(&list, i) != mid_val);
    }

    // Test pop
    size_t orig_len = list_length(&list);
    for (size_t i = 0; i < orig_len; i++) {
        list_pop(&list, &buffer);
        assert(buffer == test_data[orig_len - 1 - i]);
    }
    assert(list_length(&list) == 0);

    // Test delete_front on empty list (should not crash)
    list_delete_front(&list);

    // Test destroy on empty list
    list_destroy(&list);

    // Test destroy on non-empty list
    list_push(&list, &test_data[0]);
    list_push(&list, &test_data[1]);
    list_destroy(&list);
    assert(list_length(&list) == 0);
    assert(list.head == NULL);

    return;
}
