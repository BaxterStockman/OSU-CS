#include<stdio.h>
#include<stdlib.h>
#include "dynArrayDeque.h"

#define INIT_CAP 10

void pauseOut(const char *msg) {
    printf("%s\n", msg);
    printf("Press any key to continue.\n");
    while(!getchar());
    system("clear");
}

int main() {
    int i;
    deque_p test = dequeInit(INIT_CAP);

    //system("clear");

    // Demonstrate the operation of the
    // dequeAddBack function
    pauseOut("I will now add 1 through 10 to the deque using dequeAddBack.");
    for(i = 1; i <= 10; i++)
        dequeAddBack(test, i);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeRmFront function
    pauseOut("I will now remove the 3 elements at the front of the deque using dequeRmFront.");
    for(i = 0; i < 3; i++)
        dequeRmFront(test);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeRmBack function
    pauseOut("I will now remove the 4 elements at the back of the deque using dequeRmBack.");
    for(i = 0; i < 4; i++)
        dequeRmBack(test);
    printDeque(test);
    putchar('\n');

    // Demonstrate the operation of the
    // dequeAddFront function
    pauseOut("I will now add 3 through -3 to the front to demonstrate wraparound.");
    for(i = 3; i > -4; i--)
        dequeAddFront(test, i);
    printDeque(test);
    putchar('\n');
    printf("Notice that the memory address of the first element is higher than the last.\n");
    putchar('\n');

    // Demonstrate expansion of the array
    pauseOut("I will now add 7 through 15 to the back to demonstrate expansion.");
    for(i = 7; i < 16; i++)
        dequeAddBack(test, i);
    printDeque(test);
    putchar('\n');
    printf("Notice that the memory addresses are now in order.");
    putchar('\n');

    return 0;
}
