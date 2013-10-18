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
