#ifndef ArrayBagStack
# define ArrayBagStack

# define TYPE int
# define EQ(a, b) (a == b)

//struct arrayBagStack {
//TYPE data [100];
//int count;
//};

typedef struct arrayBagStack *arrayBagStack_t;

void initArray(arrayBagStack_t b);
void addArray (arrayBagStack_t b, TYPE v);
int containsArray (arrayBagStack_t b, TYPE v);
int removeArray (arrayBagStack_t b, TYPE v);
int sizeArray (arrayBagStack_t b);

void pushArray (arrayBagStack_t b, TYPE v);
TYPE topArray (arrayBagStack_t b);
void popArray (arrayBagStack_t b);
int isEmptyArray (arrayBagStack_t b);
# endif
