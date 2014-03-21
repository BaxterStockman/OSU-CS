/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment 1

  $RCSfile: check_vowel.c,v $
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/13 21:01:35 $
  $Log: check_vowel.c,v $
  Revision 1.1  2014/01/13 21:01:35  schreibm
  Initial revision


  from:
  http://www.programmingsimplified.com/c/source-code/c-program-check-vowel

 */


#include <stdio.h>

int main()
{
    char ch;

    printf("Input a character\n");
    ch = fgetc(stdin);

    switch(ch)
    {
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'i':
        case 'I':
        case 'o':
        case 'O':
        case 'u':
        case 'U':
            printf("%c is a vowel.\n", ch);
            break;
        default:
            printf("%c is not a vowel.\n", ch);
    }

    return ch;
}
