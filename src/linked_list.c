#include "linked_list.h"



//Initialise a list
void list_init(List *list, size_t elementSize) {
    list->head = NULL;
    list->elementSize = elementSize;
    list->length = 0;
    return;
}


//Destroy associated memory
void list_destroy(List *list) {

    struct ListNode *current = list->head;;
    struct ListNode *next = current;
    for(size_t i = 0; i < list->length; i++) {
        next = next->next;
        free(current);

        current = next;
    } 

    return;
}



//Get capacity
size_t list_length(List *list) {
    return list->length;
}





//Insert item to a list - return reference to item in list
void *list_push(List *list, void *newItem) {
    
    struct ListNode newNode = malloc(sizeof(struct ListNode) + list->elementSize);
    if(newNode == NULL) return NULL;

    memcpy(newNode->data, newItem, list->elementSize);
    newNode->next = NULL;
    newNode->next = list->head;
    list->head = newNode;

    return newNode; 
}


//Pop from a list to a buffer
void list_pop(List *list, void *buffer) {
    memcpy(buffer, list->head->data, list->elementSize);
    list_delete_front(list);
}


//Peak front of a list
const void *list_peak_front(List *list) {
    return list->head->data;
}



//Delete front element
void list_delete_front(List *list) {
   
    struct ListNode *current = list->head; 
    list->head = list->head->next;
    free(current);
    return;
}


//Delete element at an index
void list_delete_index(List *list, size_t index) {

    struct ListNode *previous = list->head;
    struct ListNode *current = previous;
    struct ListNode *next = current;

    for(size_t i = 0; i < index; i++) {
        previous = current;
        current = next; 
        next = next->next;
    }
    previous->next = next;
    free(current);

    return;
}


//Get element
void *list_at(List *list, size_t index) {
   
    struct ListNode *current = list->head;
    for(size_t i = 0; i < index; i++) {
        current = current->next;         
    }
    return current->data; 
}




