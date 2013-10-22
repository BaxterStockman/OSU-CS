<<<<<<< HEAD
/*  dynamicArray.c: Dynamic Array implementation. */
=======
/*	dynamicArray.c: Dynamic Array implementation. */
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
#include <assert.h>
#include <stdlib.h>
#include "dynamicArray.h"

struct DynArr
{
<<<<<<< HEAD
    TYPE *data;     /* pointer to the data array */
    int size;       /* Number of elements in the array */
    int capacity;   /* capacity ofthe array */
=======
	TYPE *data;		/* pointer to the data array */
	int size;		/* Number of elements in the array */
	int capacity;	/* capacity ofthe array */
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
};


/* ************************************************************************
<<<<<<< HEAD
    Dynamic Array Functions
=======
	Dynamic Array Functions
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
************************************************************************ */

/* Initialize (including allocation of data array) dynamic array.

<<<<<<< HEAD
    param:  v       pointer to the dynamic array
    param:  cap     capacity of the dynamic array
    pre:    v is not null
    post:   internal data array can hold cap elements
    post:   v->data is not null
*/
void initDynArr(DynArr *v, int capacity)
{
    assert(capacity > 0);
    assert(v!= 0);
    v->data = (TYPE *) malloc(sizeof(TYPE) * capacity);
    assert(v->data != 0);
    v->size = 0;
    v->capacity = capacity;
=======
	param: 	v		pointer to the dynamic array
	param:	cap 	capacity of the dynamic array
	pre:	v is not null
	post:	internal data array can hold cap elements
	post:	v->data is not null
*/
void initDynArr(DynArr *v, int capacity)
{
	assert(capacity > 0);
	assert(v!= 0);
	v->data = (TYPE *) malloc(sizeof(TYPE) * capacity);
	assert(v->data != 0);
	v->size = 0;
	v->capacity = capacity;	
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
}

/* Allocate and initialize dynamic array.

<<<<<<< HEAD
    param:  cap     desired capacity for the dyn array
    pre:    none
    post:   none
    ret:    a non-null pointer to a dynArr of cap capacity
            and 0 elements in it.
*/
DynArr* createDynArr(int cap)
{
    assert(cap > 0);
    DynArr *r = (DynArr *)malloc(sizeof( DynArr));
    assert(r != 0);
    initDynArr(r,cap);
    return r;
}

/* Deallocate data array in dynamic array.

    param:  v       pointer to the dynamic array
    pre:    none
    post:   d.data points to null
    post:   size and capacity are 0
    post:   the memory used by v->data is freed
*/
void freeDynArr(DynArr *v)
{
    if(v->data != 0)
    {
        free(v->data);  /* free the space on the heap */
        v->data = 0;    /* make it point to null */
    }
    v->size = 0;
    v->capacity = 0;
}

/* Deallocate data array and the dynamic array ure.

    param:  v       pointer to the dynamic array
    pre:    none
    post:   the memory used by v->data is freed
    post:   the memory used by d is freed
*/
void deleteDynArr(DynArr *v)
{
    freeDynArr(v);
    free(v);
}

/* Resizes the underlying array to be the size cap

    param:  v       pointer to the dynamic array
    param:  cap     the new desired capacity
    pre:    v is not null
    post:   v has capacity newCap
*/
void _dynArrSetCapacity(DynArr *v, int newCap)
{
    assert(v);
    v->capacity = newCap;
    v->data = realloc(v->data, sizeof(TYPE) * v->capacity);
=======
	param:	cap 	desired capacity for the dyn array
	pre:	none
	post:	none
	ret:	a non-null pointer to a dynArr of cap capacity
			and 0 elements in it.		
*/
DynArr* createDynArr(int cap)
{
	assert(cap > 0);
	DynArr *r = (DynArr *)malloc(sizeof( DynArr));
	assert(r != 0);
	initDynArr(r,cap);
	return r;
}

/* Deallocate data array in dynamic array. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	d.data points to null
	post:	size and capacity are 0
	post:	the memory used by v->data is freed
*/
void freeDynArr(DynArr *v)
{
	if(v->data != 0)
	{
		free(v->data); 	/* free the space on the heap */
		v->data = 0;   	/* make it point to null */
	}
	v->size = 0;
	v->capacity = 0;
}

/* Deallocate data array and the dynamic array ure. 

	param: 	v		pointer to the dynamic array
	pre:	none
	post:	the memory used by v->data is freed
	post:	the memory used by d is freed
*/
void deleteDynArr(DynArr *v)
{
	freeDynArr(v);
	free(v);
}

/* Resizes the underlying array to be the size cap 

	param: 	v		pointer to the dynamic array
	param:	cap		the new desired capacity
	pre:	v is not null
	post:	v has capacity newCap
*/
void _dynArrSetCapacity(DynArr *v, int newCap)
{	
	/* FIXME: You will write this function */
	
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
}

/* Get the size of the dynamic array

<<<<<<< HEAD
    param:  v       pointer to the dynamic array
    pre:    v is not null
    post:   none
    ret:    the size of the dynamic array
*/
int sizeDynArr(DynArr *v)
{
    assert(v);
    return v->size;
}

/*  Adds an element to the end of the dynamic array

    param:  v       pointer to the dynamic array
    param:  val     the value to add to the end of the dynamic array
    pre:    the dynArry is not null
    post:   size increases by 1
    post:   if reached capacity, capacity is doubled
    post:   val is in the last utilized position in the array
*/
void addDynArr(DynArr *v, TYPE val)
{
    assert(v);
    pushDynArr(v, val);
}

/*  Get an element from the dynamic array from a specified position

    param:  v       pointer to the dynamic array
    param:  pos     integer index to get the element from
    pre:    v is not null
    pre:    v is not empty
    pre:    pos < size of the dyn array and >= 0
    post:   no changes to the dyn Array
    ret:    value stored at index pos
=======
	param: 	v		pointer to the dynamic array
	pre:	v is not null
	post:	none
	ret:	the size of the dynamic array
*/
int sizeDynArr(DynArr *v)
{
	return v->size;
}

/* 	Adds an element to the end of the dynamic array

	param: 	v		pointer to the dynamic array
	param:	val		the value to add to the end of the dynamic array
	pre:	the dynArry is not null
	post:	size increases by 1
	post:	if reached capacity, capacity is doubled
	post:	val is in the last utilized position in the array
*/
void addDynArr(DynArr *v, TYPE val)
{
	/* FIXME: You will write this function */

}

/*	Get an element from the dynamic array from a specified position
	
	param: 	v		pointer to the dynamic array
	param:	pos		integer index to get the element from
	pre:	v is not null
	pre:	v is not empty
	pre:	pos < size of the dyn array and >= 0
	post:	no changes to the dyn Array
	ret:	value stored at index pos
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
*/

TYPE getDynArr(DynArr *v, int pos)
{
<<<<<<< HEAD
    assert(v);
    assert(sizeDynArr(v));
    assert(pos < v->size);
    assert(pos >= 0);
    return v->data[pos];
}

/*  Put an item into the dynamic array at the specified location,
    overwriting the element that was there

    param:  v       pointer to the dynamic array
    param:  pos     the index to put the value into
    param:  val     the value to insert
    pre:    v is not null
    pre:    v is not empty
    pre:    pos >= 0 and pos < size of the array
    post:   index pos contains new value, val
*/
void putDynArr(DynArr *v, int pos, TYPE val)
{
    assert(v);
    assert(sizeDynArr(v));
    assert(pos >= 0);
    assert(pos < v->size);
    v->data[pos] = val;
}

/*  Swap two specified elements in the dynamic array

    param:  v       pointer to the dynamic array
    param:  i,j     the elements to be swapped
    pre:    v is not null
    pre:    v is not empty
    pre:    i, j >= 0 and i,j < size of the dynamic array
    post:   index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynArr *v, int i, int  j)
{
    assert(v);
    assert(sizeDynArr(v));
    TYPE temp = getDynArr(v, i);
    putDynArr(v, i, getDynArr(v, j));
    putDynArr(v, j, temp);
}

/*  Remove the element at the specified location from the array,
    shifts other elements back one to fill the gap

    param:  v       pointer to the dynamic array
    param:  idx     location of element to remove
    pre:    v is not null
    pre:    v is not empty
    pre:    idx < size and idx >= 0
    post:   the element at idx is removed
    post:   the elements past idx are moved back one
*/
void removeAtDynArr(DynArr *v, int idx)
{
    assert(v);
    assert(sizeDynArr(v));
    assert(idx < v->size);
    assert(idx >= 0);
    for( ; idx < v->size-1; idx++)
        v->data[idx] = v->data[idx+1];
    v->size--;
=======
	/* FIXME: You will write this function */

	/* FIXME: you must change this return value */
	return 1; 
}

/*	Put an item into the dynamic array at the specified location,
	overwriting the element that was there

	param: 	v		pointer to the dynamic array
	param:	pos		the index to put the value into
	param:	val		the value to insert 
	pre:	v is not null
	pre:	v is not empty
	pre:	pos >= 0 and pos < size of the array
	post:	index pos contains new value, val
*/
void putDynArr(DynArr *v, int pos, TYPE val)
{
	/* FIXME: You will write this function */
}

/*	Swap two specified elements in the dynamic array

	param: 	v		pointer to the dynamic array
	param:	i,j		the elements to be swapped
	pre:	v is not null
	pre:	v is not empty
	pre:	i, j >= 0 and i,j < size of the dynamic array
	post:	index i now holds the value at j and index j now holds the value at i
*/
void swapDynArr(DynArr *v, int i, int  j)
{
	/* FIXME: You will write this function */
}

/*	Remove the element at the specified location from the array,
	shifts other elements back one to fill the gap

	param: 	v		pointer to the dynamic array
	param:	idx		location of element to remove
	pre:	v is not null
	pre:	v is not empty
	pre:	idx < size and idx >= 0
	post:	the element at idx is removed
	post:	the elements past idx are moved back one
*/
void removeAtDynArr(DynArr *v, int idx)
{
	/* FIXME: You will write this function */
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
}



/* ************************************************************************
<<<<<<< HEAD
    Stack Interface Functions
************************************************************************ */

/*  Returns boolean (encoded in an int) demonstrating whether or not the
    dynamic array stack has an item on it.

    param:  v       pointer to the dynamic array
    pre:    the dynArr is not null
    post:   none
    ret:    1 if empty, otherwise 0
*/
int isEmptyDynArr(DynArr *v)
{
    assert(v);
    return !sizeDynArr(v);
}

/*  Push an element onto the top of the stack

    param:  v       pointer to the dynamic array
    param:  val     the value to push onto the stack
    pre:    v is not null
    post:   size increases by 1
            if reached capacity, capacity is doubled
            val is on the top of the stack
*/
void pushDynArr(DynArr *v, TYPE val)
{
    assert(v);
    v->size++;
    if(v->size >= v->capacity)
        _dynArrSetCapacity(v, 2 * v->capacity);
    assert(v->size < v->capacity);
    v->data[v->size-1] = val;
}

/*  Returns the element at the top of the stack

    param:  v       pointer to the dynamic array
    pre:    v is not null
    pre:    v is not empty
    post:   no changes to the stack
*/
TYPE topDynArr(DynArr *v)
{
    assert(v);
    assert(sizeDynArr(v));
    return v->data[v->size-1];
}

/* Removes the element on top of the stack

    param:  v       pointer to the dynamic array
    pre:    v is not null
    pre:    v is not empty
    post:   size is decremented by 1
            the top has been removed
*/
void popDynArr(DynArr *v)
{
    assert(v);
    assert(sizeDynArr(v));
    v->size--;
}

/* ************************************************************************
    Bag Interface Functions
************************************************************************ */

/*  Returns boolean (encoded as an int) demonstrating whether or not
    the specified value is in the collection
    true = 1
    false = 0

    param:  v       pointer to the dynamic array
    param:  val     the value to look for in the bag
    pre:    v is not null
    pre:    v is not empty
    post:   no changes to the bag
*/
int containsDynArr(DynArr *v, TYPE val)
{
    assert(v);
    assert(sizeDynArr(v));
    int i;
    for(i = 0; i < v->size; i++)
        if(v->data[i] == val)
            return 1;
    return 0;
}

/*  Removes the first occurrence of the specified value from the collection
    if it occurs

    param:  v       pointer to the dynamic array
    param:  val     the value to remove from the array
    pre:    v is not null
    pre:    v is not empty
    post:   val has been removed
    post:   size of the bag is reduced by 1
*/
void removeDynArr(DynArr *v, TYPE val)
{
    assert(v);
    assert(sizeDynArr(v));
    TYPE *temp_data = v->data;
    for( ; *(temp_data); temp_data++)
        if(*(temp_data) == val)
            removeAtDynArr(v, temp_data - v->data);
=======
	Stack Interface Functions
************************************************************************ */

/*	Returns boolean (encoded in an int) demonstrating whether or not the 
	dynamic array stack has an item on it.

	param:	v		pointer to the dynamic array
	pre:	the dynArr is not null
	post:	none
	ret:	1 if empty, otherwise 0
*/
int isEmptyDynArr(DynArr *v)
{
	/* FIXME: You will write this function */
	
	/* FIXME:  You will change this return value*/
	return 1;
}

/* 	Push an element onto the top of the stack

	param:	v		pointer to the dynamic array
	param:	val		the value to push onto the stack
	pre:	v is not null
	post:	size increases by 1
			if reached capacity, capacity is doubled
			val is on the top of the stack
*/
void pushDynArr(DynArr *v, TYPE val)
{
	/* FIXME: You will write this function */
}

/*	Returns the element at the top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the stack
*/
TYPE topDynArr(DynArr *v)
{
	/* FIXME: You will write this function */
	
	/* FIXME: You will change this return value*/
	return 1;
}

/* Removes the element on top of the stack 

	param:	v		pointer to the dynamic array
	pre:	v is not null
	pre:	v is not empty
	post:	size is decremented by 1
			the top has been removed
*/
void popDynArr(DynArr *v)
{
	/* FIXME: You will write this function */
}

/* ************************************************************************
	Bag Interface Functions
************************************************************************ */

/*	Returns boolean (encoded as an int) demonstrating whether or not
	the specified value is in the collection
	true = 1
	false = 0

	param:	v		pointer to the dynamic array
	param:	val		the value to look for in the bag
	pre:	v is not null
	pre:	v is not empty
	post:	no changes to the bag
*/
int containsDynArr(DynArr *v, TYPE val)
{
	/* FIXME: You will write this function */
	
	/* FIXME:  You will change this return value */
	return 1;

}

/*	Removes the first occurrence of the specified value from the collection
	if it occurs

	param:	v		pointer to the dynamic array
	param:	val		the value to remove from the array
	pre:	v is not null
	pre:	v is not empty
	post:	val has been removed
	post:	size of the bag is reduced by 1
*/
void removeDynArr(DynArr *v, TYPE val)
{
	/* FIXME: You will write this function */
>>>>>>> 04b60e094953c764612fa2653427dee4da9039eb
}
