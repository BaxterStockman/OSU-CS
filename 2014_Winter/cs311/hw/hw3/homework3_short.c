/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment: 3

  $RCSFile$
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/28 05:49:52 $
  $Log: homework3_short.c,v $
  Revision 1.1  2014/01/28 05:49:52  schreibm
  Initial revision

*/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
#include <pcre.h>

#define FORMAT_BUF 255

/* Simplifies turning on verbose messages
 * when '-v' flag is invoked */
#define write_verb(V, ...)              \
    do {                                \
        if(V >= 1)                      \
            fprintf(__VA_ARGS__);       \
    } while(0)

/* Print usage message */
void
usage(FILE *stream, const char *prog_name, const char *err, int exit_code)
{
    fprintf(stream, "Error: %s\n", err);
    fprintf(stream, "Usage: %s -f FILE -l LENGTH -o OFFSET -O OFFSET "
                    "[-e LENGTH] [-v]\n", prog_name);
    fprintf(stream,
            "   -f    --file FILE    Name of file to examine\n"
            "   -l    --length       length in bytes to write\n"
            "   -o    --offset1      length in bytes from start of file to "
                                    "begin writing first sequence\n"
            "   -O    --offset2      length in bytes from first offset to "
                                    "begin writing second sequence\n"
            "   -e    --elength      length in bytes from end of file to "
                                    "write\n"
            "   -v    --verbose      turn on verbose output\n");
    exit(exit_code);
}

void
seek_read_write(int fd, off_t offset, int whence, void *buf, size_t count)
{
    if(lseek(fd, offset, whence) == -1) {
        perror("lseek() failed");
        exit(1);
    }

    if(read(fd, buf, count) == -1) {
        perror("read() failed");
        exit(1);
    }

    if(write(1, buf, count) == -1) {
        perror("write() failed");
        exit(1);
    }
}


int main (int argc, char **argv)
{
    if(argc < 4)
        usage(stderr, argv[0], "Insufficient arguments", 1);

    int c;
    int fd = -1;
    int vflag = 0;
    char *fname = NULL;
    size_t len = 0, elen = 0, bufsize = 0;
    off_t o_one = 0, o_two = 0;

    int mand_opt_num = 4;
    int seen = 0;

    struct option longopts[] = {
        { "verbose",  no_argument,       &vflag, 1   },
        { "file",     required_argument, 0,      'f' },
        { "length",   required_argument, 0,      'l' },
        { "offset1",  required_argument, 0,      'o' },
        { "offset2",  required_argument, 0,      'O' },
        { "elength",  required_argument, 0,      'e' },
        { 0, 0, 0, 0 }
    };

    int opt_index = 0;

    /*
     * Loop adapted from example given at
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     *
     * Loop through command line arguments, incrementing
     * 'seen' when we encounter a mandatory argument.
     *
     * All options besides '-v' require an argument.
     */
    while((c = getopt_long(argc, argv, "vf:l:o:O:e:",
                    longopts, &opt_index)) != -1) {
        switch(c) {
            case 0:
                break;
            case('v'):
                vflag = 1;
                break;
            case('f'):
                fname = optarg;
                seen++;
                break;
            case('l'):
                len = strtoul(optarg, 0, 10);
                if(len < 1)
                    usage(stderr, argv[0], "length must be positive", 1);
                seen++;
                break;
            case('o'):
                o_one = atoll(optarg);
                if(o_one < 0)
                    usage(stderr, argv[0], "offset1 must be nonnegative", 1);
                seen++;
                break;
            case('O'):
                o_two = atoll(optarg);
                seen++;
                break;
            case('e'):
                elen = strtoul(optarg, 0, 10);
                if((long long)elen < 1)
                    usage(stderr, argv[0], "elength must be positive", 1);
                break;
            case('?'):
            default:
                usage(stderr, argv[0], "Unrecognized option", 1);
        }
    }

    if(seen < mand_opt_num)
        usage(stderr, argv[0], "Insufficient number of arguments", 1);

    struct stat file_stats;
    if(stat(fname, &file_stats) == -1) {
        perror("fstat");
        exit(1);
    }

    off_t off_sum = o_one + o_two;
    if(off_sum < 0
            || (size_t)off_sum + len > file_stats.st_size
            || (size_t)o_one + len > file_stats.st_size)
        usage(stderr, argv[0], "Attempt to read past file boundary", 1);


    bufsize = (len > elen ? len : elen) + 1;
    char buf[bufsize];

    if((fd = open(fname, O_RDONLY)) == -1) {
        perror("open() failed");
        exit(1);
    }

    write_verb(vflag, stdout,
                "Options:\n -f: %s\n -l: %zu\n -o: %lld\n -O: %lld\n -e: "
                "%zu\n\n", fname, len, (long long)o_one, (long long)o_two, elen);

    char *demarc = "<%-8s>-------------------------------------------------\n";

    write_verb(vflag, stdout,
               "\nPrinting %zu bytes offset %lld bytes from location defined "
               "by SEEK_SET\n", len, (long long)o_one);
    printf(demarc, "offset 1");
    seek_read_write(fd, o_one, SEEK_SET, (void*)buf, len);
    putchar('\n');

    write_verb(vflag, stdout,
                "\nPrinting %zu bytes offset %lld bytes from location defined "
                "by SEEK_CUR\n", len, (long long)o_two);
    printf(demarc, "offset 2");
    seek_read_write(fd, o_two, SEEK_CUR, (void*)buf, len);
    putchar('\n');

    if(elen >= 1) {
        write_verb(vflag, stdout,
                    "\nPrinting %zu bytes offset %lld bytes from location "
                    "defined by SEEK_END\n", elen, (long long)(elen * -1));
        printf(demarc, "offset 3");
        seek_read_write(fd, (off_t)(elen * -1), SEEK_END, (void*)buf, elen);
        putchar('\n');
    }


    if(close(fd) == -1) {
        perror("close() failed");
        exit(1);
    }

    return 0;
}
