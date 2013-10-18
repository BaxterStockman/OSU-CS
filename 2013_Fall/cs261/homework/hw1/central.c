#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    char *cmds[] = {"./Q0", "./Q1", "./Q2"};
    int i;

    for(i = 0; i < 3; i++)
        system(cmds[i]);
}
