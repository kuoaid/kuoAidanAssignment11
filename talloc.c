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
    new_value->c.cdr = NULL;
    new_value->type = NULL_TYPE;

    //if no list yet, make list
    if (activeList == NULL) {
        activeList = new_value;
        //activeList->c.cdr=malloc(sizeof(Value));
    } else {

        Value* curr_pointer = activeList;
        while(curr_pointer->c.cdr != NULL){
            curr_pointer = curr_pointer->c.cdr;
        }
        curr_pointer->c.cdr = new_value;
    }
    return new_value;
}


void tfreeHelper(Value *list) {
    if (list->c.cdr != NULL) {
        tfreeHelper(list->c.cdr);
    }
    free(list);
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated for purposes of maintaining the active list. Hint: consider 
// that talloc may be called again after tfree is called...
void tfree() {
    tfreeHelper(activeList);
    free(activeList);
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