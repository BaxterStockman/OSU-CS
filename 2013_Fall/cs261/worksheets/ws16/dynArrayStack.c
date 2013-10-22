#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<errno.h>
#include "dynArrayStack.h"

#ifndef TYPE
#define TYPE int
#endif

struct dynArr {
    TYPE *data;
    int size;
    int cap;
};

void die(const char *message) {
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

dynArr_p initDynArray(int initCap) {
    dynArr_p da = malloc(sizeof(struct dynArr));
    if(!da) die("Memory error");
    da->data = malloc(sizeof(TYPE) * (initCap));
    if(!da->data) die("Memory error");
    if(!da->size) da->size = 0;
    da->cap = initCap;
    return da;
}

void freeDynArray(dynArr_p da) {
    if(da) {
        free(da->data);
        free(da);
    }
}

void __setCapDynArr(dynArr_p da) {
    da->cap *= 2;
    da->data = realloc(da->data, sizeof(TYPE) * da->cap);
}

int sizeDynArray(dynArr_p da) {
    return da->size;
}

void addDynArray(dynArr_p da, TYPE elem) {
    putDynArray(da, da->size, elem);
}

void rmLocDynArray(dynArr_p da, int pos) {
    int rm;
    for(rm = --(da->size); rm > pos && rm > 0; rm--) {
        da->data[rm-1] = da->data[rm];
    }
}

TYPE getLocDynArray (dynArr_p da, int);

void putDynArray(dynArr_p da, int pos, TYPE elem) {
    int insert;
    if(da->size >= da->cap)
        __setCapDynArr(da);
    assert(da->size < da->cap);
    da->size++;
    for(insert = pos+1; insert < da->size; insert++) {
        da->data[insert] = da->data[insert-1];
    }
    da->data[pos] = elem;
}

void pushDynArray(dynArr_p da, TYPE elem) {
    putDynArray(da, da->size, elem);
}

TYPE topDynArray(dynArr_p da) {
    return da->data[da->size-1];
}

void popDynArray(dynArr_p da) {
    rmLocDynArray(da, da->size-1);
}

int isEmptyDynArray(dynArr_p da) {
    return(!da->size);
}
