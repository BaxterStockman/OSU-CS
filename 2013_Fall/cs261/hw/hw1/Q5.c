/* CS261- Assignment 1 - Q.5*/
/* Name: Matt Schreiber
 * Date: 10-12-2013
 * Solution description:    Calls stdlib isupper, tolower, and toupper
 *                          in order to convert a user-entered word
 *                          into StUdLyCaPs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STR 80

/*converts ch to upper case, assuming it is in lower case currently*/
char toUpperCase(char ch){
     return ch-'a'+'A';
}

/*converts ch to lower case, assuming it is in upper case currently*/
char toLowerCase(char ch){
     return ch-'A'+'a';
}

void sticky(char* word){
    int i;
    word[0] = toupper(word[0]);
    // Sets char at current index to lowercase if previous char was
    // uppercase, or to uppercase if previous char was lowercase.
    for(i = 1; word[i] != '\0'; i++)
        word[i] = isupper(word[i-1]) ? tolower(word[i]) : toupper(word[i]);
}


int main(){
    char word[MAX_STR];

    /*Read word from the keyboard using scanf*/
    printf("Please enter a word:  ");
    scanf("%s", word);

    /*Call sticky*/
    sticky(word);

    /*Print the new word*/
    printf("In StudlyCaps: %s\n", word);

    return 0;
}
