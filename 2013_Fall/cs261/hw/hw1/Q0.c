/* CS261- Assignment 1 - Q. 0*/
/* Name: Matt Schreiber
 * Date: 10-12-2013
 * Solution description:    Program uses the dereference operator to print
 *                          the value pointed to by pointer, the
 *                          ampersand operator to print the address in
 *                          memory of a variable, and the variable
 *                          name of a pointer to print the pointer's
 *                          address in memory.
 */

#include <stdio.h>
#include <stdlib.h>

void fooA(int* iptr);

int main(){

    /*declare an integer x*/
    int x = 10;

    /*print the address of x*/
    printf("Address of x: %p\n", &x);

    /*Call fooA() with the address of x*/
    fooA(&x);

    /*print the value of x*/
    printf("Value of x: %d\n", x);

    return 0;
}

void fooA(int* iptr){
     /*Print the value pointed to by iptr*/
    printf("iptr points to value %d\n", *iptr);

     /*Print the address pointed to by iptr*/
    printf("iptr points to address %p\n", iptr);

     /*Print the address of iptr itself*/
    printf("iptr is located at address %p\n", &iptr);
}

