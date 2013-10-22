#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int main() {
    int val;
    char *str = "54321";
    while(*str) {
        printf("%c -- %d\n", *str, isdigit(*str));
        if(*str++ == '\0')
            printf("Encountered null char\n");
    }
}
