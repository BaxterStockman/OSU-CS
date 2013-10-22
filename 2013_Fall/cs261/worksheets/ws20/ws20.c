//-----------------------------------------------------------------------------
//  dynArrayDeque.h
//-----------------------------------------------------------------------------

#ifndef dynArrayDeque
#define dynArrayDeque

#define TYPE int

typedef struct deque *deque_p;

deque_p dequeInit(int initCap);
void _dequeSetCap(deque_p d, int newCap);
void _dequeDoubleCap(deque_p d);
void dequeFree(deque_p d);
int dequeSize(deque_p d);
void dequeAddFront(deque_p d, TYPE val);
void dequeAddBack(deque_p d, TYPE val);
void dequeRmFront(deque_p d);
void dequeRmBack(deque_p d);
TYPE* dequeFront(deque_p d);
TYPE* dequeBack(deque_p d);
TYPE* dequeArrayStart(deque_p d);
void printDeque(deque_p d);
void die(const char *message);

#endif


//-----------------------------------------------------------------------------
//  dynArrayDeque.c
//-----------------------------------------------------------------------------

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


//-----------------------------------------------------------------------------
//  test.c
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include "dynArrayDeque.h"

#define INIT_CAP 10

void pauseOut(const char *msg) {
    printf("%s\n", msg);
    //printf("Press any key to continue.\n");
    //while(!getchar());
    //system("clear");
}

int main() {
    int i;
    deque_p test = dequeInit(INIT_CAP);

    //system("clear");

    // Demonstrate the operation of the
    // dequeAddBack function
    pauseOut("I will now add 1 through 10 to the deque using dequeAddBack.");
    for(i = 1; i <= 10; i++)
        dequeAddBack(test, i);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeRmFront function
    pauseOut("I will now remove the 3 elements at the front of the deque using dequeRmFront.");
    for(i = 0; i < 3; i++)
        dequeRmFront(test);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeRmBack function
    pauseOut("I will now remove the 4 elements at the back of the deque using dequeRmBack.");
    for(i = 0; i < 4; i++)
        dequeRmBack(test);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeAddFront function
    pauseOut("I will now add 3 through -3 to the front to demonstrate wraparound.");
    for(i = 3; i > -4; i--)
        dequeAddFront(test, i);
    printDeque(test);
    putchar('\n');
    printf("Notice that the memory address of the first element is higher than the last.\n");
    putchar('\n');

    // Demonstrate expansion of the array
    pauseOut("I will now add 7 through 15 to the back to demonstrate expansion.");
    for(i = 7; i < 16; i++)
        dequeAddBack(test, i);
    printDeque(test);
    putchar('\n');
    printf("Notice that the memory addresses are now in order.");
    putchar('\n');

    return 0;
}


//-----------------------------------------------------------------------------
//  Output of test.c
//-----------------------------------------------------------------------------

I will now add 1 through 10 to the deque using dequeAddBack.
Deque data begins at address 0xbd7030.
Deque data begins at index 0.
Deque size: 10.
Deque capacity: 10.
Deque does not wrap around.
 0 (0xbd7030): 1
 1 (0xbd7034): 2
 2 (0xbd7038): 3
 3 (0xbd703c): 4
 4 (0xbd7040): 5
 5 (0xbd7044): 6
 6 (0xbd7048): 7
 7 (0xbd704c): 8
 8 (0xbd7050): 9
 9 (0xbd7054): 10

I will now remove the 3 elements at the front of the deque using dequeRmFront.
Deque data begins at address 0xbd7030.
Deque data begins at index 3.
Deque size: 7.
Deque capacity: 10.
Deque does not wrap around.
 * (0xbd7030): <empty>
 * (0xbd7034): <empty>
 * (0xbd7038): <empty>
 0 (0xbd703c): 4
 1 (0xbd7040): 5
 2 (0xbd7044): 6
 3 (0xbd7048): 7
 4 (0xbd704c): 8
 5 (0xbd7050): 9
 6 (0xbd7054): 10

I will now remove the 4 elements at the back of the deque using dequeRmBack.
Deque data begins at address 0xbd7030.
Deque data begins at index 3.
Deque size: 3.
Deque capacity: 10.
Deque does not wrap around.
 * (0xbd7030): <empty>
 * (0xbd7034): <empty>
 * (0xbd7038): <empty>
 0 (0xbd703c): 4
 1 (0xbd7040): 5
 2 (0xbd7044): 6
 * (0xbd7048): <empty>
 * (0xbd704c): <empty>
 * (0xbd7050): <empty>
 * (0xbd7054): <empty>

I will now add 3 through -3 to the front to demonstrate wraparound.
Deque data begins at address 0xbd7030.
Deque data begins at index 6.
Deque size: 10.
Deque capacity: 10.
Deque wraps around.
 0 (0xbd7048): -3
 1 (0xbd704c): -2
 2 (0xbd7050): -1
 3 (0xbd7054): 0
 4 (0xbd7030): 1
 5 (0xbd7034): 2
 6 (0xbd7038): 3
 7 (0xbd703c): 4
 8 (0xbd7040): 5
 9 (0xbd7044): 6

Notice that the memory address of the first element is higher than the last.

I will now add 7 through 15 to the back to demonstrate expansion.
Deque data begins at address 0xbd7080.
Deque data begins at index 0.
Deque size: 19.
Deque capacity: 20.
Deque does not wrap around.
 0 (0xbd7080): -3
 1 (0xbd7084): -2
 2 (0xbd7088): -1
 3 (0xbd708c): 0
 4 (0xbd7090): 1
 5 (0xbd7094): 2
 6 (0xbd7098): 3
 7 (0xbd709c): 4
 8 (0xbd70a0): 5
 9 (0xbd70a4): 6
10 (0xbd70a8): 7
11 (0xbd70ac): 8
12 (0xbd70b0): 9
13 (0xbd70b4): 10
14 (0xbd70b8): 11
15 (0xbd70bc): 12
16 (0xbd70c0): 13
17 (0xbd70c4): 14
18 (0xbd70c8): 15
 * (0xbd70cc): <empty>

Notice that the memory addresses are now in order.
