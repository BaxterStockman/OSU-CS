/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment 1

  $RCSfile: odd_even.c,v $
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/13 21:02:03 $
  $Log: odd_even.c,v $
  Revision 1.1  2014/01/13 21:02:03  schreibm
  Initial revision


  from:
  http://www.programmingsimplified.com/c/source-code/c-program-check-odd-even

 */


#include<stdio.h>

#define MAXLEN 255

int main()
{
    char s[MAXLEN];
    int n;

    printf("Enter an integer\n");

    /* Input-getting method altered to prevent buffer overflow
     * risk associated with scanf().  Maybe not a problem with a
     * homework assignment, but hey, you never know. :) */
    fgets(s, MAXLEN, stdin);
    sscanf(s, "%d", &n);

    if ( (n & 1) == 1 )
        printf("Odd\n");
    else
       printf("Even\n");

    return 0;
}
