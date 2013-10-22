#include<stdio.h>
#include<stdlib.h>
#include "dynArrayStack.h"

int main() {
    int i;
    int cap = 5;
    printf("Initializing test stack...\n");
    dynArr_p test = initDynArray(5);
    printf("There are %d elements in the test stack.\n", sizeDynArray(test));
    putchar('\n');

    printf("Adding the following series of values to the test stack:\n");
    for(i = 1; i <= cap * 2; i++) {
        printf("%d ", i);
        pushDynArray(test, i);
    }
    putchar('\n');
    putchar('\n');

    printf("Now test bag contains:\n");
    printDynArray(test);
    putchar('\n');

    if(containsDynArray(test, 6)) {
        printf("Test bag contains 6; removing it.\n");
        rmValDynArray(test, 6);
    }
    printDynArray(test);
    putchar('\n');

    printf("Adding 6 back to array in original position...\n");

    putDynArray(test, 5, 6);
    printDynArray(test);
    putchar('\n');

    printf("Popping values of the top of the stack:\n");
    while(sizeDynArray(test)) {
        printf("Popping %d...\n", topDynArray(test));
        popDynArray(test);
    }
    putchar('\n');

    printf("Destroying test stack...\n");
    freeDynArray(test);
}


