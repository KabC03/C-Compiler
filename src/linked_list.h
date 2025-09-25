#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

struct ListNode {
    struct ListNode *next;
    uint8_t data[];
};
typedef struct List {
    struct ListNode *head;
    size_t elementSize;
    size_t length;
} List;

void list_init(List *list, size_t elementSize);
void list_destroy(List *list);
size_t list_length(List *list);
void *list_push(List *list, void *newItem);
const void *list_peek_front(List *list);
void list_delete_front(List *list);
void list_delete_index(List *list, size_t index);
void *list_at(List *list, size_t index);
void list_pop(List *list, void *buffer);


#endif




