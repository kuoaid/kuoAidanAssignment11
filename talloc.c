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
    
    Value* new_pointer = malloc(sizeof(Value));
    new_pointer->type = PTR_TYPE;
    new_pointer->p = malloc(size);

    if (activeList == NULL) {
        activeList = malloc(sizeof(Value));
        activeList->type = CONS_TYPE;
        activeList->c.car = new_pointer;
        activeList->c.cdr = NULL;
    }else{
        Value* new_cons = malloc(sizeof(Value));
        new_cons->type = CONS_TYPE;
        new_cons->c.car = new_pointer;
        new_cons->c.cdr = activeList;
        activeList = new_cons;
    }
    return new_pointer->p;
}


void tfreeHelper(Value *list) {
    if(list==NULL){
        free(list);
    }else if(list->c.cdr != NULL) {
        tfreeHelper(list->c.cdr);
        }
        
    free(list->c.car->p);
    free(list->c.car);
    free(list);
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated for purposes of maintaining the active list. Hint: consider 
// that talloc may be called again after tfree is called...
void tfree() {
    tfreeHelper(activeList);
    activeList=NULL;
    
}

// Replacement for the C function 'exit' that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on, since you'll be able
// to call it to clean up memory and exit your program whenever an error occurs.
// Briefly look up exit to get a sense of what the 'status' parameter does.
void texit(int status) {
    tfree();
    exit(status);
}