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


//-----------------------------------------------------------------------------
//  'Internal' methods on dynArr struct.
//-----------------------------------------------------------------------------

dynArr_p createDynArr(int initCap) {
    dynArr_p da = malloc(sizeof(struct dynArr));
    if(!da) die("Memory error");
    da->data = malloc(sizeof(TYPE) * (initCap));
    if(!da->data) die("Memory error");
    if(!da->size) da->size = 0;
    da->cap = initCap;
    return da;
}

void freeDynArr(dynArr_p da) {
    if(da) {
        free(da->data);
        da->data = NULL;
        if(da->data) die("Failed to free memory associated with dynamic array");
        free(da);
        da = NULL;
        if(da) die("Failed to free memory associated with data structure");
    }
}

void __setCapDynArr(dynArr_p da) {
    da->cap *= 2;
    da->data = realloc(da->data, sizeof(TYPE) * da->cap);
}

int sizeDynArr(dynArr_p da) {
    return da->size;
}


//-----------------------------------------------------------------------------
//  Bag interface functions.
//-----------------------------------------------------------------------------

TYPE * containsDynArr(dynArr_p da, TYPE elem) {
    TYPE *temp_data = da->data;
    for( ; *(temp_data); temp_data++)
        if(*(temp_data) == elem)
            return temp_data;
    return NULL;
}

void addDynArr(dynArr_p da, TYPE elem) {
    putDynArr(da, da->size, elem);
}

void rmLocDynArr(dynArr_p da, int pos) {
    if(pos >= da->size) die("Attempt to access invalid index");
    for( ; pos < da->size-1; pos++)
        da->data[pos] = da->data[pos+1];
    da->size--;
}

void rmValDynArr(dynArr_p da, TYPE elem) {
    rmLocDynArr(da, containsDynArr(da, elem) - da->data);
}


//-----------------------------------------------------------------------------
//  Stack interface functions.
//-----------------------------------------------------------------------------

TYPE topDynArr(dynArr_p da) {
    return da->data[da->size-1];
}

void putDynArr(dynArr_p da, int pos, TYPE elem) {
    da->size++;
    int insert = da->size-1;
    if(da->size >= da->cap)
        __setCapDynArr(da);
    if(da->size >= da->cap) die("Attempt to write to invalid memory location");
    for( ; insert > pos ; insert--)
        da->data[insert] = da->data[insert-1];
    da->data[pos] = elem;
}

void pushDynArr(dynArr_p da, TYPE elem) {
    putDynArr(da, da->size, elem);
}

void popDynArr(dynArr_p da) {
    rmLocDynArr(da, da->size-1);
}

int isEmptyDynArr(dynArr_p da) {
    return(!da->size);
}

void die(const char *message) {
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

void printDynArr(dynArr_p da) {
    int i = da->size-1;
    for( ; i >=0; i--)
        printf("Index %d: %d\n", i, da->data[i]);
}
