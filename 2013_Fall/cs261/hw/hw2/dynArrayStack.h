#ifndef dynArrayStack
# define dynArrayStack

# define TYPE int
# define EQ(a, b) (a == b)

typedef struct dynArr *dynArr_p;

//-----------------------------------------------------------------------------
//  'Internal' methods on dynArr struct.
//-----------------------------------------------------------------------------
dynArr_p createDynArr(int initCap);
void freeDynArr(dynArr_p da);
void __setCapDynArr(dynArr_p da);
int sizeDynArr(dynArr_p da);

//-----------------------------------------------------------------------------
//  Bag interface functions.
//-----------------------------------------------------------------------------
TYPE * containsDynArr(dynArr_p da, TYPE elem);
void addDynArr(dynArr_p da, TYPE elem);
void rmLocDynArr(dynArr_p da, int pos);
void rmValDynArr(dynArr_p da, int elem);
void putDynArr(dynArr_p da, int pos, TYPE elem);

//-----------------------------------------------------------------------------
//  Stack interface functions.
//-----------------------------------------------------------------------------
TYPE topDynArr(dynArr_p da);
void pushDynArr(dynArr_p da, TYPE elem);
void popDynArr(dynArr_p da);
int isEmptyDynArr(dynArr_p da);

//-----------------------------------------------------------------------------
//  Miscellaneous functions.
//-----------------------------------------------------------------------------
void die(const char *message);
void printDynArr(dynArr_p da);

# endif
