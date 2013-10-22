#ifndef dynArrayStack
# define dynArrayStack

# define TYPE int
# define EQ(a, b) (a == b)

typedef struct dynArr *dynArr_p;

void die(const char *message);
dynArr_p initDynArray(int initCap);
void freeDynArray(dynArr_p da);
void __setCapDynArr(dynArr_p da);
int sizeDynArray(dynArr_p da);
void addDynArray(dynArr_p da, TYPE elem);
void rmLocDynArray(dynArr_p da, int pos);
TYPE getLocDynArray(dynArr_p da, int);
void putDynArray(dynArr_p da, int pos, TYPE elem);

//-----------------------------------------------------------------------------
//  Stack interface functions.
//-----------------------------------------------------------------------------

void pushDynArray(dynArr_p da, TYPE elem);
TYPE topDynArray(dynArr_p da);
void popDynArray(dynArr_p da);
int isEmptyDynArray(dynArr_p da);
# endif
