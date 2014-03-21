/* CS261- Assignment 1 - Q.3*/
/* Name: Matt Schreiber
 * Date: 10-12-2013
 * Solution description:    Creates an array filled with randomly-
 *                          generated values, then uses Shell sort
 *                          to sort them from lowest to highest.
 *                          Prints initial array and sorted array
 *                          to the console.
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void sort(int* number, int n){
    /*Sort the given array number , of length n*/
    // Implemented Shell sort.
    int h, i, j, temp;

    h = 1;
    while(h < n)
        h = 3*h + 1;
    h /= 3;

    // Shell sort
    while(h > 0) {
        h /= 3;
        for(i = h; i < n; i++) {
            temp = number[i];
            for(j = i; j >= h && number[j - h] > temp; j -= h)
                number[j] = number[j - h];
            number[j] = temp;
        }
    }
}

int main() {
    /*Declare an integer n and assign it a value of 20.*/
    int n = 20;

    /*Allocate memory for an array of n integers using malloc.*/
    int *number = malloc(n * sizeof(int));

    /*Fill this array with random numbers, using rand().*/
    srand(time(NULL));
    int i;
    for(i = 0; i < n; i++)
        number[i] = rand();

    /*Print the contents of the array.*/
    putchar('\n');
    printf("Before being sorted:\n");
    for(i = 0; i < n; i++)
        printf("Value at index %2d is: %-10d\n", i, number[i]);
    putchar('\n');

    /*Pass this array along with n to the sort() function of part a.*/
    sort(number, n);

    /*Print the contents of the array.*/
    printf("After being sorted:\n");
    for(i = 0; i < n; i++)
        printf("Value at index %2d is: %-10d\n", i, number[i]);
    putchar('\n');

    return 0;
}
