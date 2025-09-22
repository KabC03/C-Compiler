#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct ListNode {
    struct ListNode *next;
    uint8_t data[];
};
typedef struct List {
    struct ListNode *head;
    size_t elementSize;
    size_t length;
} List;

#endif




