#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include "dynArrayDeque.h"

#ifndef TYPE
#define TYPE int
#endif

#define INIT_CAP 10
#define RM_NUM 3

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
        if(j >= d->cap)
            j = -1;
    }

    temp->cap = newCap;
    temp->beg = 0;
    temp->size = d->size;

    dequeFree(d);
    d = temp;
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
    if(d->size >= d->cap) {
        putchar('\n');
        printf("Doubling after call to dequeAddFront...\n");
        printf("current contents:\n");
        printDeque(d);
        putchar('\n');
        _dequeDoubleCap(d);
    }

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
    if(d->size >= d->cap) {
        putchar('\n');
        printf("Doubling after call to dequeAddBack...\n");
        printf("current contents:\n");
        printDeque(d);
        putchar('\n');
        _dequeDoubleCap(d);
    }

    // get pointer to front of deque.
    TYPE* back = dequeBack(d);
    // If pointer to the back is the last
    // position in the array, set back to point
    // to the position just before the first
    // element of the array.
    if(back == &(d->data[d->cap-1]))
        back = --(d->data);

    *(++back) = val;
    d->size++;
}

void dequeRmFront(deque_p d) {
    if(d->beg == d->cap-1)
        d->beg = 0;
    else
        d->beg++;
    d->size--;
}

void dequeRmBack(deque_p d) {
    d->size--;
}

TYPE* dequeFront(deque_p d) {
    return &(d->data[d->beg]);
}

TYPE* dequeBack(deque_p d) {
    if(d->beg + d->size > d->cap)
        return &(d->data[d->beg + d->size - d->cap - 1]);
    else
        return &(d->data[d->beg + d->size - 1]);
}

TYPE* dequeArrayStart(deque_p d) {
    return d->data;
}

void printDeque(deque_p d) {
    int i, j;
    printf("deque begins at index %d.\n", d->beg);
    printf("deque size: %d.\n", d->size);
    printf("deque capacity: %d.\n", d->cap);
    if((d->beg + d->size) > d->cap) {
        printf("deque wraps around.\n");
        for(i = d->beg, j = 0; i < d->cap; i++, j++)
            printf("%d (%p): %d\n", j, &(d->data[i]), d->data[i]);
        for(i = 0; i < d->beg; i++, j++)
            printf("%d (%p): %d\n", j, &(d->data[i]), d->data[i]);
    }
    else {
        printf("deque does not wrap around.\n");
        for(i = d->beg, j = 0; i < d->beg + d->size; i++, j++)
            printf("%d (%p): %d\n", j, &(d->data[i]), d->data[i]);
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

int main() {
    int i;
    deque_p test = dequeInit(INIT_CAP);

    putchar('\n');
    printf("Address of array beginning: %p\n", (void*)dequeArrayStart(test));


    for(i = 1; i <= INIT_CAP; i++)
        dequeAddBack(test, i);

    putchar('\n');
    printf("Printing deque contents...\n");
    printDeque(test);

    for(i = 0; i < RM_NUM; i++)
        dequeRmFront(test);

    putchar('\n');
    printf("d->beg: %d; d->size: %d\n", test->beg, test->size);

    putchar('\n');
    printf("(After removal) Printing deque contents...\n");
    printDeque(test);

    dequeAddBack(test, 11);
    putchar('\n');
    printf("Printing deque contents...\n");
    printDeque(test);
}
