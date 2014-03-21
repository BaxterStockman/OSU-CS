/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment 1

  $RCSfile: read_file.c,v $
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/13 21:02:10 $
  $Log: read_file.c,v $
  Revision 1.1  2014/01/13 21:02:10  schreibm
  Initial revision


  from:
  http://www.programmingsimplified.com/c-program-read-file

 */

#include <string.h>
#include "read_file.h"

#define FN_MAXLEN 25

char * chomp(char *s);

int main()
{
    char ch, file_name[FN_MAXLEN];
    //FILE *fp;
    int fd;
    int num_read;

    printf("Enter the name of file you wish to see\n");
    fgets(file_name, FN_MAXLEN, stdin);

    chomp(file_name);

    fd = open(file_name, O_RDONLY);
    if (fd < 0)
    {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }
    printf("The contents of %s file are :\n", file_name);

    for (;;)
    {
        num_read = read(fd, &ch, sizeof(ch));
        if (num_read < 0)
        {
            perror("Error while reading the file\n");
            exit(EXIT_FAILURE);
        }
        if (num_read == 0)
        {
           break;
        }
       write(STDOUT_FILENO, &ch, sizeof(ch));
    }
    return ch;
}

/* Chomp the trailing newline */
char * chomp(char *s) {
    size_t len = strlen(s);
    if(s[len-1] == '\n')
        s[len-1] = '\0';
    return s;
}
