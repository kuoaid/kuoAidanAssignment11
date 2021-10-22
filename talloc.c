#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "./talloc.h"
#include <string.h>

// Global variable holding the head of the active list (i.e., the linked
// list containing pointers to all of the memory that has been allocated
// via calls to talloc).
Value *activeList = NULL;

void *talloc(size_t size) {

    Value* new_value = malloc(size);
    new_value->type = NULL_TYPE;

    //if no list yet, make list
    if (activeList == NULL) {
        activeList->c.cdr=malloc(sizeof(Value));
    }

    Value* curr_pointer = activeList;
    while(curr_pointer->c.cdr != NULL){
        curr_pointer = curr_pointer->c.cdr;
    }
    curr_pointer->c.cdr = new_value;
    return new_value;
}