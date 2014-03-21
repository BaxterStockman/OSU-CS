#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "cirListDeque.h"

/* Double Link Struture */
typedef struct DLink {
    TYPE value;/* value of the link */
    struct DLink * next;/* pointer to the next link */
    struct DLink * prev;/* pointer to the previous link */
} DLink;

# define TYPE_SENTINEL_VALUE DBL_MAX


/* ************************************************************************
 Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
 ************************************************************************ */

typedef struct cirListDeque {
    int size; /* number of links in the deque */
    struct DLink *Sentinel; /* pointer to the sentinel */
} cl_deque;

/* internal functions prototypes */
struct DLink* _createLink (TYPE val);
void _addLinkAfter(cl_deque *q, struct DLink *lnk, TYPE v);
void _removeLink(cl_deque *q, struct DLink *lnk);



/* ************************************************************************
    Deque Functions
************************************************************************ */

/* Initialize deque.

    param:  q       pointer to the deque
    pre:    q is not null
    post:   q->Sentinel is allocated and q->size equals zero
*/
void _initCirListDeque (cl_deque *q) {
    q->Sentinel = malloc(sizeof(DLink));
    assert(q->Sentinel);
    q->Sentinel->next = q->Sentinel;
    q->Sentinel->prev = q->Sentinel;
    q->size = 0;
}

/*
 create a new circular list deque

 */

cl_deque *createCirListDeque() {
    cl_deque *newCL = malloc(sizeof(cl_deque));
    assert(newCL);
    newCL->Sentinel = NULL;
    _initCirListDeque(newCL);
    return(newCL);
}


/* Create a link for a value.

    param:  val     the value to create a link for
    pre:    none
    post:   a link to store the value
*/
DLink * _createLink (TYPE val) {
    DLink *newLink =  malloc(sizeof(DLink));
    newLink->prev = NULL;
    newLink->next = NULL;
    newLink->value = val;
    return newLink;
}

/* Adds a link after another link

    param:  q       pointer to the deque
    param:  lnk     pointer to the existing link in the deque
    param:  v       value of the new link to be added after the existing link
    pre:    q is not null
    pre:    lnk is not null
    pre:    lnk is in the deque
    post:   the new link is added into the deque after the existing link
*/
void _addLinkAfter(cl_deque *q, struct DLink *lnk, TYPE v) {
    DLink *newLink = _createLink(v);
    newLink->prev = lnk;        // set 'prev' to point to previous link
    newLink->next = lnk->next;  // next 'next' to point to next link
    lnk->next->prev = newLink;  // set the next link's 'prev' value to point to new link
    lnk->next = newLink;        // set the previous link's 'next' value to point to new link
    q->size++;                  // increment list size
}

/* Adds a link to the back of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the back of the deque
*/
void addBackCirListDeque (cl_deque *q, TYPE val) {
    // adds a link after the current last link
    // in the list, i.e. the link pointed to by
    // the 'prev' value of the sentinel
    _addLinkAfter(q, q->Sentinel->prev, val);
}

/* Adds a link to the front of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the front of the deque
*/
void addFrontCirListDeque(cl_deque *q, TYPE val) {
    // adds a link after the sentinel
    _addLinkAfter(q, q->Sentinel, val);
}

/* Get the value of the front of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   none
    ret:    value of the front of the deque
*/
TYPE frontCirListDeque(cl_deque *q) {
    return q->Sentinel->next->value;
}

/* Get the value of the back of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   none
    ret:    value of the back of the deque
*/
TYPE backCirListDeque(cl_deque *q) {
    return q->Sentinel->prev->value;
}

/* Remove a link from the deque

    param:  q       pointer to the deque
    param:  lnk     pointer to the link to be removed
    pre:    q is not null and q is not empty
    post:   the link is removed from the deque
*/
void _removeLink(cl_deque *q, struct DLink *lnk) {
    lnk->next->prev = lnk->prev;    // set next link's 'prev' value to point to link before lnk
    lnk->prev->next = lnk->next;    // set previous link's 'next' value to point to link after lnk
    free(lnk);      // free memory associated with link
    q->size--;      // decrement size of list
}

/* Remove the front of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the front is removed from the deque
*/
void removeFrontCirListDeque (cl_deque *q) {
    // remove link after sentinel
    _removeLink(q, q->Sentinel->next);
}


/* Remove the back of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the back is removed from the deque
*/
void removeBackCirListDeque(cl_deque *q) {
    // remove link before sentinel
    _removeLink(q, q->Sentinel->prev);
}

/* De-allocate all links of the deque

    param:  q       pointer to the deque
    pre:    none
    post:   All links (including Sentinel) are de-allocated
*/
void freeCirListDeque(cl_deque *q) {
    // point link to the sentinel
    DLink *here = q->Sentinel;
    // iterate through list, each iteration freeing
    // the link BEFORE the current value of 'here'.
    // Break when we get back to the sentinel link.
    while((here = here->next)) {
        free(here->prev);
        if(here == q->Sentinel)
            break;
    }
    free(here);     // free the sentinel link
    q->size = 0;    // set list size to zero
}

/* Check whether the deque is empty

    param:  q       pointer to the deque
    pre:    q is not null
    ret:    1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(cl_deque *q) {
    return !q->size;    // returns 0 if the list has elements, 1 if it does not
}

/* Print the links in the deque from front to back

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the links in the deque are printed from front to back
*/
void printCirListDeque(cl_deque *q) {
    int i;
    DLink *here = q->Sentinel->next;
    for(i = 1; here != q->Sentinel; here = here->next, i++)
        printf("%s %d (%p): %g\n", "Link ", i, here, (double)here->value);
    // only tricky bit: casts here->value to double to work with '%g'
    // format specifier
}

/* Reverse the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the deque is reversed
*/
void reverseCirListDeque(cl_deque *q) {
    DLink *here = q->Sentinel;
    DLink *last;
    // 'here' jumps to next value, 'last' is now
    // the previous value of 'here'
    while(((last = here) && (here = here->next))) {
        // put value of 'prev' into 'next'
        last->next = last->prev;
        // 'here' used to be before 'last',
        // now it's after
        last->prev = here;
        // Already swapped the sentinel on the
        // first iteration, so break here
        if(here == q->Sentinel) break;
    }
}
