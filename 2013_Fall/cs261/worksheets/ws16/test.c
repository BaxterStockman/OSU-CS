#include<stdio.h>
#include<stdlib.h>
#include "dynArrayStack.h"

int main() {
    int i;
    int cap = 5;
    printf("Initializing test stack...\n");
    dynArr_p test = initDynArray(5);
    printf("There are %d elements in the test stack.\n", sizeDynArray(test));

    printf("Adding the following series of values to the test stack:\n");
    for(i = 1; i <= cap * 2; i++) {
        printf("%d ", i);
        pushDynArray(test, i);
    }
    putchar('\n');

    printf("Popping values of the top of the stack:\n");
    while(sizeDynArray(test)) {
        printf("Popping %d...\n", topDynArray(test));
        popDynArray(test);
    }

    printf("Destroying test stack...\n");
    freeDynArray(test);
}


