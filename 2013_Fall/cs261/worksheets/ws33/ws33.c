#include <stdio.h>
#include <assert.h>
#include "dynamicArray.h"

#ifndef TYPE
#define TYPE double
#endif

struct dynArray;

void swap (struct dynArray * v, int i, int j) { /* swap elements i j */
    TYPE temp = dynArrayGet(v, i);
    dynArrayPut(v, i, dynArrayGet(v, j));
    dynArrayPut(v, j, temp);
}

int indexSmallest (struct dynArray * v, int i, int j) {
/* return index of smallest element */
    if (LT(dynArrayGet(v, i), dynArrayGet(v, j)))
       return i;
    return j;
}

TYPE heapGetFirst (struct dynArray *heap) {
    assert(dynArraySize(heap) > 0);
    return dynArrayGet(heap, 0);
}

void _adjustHeap(struct dynArray * heap, int max, int pos) {a
    int leftChild = 2 * pos + 1;    // left child is located at 2x parent node's position, plus 1
    int rightChild = 2 * pos + 2;   // right child is located at 2x parent node's position, plus 2

    // if the value rightChild is less than max, there exists a right child of node pos AND a left
    // child, since the left child is always 'filled' before the right
    if(rightChild < max) {
        // we determine whether the left or right node is smallest in order to maintain
        // heap order when swapping a child and parent node
        int smallestChild = indexSmallest(heap, leftChild, rightChild);
        // if the child is smaller, swap with the parent and call _adjustHeap recursively on child
        if(compare(heap[pos], heap[smallestChild])) {
            swap(heap, pos, smallestChild);
            _adjustHeap(heap, max, smallestChild);
        }
    }
    // if the value leftChild is less than max, there exists a left child of node pos
    else if(leftChild < max)
        // if the child is smaller, swap with the parent and call _adjustHeap recursively on child
        if(compare(heap[pos], heap[leftChild])) {
            swap(heap, pos, leftChild);
            _adjustHeap(heap, max, leftChild);
        }
}

void HeapRemoveFirst(struct dynArray *heap) {
    int last = dynArraySize(heap) - 1;
    assert (last != 0); /* make sure we have at least one element */
    /* Copy the last element to the first position */
    dynArrayPut(heap, 0, dynArrayGet(heap, last));
    dynArrayRemoveAt(heap, last); /* Remove last element.*/
    _adjustHeap(heap, last, 0);/* Rebuild heap */
}

void heapAdd(struct dynArray * heap, TYPE newValue) {
    dynArrayAdd(heap, newValue);
    int last = dynArraySize(heap) - 1;
    adjustHeap(heap, last, 0)
}
