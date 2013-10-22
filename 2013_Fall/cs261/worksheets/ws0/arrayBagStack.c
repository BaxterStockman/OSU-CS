#include<stdio.h>
#include<assert.h>
#include "arrayBagStack.h"
#define MAXDATA 100
# define EQ(a, b) (a == b)


//-----------------------------------------------------------------------------
//  Structure used as basis for stack and bag implementation.
//-----------------------------------------------------------------------------
struct arrayBagStack {
    TYPE data [MAXDATA];    // Array to hold data in stack/bag.  Size of array
                            // set by MAXDATA #define
    int count;              // Number of elements in array
};

// Initializes number of elements on stack/in bag to zero.
void initArray(arrayBagStack_t b) {
    b->count = 0;
}

//-----------------------------------------------------------------------------
//  Bag interface functions.
//-----------------------------------------------------------------------------

// Adds value 'v' to stack/bag.
void addArray (arrayBagStack_t b, TYPE v) {
    pushArray(b, v);
}

// Returns 1 if bag contains 'v', returns 0 otherwise
int containsArray (arrayBagStack_t b, TYPE v) {

    // Starting positions of search for 'v'.
    int bot_up, mid_dn, mid_up, top_dn;
    bot_up = 0;
    mid_dn = mid_up = b->count/2;
    top_dn = b->count - 1;

    for( ; ~((bot_up > mid_dn) && (top_dn < mid_up));
            bot_up ++, mid_dn--, mid_up++, top_dn--)
        if(b->data[bot_up] == v || b->data[mid_dn] == v
                || b->data[mid_up] == v ||b->data[top_dn] == v)
            return 1;
    return 0;
}

// Removes 'v' from bag 'b'.
// Precondition: 'b' contains at least one value equal to 'v'.
int removeArray (arrayBagStack_t b, TYPE v) {

    // Check that 'b' contains 'v'
    //if(!containsArray(b, v)) {
    //    printf("Error: attempt to remove nonexistent value.\n");
    //    return 0;
    //}
    assert(containsArray(b, v));

    // Starting positions of search for 'v'.
    int bot_up, mid_dn, mid_up, top_dn, found;
    bot_up = 0;
    mid_dn = mid_up = b->count/2;
    top_dn = b->count - 1;

    for( ; ~((bot_up > mid_dn) && (top_dn < mid_up));
            bot_up ++, mid_dn--, mid_up++, top_dn--) {
        if(b->data[bot_up] == v) {
            found = bot_up;
            break;
        }
        if(b->data[mid_dn] == v) {
            found = mid_dn;
            break;
        }
        if(b->data[mid_up] == v) {
            found = mid_up;
            break;
        }
        if(b->data[top_dn] == v) {
            found = top_dn;
            break;
        }
    }

    // Decrement positions of all values above index 'found'.
    while(found < b->count)
        b->data[found] = b->data[++v];

    // Pop top element of array to account for removal of 'v'.
    popArray(b);

    return 1;
}

// Return number of elements in 'b'.
// Precondition: 'b' has been initialized.
int sizeArray (arrayBagStack_t b) {
    assert(b->count >= 0);
    return b->count;
}

//-----------------------------------------------------------------------------
//  Stack interface functions.
//-----------------------------------------------------------------------------

// Push 'v' onto 'b' and increment 'b->count'.
void pushArray (arrayBagStack_t b, TYPE v) {
    b->data[b->count] = v;
    b->count++;
}

// Return the value on top of stack 'b'.
// Precondition: 'b' contains at least one value.
TYPE topArray (arrayBagStack_t b) {
    assert(!isEmptyArray(b));
    return b->data[b->count - 1];
}

// Pops top value of array 'b'.
// Precondition: 'b' contains at least one value.
void popArray (arrayBagStack_t b) {
    //assert(!isEmptyArray(b));
    b->data[--(b->count)] = 0;
}

// Returns 1 if 'b' contains any values, 0 otherwise.
int isEmptyArray (arrayBagStack_t b) {
    return !(b->count);
}

