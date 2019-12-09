/********************************************************************
 Project 2A
 lab2_add.c
------------
 NAME: Nikola Samardzic
 EMAIL: nikola.s@ucla.edu
 ID: 904799099
********************************************************************/
#include <stddef.h>
#include <sched.h>
#include <stdio.h>
#include "SortedList.h"

int opt_yield;

void 
SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    SortedListElement_t *curr = list->next;
    while (curr->key != NULL && *(curr->key) < *(element->key)) {
	if(opt_yield & INSERT_YIELD) sched_yield();
	curr = curr->next;
    }
    element->next = curr;
    element->prev = curr->prev;
    curr->prev = element;
    (element->prev)->next = element;
}

int
SortedList_delete( SortedListElement_t *element) {

    if (element->prev == NULL || element->prev == NULL
	|| (element->prev)->next == NULL 
	|| (element->next)->prev == NULL) {
	return 1;
    }
    (element->prev)->next = element->next;
    if(opt_yield & DELETE_YIELD) sched_yield();
    (element->next)->prev = element->prev;
    return 0;
}

SortedListElement_t *
SortedList_lookup(SortedList_t *list, const char *key) {
    SortedListElement_t *curr = list->next;
    while(curr->key != key) {
	if (curr->key == NULL) return NULL;
	if(opt_yield & LOOKUP_YIELD) sched_yield();	
	curr = curr->next;
    }
    return curr;
}

int 
SortedList_length(SortedList_t *list) {
    SortedListElement_t *curr = list->next;
    int count;
    for(count = 0; curr->key != NULL; count++) {
	if(opt_yield & LOOKUP_YIELD) sched_yield();
	if (curr->next == NULL || (curr->next)->prev != curr)
	    return -1;
	if (curr->prev == NULL || (curr->prev)->next != curr)
	    return -1;		
	curr = curr->next;
    }
    return count;
}
