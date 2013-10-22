#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "arrayBagStack.h"

int main() {
    int i, j, k;
    int fill = 50;

    srand(time(NULL));

    arrayBagStack_t test = malloc(sizeof(test));
    initArray(test);
    printf("After initialization, test has %d members.\n", sizeArray(test));
    popArray(test);

    for(i = 0; i < 50; i++)
        pushArray(test, i);

    printf("Adding the following series of values to test:\n");
    j = sizeArray(test);
    for(i = 1; i <= j; i++) {
        k = topArray(test);
        printf("%2d ", k);
        popArray(test);
        if(!(i % 20))
            putchar('\n');
    }

    putchar('\n');

    for(i = 0; i < 50; i++)
        pushArray(test, i);

    for(i = 0; i < 5; i++) {
        j = rand() % (sizeArray(test));
        if(containsArray(test, j))
            printf("test contains '%d'\n", j);
    }


    printf("After pushing %d values, test has %d members.\n", fill, sizeArray(test));
}
