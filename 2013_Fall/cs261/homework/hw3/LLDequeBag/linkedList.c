#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef TYPE
#define TYPE double
#endif

/* Double Link*/
typedef struct DLink {
    TYPE value;
    struct DLink * next;
    struct DLink * prev;
} DLink;

/* Double Linked List with Head and Tail Sentinels  */

typedef struct linkedList{
    int size;
    struct DLink *firstLink;
    struct DLink *lastLink;
} linkedList;

/*
    initList
    param lst the linkedList
    pre: lst is not null
    post: lst size is 0
*/

void _initList (struct linkedList *lst) {
    // allocate memory for first and last links
    // and check that malloc has succeeded
    lst->firstLink = malloc(sizeof(DLink));
    assert(lst->firstLink);
    //lst->firstLink = NULL;
    lst->lastLink = malloc(sizeof(DLink));
    assert(lst->lastLink);
    //lst->lastLink = NULL;

    // Initialize values
    lst->firstLink->value = 0;
    lst->lastLink->value = 0;

    // set DLinks to point at one another
    lst->firstLink->next = lst->lastLink;
    lst->firstLink->prev = NULL;
    lst->lastLink->prev = lst->firstLink;
    lst->lastLink->next = NULL;

    // set size of list to 0
    lst->size = 0;
}

/*
 createList
 param: none
 pre: none
 post: firstLink and lastLink reference sentinels
 */

linkedList * createLinkedList() {
    linkedList *newList = malloc(sizeof(linkedList));
    assert(newList);
    _initList(newList);
    return(newList);
}

/*
    _addLinkBeforeBefore
    param: lst the linkedList
    param: l the  link to add before
    param: v the value to add
    pre: lst is not null
    pre: l is not null
    post: lst is not empty
*/

/* Adds Before the provided link, l */

void _addLinkBefore(struct linkedList *lst, struct DLink *l, TYPE v) {
    assert(lst);
    assert(l);

    // create new link
    DLink *newLink = malloc(sizeof(DLink));
    assert(newLink);

    // set values of newLink's data members
    newLink->value = v;
    newLink->next = l;
    newLink->prev = l->prev;

    // alter surrounding links to point
    // to newLink
    l->prev->next = newLink;
    l->prev = newLink;

    lst->size++;
}


/*
    addFrontList
    param: lst the linkedList
    param: e the element to be added
    pre: lst is not null
    post: lst is not empty, increased size by 1
*/

void addFrontList(struct linkedList *lst, TYPE e) {
    assert(lst);
    _addLinkBefore(lst, lst->firstLink->next, e);
}

/*
    addBackList
    param: lst the linkedList
    param: e the element to be added
    pre: lst is not null
    post: lst is not empty, increased size by 1
*/

void addBackList(struct linkedList *lst, TYPE e) {
    assert(lst);
    _addLinkBefore(lst, lst->lastLink, e);
}

/*
    frontList
    param: lst the linkedList
    pre: lst is not null
    pre: lst is not empty
    post: none
*/

TYPE frontList (struct linkedList *lst) {
    assert(lst);
    assert(lst->size);
    return lst->firstLink->next->value;
}

/*
    backList
    param: lst the linkedList
    pre: lst is not null
    pre: lst is not empty
    post: lst is not empty
*/

TYPE backList(struct linkedList *lst) {
    assert(lst);
    assert(lst->size);
    return lst->lastLink->prev->value;
}

/*
    _removeLink
    param: lst the linkedList
    param: l the linke to be removed
    pre: lst is not null
    pre: l is not null
    post: lst size is reduced by 1
*/

void _removeLink(struct linkedList *lst, struct DLink *l) {
    assert(lst);
    assert(l);
    l->next->prev = l->prev;
    l->prev->next = l->next;
    free(l);
    lst->size--;

}

/*
    removeFrontList
    param: lst the linkedList
    pre:lst is not null
    pre: lst is not empty
    post: size is reduced by 1
*/

void removeFrontList(struct linkedList *lst) {
    assert(lst);
    assert(!isEmptyList(lst));
    _removeLink(lst, lst->firstLink->next);
}

/*
    removeBackList
    param: lst the linkedList
    pre: lst is not null
    pre:lst is not empty
    post: size reduced by 1
*/

void removeBackList(struct linkedList *lst)
{
    assert(lst);
    assert(!isEmptyList(lst));
    _removeLink(lst, lst->lastLink->prev);
}

/*
    isEmptyList
    param: lst the linkedList
    pre: lst is not null
    post: none
*/

int isEmptyList(struct linkedList *lst) {
    assert(lst);
    return lst->firstLink == lst->lastLink;
}


/* Function to print list
 Pre: lst is not null
 */
void _printList(struct linkedList* lst, const char *delim) {
    assert(lst);
    DLink *start = lst->firstLink;
    while((start = start->next) != lst->lastLink)
        printf("%d%s", start->value, delim);
}

/*
    Add an item to the bag
    param:  lst     pointer to the bag
    param:  v       value to be added
    pre:    lst is not null
    post:   a link storing val is added to the bag
 */
void addList(struct linkedList *lst, TYPE v){
    assert(lst);
    addBackList(lst, v);
}

/*  Returns boolean (encoded as an int) demonstrating whether or not
    the specified value is in the collection
    true = 1
    false = 0

    param:  lst     pointer to the bag
    param:  e       the value to look for in the bag
    pre:    lst is not null
    pre:    lst is not empty
    post:   no changes to the bag
*/
int containsList (struct linkedList *lst, TYPE e) {
    assert(lst);
    assert(lst->size);
    DLink *start = lst->firstLink;
    while((start = start->next) != lst->lastLink)
        if(start->value == e) return 1;
    return 0;
}

/*  Removes the first occurrence of the specified value from the collection
    if it occurs

    param:  lst     pointer to the bag
    param:  e       the value to be removed from the bag
    pre:    lst is not null
    pre:    lst is not empty
    post:   e has been removed
    post:   size of the bag is reduced by 1
*/
void removeList (struct linkedList *lst, TYPE e) {
    assert(lst);
    assert(lst->size);
    DLink *start = lst->firstLink;
    while((start = start->next) != lst->lastLink)
        if(start->value == e) {
            start->next->prev = start->prev;
            start->prev->next = start->next;
            free(start);
        }
}
