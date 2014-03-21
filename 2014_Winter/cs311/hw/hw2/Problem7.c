/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment: 2

  $RCSFile$
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/21 06:27:57 $
  $Log: Problem7.c,v $
  Revision 1.1  2014/01/21 06:27:57  schreibm
  Initial revision

*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/stat.h>

/* Enumerated type used as aid to printing arguments
   in the order invoked by the caller */
enum arg_order { H_ARG, T_ARG, F_ARG };

/* Print usage message */
void usage(FILE *stream, const char *prog_name, int exit_code)
{
    fprintf(stream, "Usage: %s [-h] [-t] [-f FILE]\n", prog_name);
    fprintf(stream,
            "    -h    --hostname   Print machine's hostname\n"
            "    -t    --time       Print current time\n"
            "    -f    --file FILE  Print size of FILE in bytes\n");
    exit(exit_code);
}

int main (int argc, char **argv)
{
    /* Program requires >= 1 arguments */
    if(argc <= 1)
        usage(stderr, argv[0], 1);

    int c;
    int hflag = 0;
    int tflag = 0;
    char *fname = NULL;

    /* Initialize argument order elements to -1;
       arguments with -1 value will not print */
    enum arg_order order[] = { -1, -1, -1 };

    struct utsname uname_pointer;
    time_t time_raw_format;
    struct stat s;

    struct option longopts[] = {
        { "hostname", no_argument,       &hflag, 1   },
        { "time",     no_argument,       &tflag, 1   },
        { "filename", required_argument, 0,      'f' },
        { 0, 0, 0, 0 }
    };

    int option_index = 0;

    /*
       Loop adapted from example given at
       http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     */
    int i = 0;
    while((c = getopt_long(argc, argv, "htf:", longopts, &option_index)) != -1) {
        switch(c) {
            case 0:
                break;
            case('h'):
                hflag = 1;
                order[i] = H_ARG;
                break;
            case('t'):
                tflag = 1;
                order[i] = T_ARG;
                break;
            case('f'):
                fname = optarg;
                order[i] = F_ARG;
                break;
            case('?'):
            default:
                usage(stderr, argv[0], 1);
        }
        i++;
    }

    /* Print output in the order specified by the caller */
    for(i = 0; i < 3; i++) {
        switch(order[i]) {
            /* Print hostname */
            case(H_ARG):
                uname(&uname_pointer);
                printf("Hostname = %s \n", uname_pointer.nodename);
                break;
            /* Print local time */
            case(T_ARG):
                time(&time_raw_format);
                printf("The current local time: %s", ctime(&time_raw_format));
                break;
            /* Print size of file in bytes */
            case(F_ARG):
                if (stat(fname, &s) == 0) {
                    printf("Size of file '%s' is %d bytes\n", fname, (int) s.st_size);
                } else {
                    printf("File '%s' not found\n", fname);
            }
            case(-1):
                break;
            default:
                usage(stderr, argv[0], 1);
        }
    }

    return 0;
}
