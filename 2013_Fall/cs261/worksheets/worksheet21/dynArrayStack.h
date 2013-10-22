#ifndef dynArrayStack
# define dynArrayStack

# define TYPE int
# define EQ(a, b) (a == b)

typedef struct dynArr *dynArr_p;

//-----------------------------------------------------------------------------
//  'Internal' methods on dynArr struct.
//-----------------------------------------------------------------------------
dynArr_p initDynArray(int initCap);
void freeDynArray(dynArr_p da);
void __setCapDynArr(dynArr_p da);
int sizeDynArray(dynArr_p da);

//-----------------------------------------------------------------------------
//  Bag interface functions.
//-----------------------------------------------------------------------------
TYPE * containsDynArray(dynArr_p da, TYPE elem);
void addDynArray(dynArr_p da, TYPE elem);
void rmLocDynArray(dynArr_p da, int pos);
void rmValDynArray(dynArr_p da, int elem);
void putDynArray(dynArr_p da, int pos, TYPE elem);

//-----------------------------------------------------------------------------
//  Stack interface functions.
//-----------------------------------------------------------------------------
TYPE topDynArray(dynArr_p da);
void pushDynArray(dynArr_p da, TYPE elem);
void popDynArray(dynArr_p da);
int isEmptyDynArray(dynArr_p da);

//-----------------------------------------------------------------------------
//  Miscellaneous functions.
//-----------------------------------------------------------------------------
void die(const char *message);
void printDynArray(dynArr_p da);

# endif
