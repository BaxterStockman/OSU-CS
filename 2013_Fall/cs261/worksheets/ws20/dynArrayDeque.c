#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include "dynArrayDeque.h"

#ifndef TYPE
#define TYPE int
#endif

struct deque {
    TYPE *data;
    int cap;
    int size;
    int beg;
};

deque_p dequeInit(int initCap) {
    if(initCap <= 0) die("Deque capacity must be positive integer");
    deque_p d = (deque_p)malloc(sizeof(struct deque));
    if(!d) die("Memory error");
    d->size = d->beg = 0;
    d->cap = initCap;
    d->data = (TYPE*)malloc(sizeof(TYPE) * initCap);
    if(!d->data) die("Memory error");
    return d;
}

void _dequeSetCap(deque_p d, int newCap) {
    int i, j;
    deque_p temp = dequeInit(newCap);

    for(i = 0, j = d->beg; i < d->size; i++, j++) {
        temp->data[i] = d->data[j];
        if(j+1 >=  d->cap)
            j = -1;
    }

    free(d->data);
    d->data = temp->data;
    d->cap = newCap;
    d->beg = 0;
}

void _dequeDoubleCap(deque_p d) {
    _dequeSetCap(d, d->cap * 2);
}

void dequeFree(deque_p d) {
    free(d->data);
    d->data = NULL;
    if(d->data) die("Memory error");
    d->size = d->cap = d->beg = 0;
}

int dequeSize(deque_p d) {
    return d->size;
}

void dequeAddFront(deque_p d, TYPE val) {
    if(d->size >= d->cap)
        _dequeDoubleCap(d);

    // Get pointer to front of deque.
    TYPE* front = dequeFront(d);

    // If the pointer to the front is the beginning
    // of the array, set d->beg to d->cap and
    // reset front to point to the next address
    // after the end of the array
    if(front == d->data) {
        d->beg = d->cap;
        front = &(d->data[d->cap]);
    }

    d->beg--;
    *(--front) = val;
    d->size++;
}
void dequeAddBack(deque_p d, TYPE val) {
    if(d->size >= d->cap)
        _dequeDoubleCap(d);

    // Get pointer to back of deque.
    TYPE* back = dequeBack(d);

    // If pointer to the back is the last
    // position in the array, set back to point
    // to the position just before the first
    // element of the array.
    if(back == &(d->data[d->cap-1]))
        back = --(d->data);
    *(++back) = val;
    (d->size)++;
}

void dequeRmFront(deque_p d) {
    if(d->beg == d->cap-1)
        d->beg = 0;
    else
        d->beg++;
    (d->size)--;
}

void dequeRmBack(deque_p d) {
    (d->size)--;
}

TYPE* dequeFront(deque_p d) {
    return &(d->data[d->beg]);
}

TYPE* dequeBack(deque_p d) {
    if(d->beg + d->size >= d->cap)
        return &(d->data[d->beg + d->size - d->cap - 1]);
    else
        return &(d->data[d->beg + d->size - 1]);
}

TYPE* dequeArrayStart(deque_p d) {
    return d->data;
}

void printDeque(deque_p d) {
    int i, j;
    printf("Deque data begins at address %p.\n", (void*)d->data);
    printf("Deque data begins at index %d.\n", d->beg);
    printf("Deque size: %d.\n", d->size);
    printf("Deque capacity: %d.\n", d->cap);
    if((d->beg + d->size) > d->cap) {
        printf("Deque wraps around.\n");
        for(i = d->beg, j = 0; i < d->cap; i++, j++)
            printf("%2d (%p): %d\n", j, &(d->data[i]), d->data[i]);
        for(i = 0; i < (d->beg + d->size - d->cap); i++, j++)
            printf("%2d (%p): %d\n", j, &(d->data[i]), d->data[i]);
        for( ; i < d->beg; i++)
            printf("%2c (%p): <empty>\n", '*', &(d->data[i]));
    }
    else {
        printf("Deque does not wrap around.\n");
        for(i = 0 ; i < d->beg; i++)
            printf("%2c (%p): <empty>\n", '*', &(d->data[i]));
        for(i = d->beg, j = 0; i < d->beg + d->size; i++, j++)
            printf("%2d (%p): %d\n", j, &(d->data[i]), d->data[i]);
        for( ; i < d->cap; i++)
            printf("%2c (%p): <empty>\n", '*', &(d->data[i]));
    }
}

void die(const char *message) {
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

