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
    int size;/* number of links in the deque */
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
    q->Sentinel->next = q->Sentinel;
    q->Sentinel->prev = q->Sentinel;
    q->size = 0;
}

/*
 create a new circular list deque

 */

cl_deque *createCirListDeque() {
    cl_deque *newCL = malloc(sizeof(cl_deque));
    _initCirListDeque(newCL);
    return(newCL);
}


/* Create a link for a value.

    param:  val     the value to create a link for
    pre:    none
    post:   a link to store the value
*/
DLink * _createLink (TYPE val) {
    return malloc(sizeof(DLink));
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
    newLink->value = v;
    newLink->prev = lnk;
    newLink->next = lnk->next;
    lnk->next->prev = newLink;
    lnk->next = newLink;
    q->size++;
}

/* Adds a link to the back of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the back of the deque
*/
void addBackCirListDeque (cl_deque *q, TYPE val) {
    _addLinkAfter(q, q->Sentinel->prev, val);
}

/* Adds a link to the front of the deque

    param:  q       pointer to the deque
    param:  val     value for the link to be added
    pre:    q is not null
    post:   a link storing val is added to the front of the deque
*/
void addFrontCirListDeque(cl_deque *q, TYPE val) {
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
    lnk->next->prev = lnk->prev;
    lnk->prev->next = lnk->next;
    free(lnk);
    q->size--;
}

/* Remove the front of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the front is removed from the deque
*/
void removeFrontCirListDeque (cl_deque *q) {
    _removeLink(q, q->Sentinel->next);
}


/* Remove the back of the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the back is removed from the deque
*/
void removeBackCirListDeque(cl_deque *q) {
    _removeLink(q, q->Sentinel->prev);
}

/* De-allocate all links of the deque

    param:  q       pointer to the deque
    pre:    none
    post:   All links (including Sentinel) are de-allocated
*/
void freeCirListDeque(cl_deque *q) {
    DLink *here = q->Sentinel;
    while((here = here->next)) {
        free(here->prev);
        if(here == q->Sentinel)
            break;
    }
    free(here);
    q->size = 0;
}

/* Check whether the deque is empty

    param:  q       pointer to the deque
    pre:    q is not null
    ret:    1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(cl_deque *q) {
    return !q->size;
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
}

/* Reverse the deque

    param:  q       pointer to the deque
    pre:    q is not null and q is not empty
    post:   the deque is reversed
*/
void reverseCirListDeque(cl_deque *q) {
    DLink *here = q->Sentinel;
    DLink *last;
    while(((last = here) && (here = here->next))) {
        last->next = last->prev;
        last->prev = here;
        if(here == q->Sentinel) break;
    }
}
