#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "./talloc.h"
#include <string.h>

// Global variable holding the head of the active list (i.e., the linked
// list containing pointers to all of the memory that has been allocated
// via calls to talloc).
Value* activeList = NULL;

void *talloc(size_t size) {
    
    void* new_anything = malloc(size);

    if (activeList == NULL) {
        activeList = malloc(sizeof(Value));
        activeList->type = CONS_TYPE;
        activeList->c.car = new_anything;
        activeList->c.cdr = NULL;
    }else{
        Value* curr_pointer = activeList;//is this a direct copy?
        while(curr_pointer->c.cdr != NULL){
        curr_pointer = curr_pointer->c.cdr;
        }
        curr_pointer = malloc(sizeof(Value));
        curr_pointer->type = CONS_TYPE;
        curr_pointer->c.car = new_anything;
        curr_pointer->c.cdr = NULL;
    }

    return new_anything;
}


void tfreeHelper(Value *list) {
    if (list->c.cdr != NULL) {
        tfreeHelper(list->c.cdr);
    }
    free(list->c.car);
    free(list);//free list->c.car
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated for purposes of maintaining the active list. Hint: consider 
// that talloc may be called again after tfree is called...
void tfree() {
    tfreeHelper(activeList);
    //free(activeList);
    activeList = NULL;
}

// Replacement for the C function 'exit' that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on, since you'll be able
// to call it to clean up memory and exit your program whenever an error occurs.
// Briefly look up exit to get a sense of what the 'status' parameter does.
void texit(int status) {
    tfree();
    exit(status);
}