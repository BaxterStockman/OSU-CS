/* CS261- Assignment 1 - Q.2*/
/* Name: Matt Schreiber
 * Date: 10-12-2013
 * Solution description:    Passes three values to foo(), which manipulates
 *                          them in certain ways.  Prints their initial
 *                          values to the console, and also prints their
 *                          values after being passed to foo().
 */

#include <stdio.h>
#include <stdlib.h>

int foo(int* a, int* b, int c){
    /*Set a to double its original value*/
    *a *= 2;

    /*Set b to half its original value*/
    *b /= 2;

    /*Assign a+b to c*/
    c = *a + *b;

    /*Return c*/
    return c;
}

int main(){
    /*Declare three integers x,y and z and initialize them to 5, 6, 7 respectively*/
    int x = 5, y = 6, z = 7;

    /*Print the values of x, y and z*/
    printf("x = %d, y = %d, & z = %d\n", x, y, z);

    /*Call foo() appropriately, passing x,y,z as parameters*/
    int f = foo(&x, &y, z);

    /*Print the value returned by foo*/
    printf("Value returned by foo() = %d\n", f);

    /*Print the values of x, y and z again*/
    printf("After running foo(), x = %d, y = %d, & z = %d\n", x, y, z);

    /*Is the return value different than the value of z?  Why?*/

    /* The return value of foo() is not equal to z because the value
     * foo() returns is the value of the variable c local to foo(),
     * which was initially set to the value of z.  Because z was passed
     * by value, its value remains unchanged within the scope of main().
    */

    return 0;
}


