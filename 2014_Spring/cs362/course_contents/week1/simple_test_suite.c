#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct container {
    int contents[255];
} container;

void simple_test(int input, int expected, container* c, int(*f)(int n, container* c)) {
    assert(f(input, c) == expected);
}
