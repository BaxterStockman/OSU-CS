/*
  Name: Matt Schreiber
  Email: schreibm@onid.oregonstate.edu
  Class: CS311-400
  Assignment: 3

  $RCSFile$
  $Revision: 1.1 $
  $Author: schreibm $
  $Date: 2014/01/28 05:49:43 $
  $Log: homework3.c,v $
  Revision 1.1  2014/01/28 05:49:43  schreibm
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
            write_string(__VA_ARGS__);  \
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

/* Fill buffer with null bytes */
void
empty_buffer(char *buf, size_t len) {
    size_t i;
    for(i = 0; i < len; i++)
        buf[i] = '\0';
}

/*
 * Prints a string using write()
 * Pre:
 *  - fd is a valid file descriptor
 *  - s is a string
 *  - format_num is the number of format strings in s
 *
 * Post:
 *  - s has been written to the file described by
 *    fd with all format strings interpolated
 */
void
write_string(int fd, char *s, ...)
{
    va_list vals;
    char *pres;
    char *temp = s;

    int psign_bare = 0, psign_db = 0;

    while((pres = strstr(temp, "%%")) != NULL) {
        psign_db++;
        temp = pres + 2;
    }

    temp = s;
    while((pres = strchr(temp, '%')) != NULL) {
        psign_bare++;
        temp = pres + 1;
    }

    int format_str_num = psign_bare - psign_db;

    /* This is not an ideal solution for allocating
     * space, since FORMAT_BUF might be way too much
     * or way too little for any given value that will be
     * interpolated into 'to_write'.  However, just
     * glancing at the source code for vfprint() nearly
     * made my head explode, so this will have to do :) */
    char to_write[strlen(s) + (format_str_num * FORMAT_BUF)];

    va_start(vals, s);
    vsprintf(to_write, s, vals);
    va_end(vals);

    if(write(fd, to_write, strlen(to_write)) == -1) {
        perror("write() failed");
        exit(1);
    }
}

/*
 * Add a marker after a specified char in a string.
 *
 * Pre:
 *  - s is a string
 *  - delim is the char after which to add the marked
 *  - mark is the marker string
 *
 * Post
 *  - mark has been added after all instances of delim in s
 */
char *
add_markers(char *s, const char delim, char *mark)
{
    int len = strlen(s);
    int mark_len = strlen(mark);

    /* Since I am using this function to insert markers
     * after newlines, get a rough estimate of the number
     * of newlines */
    int line_len = 79;
    int new_len = len + ((len / line_len) * (mark_len + 1));

    /* String that will hold s + markers */
    char *marked_s = NULL;
    if((marked_s = malloc(new_len)) == NULL) {
        perror("Allocation failed");
        exit(1);
    }

    /* Temporary string used as aid to allocating
     * more memory for marked_s when needed */
    char *temp;

    /* Loop counters/char insertion indices */
    int i, j, mark_offset = 0;

    /* Insert first mark at beginning of first line
     * if delim is a whitespace character */
    switch(delim) {
        case('\n'):
        case('\t'):
        case(' '):
            for(i = 0; i < mark_len; i++)
                marked_s[i] = mark[i];
            mark_offset += mark_len;
    }

    /* Loop through s, copying its elements into
     * marked string and adding marks after
     * occurrences of delim */
    for(i = 0; i < len; i++) {
        /* Allocate more memory if marked_s
         * is getting close to full */
        if(i + mark_offset + mark_len >= new_len) {
            if((temp = memcpy(malloc(new_len * 2),
                            marked_s, (size_t)new_len)) == NULL) {
                perror("Allocation failed");
                exit(1);
            }
            new_len *= 2;
            free(marked_s);
            marked_s = temp;
        }

        /* Copy char at i in s to i + mark_offset in
         * marked s, adding mark if the char == delim */
        if((marked_s[i + mark_offset] = s[i]) == delim) {
            for(j = 0; j < mark_len; j++)
                marked_s[i + mark_offset + j + 1] = mark[j];
            mark_offset += mark_len;
        }
    }

    /* Place null byte at end of marked_s */
    marked_s[i + mark_offset] = '\0';
    return marked_s;
}

/*
 * Seek a certain number of bytes into a file,
 * read a given number of bytes, and write
 * to standard output.
 *
 * Pre:
 *  - fd is a valid file descriptor descriging
 *    the file to be read
 *  - offset is the number of bytes from whence
 *    to seek
 *  - whence is the anchor point in the file
 *    from which to seek
 *  - buf is a pointer sufficient to hold the
 *    bytes read from the file
 *  - count is the number of bytes to read
 *
 * Post:
 *  - buf contains the bytes read from the
 *    file described by fd
 *  - The contents of buf have been written
 *    to standard output
 */
void
seek_read_write(int fd, off_t offset, int whence, void *buf, size_t count)
{
    /* Seek to offset specified by
     * 'offset' and 'whence' */
    if(lseek(fd, offset, whence) == -1) {
        perror("lseek() failed");
        exit(1);
    }

    /* Read 'count' bytes into 'buf' */
    if(read(fd, buf, count) == -1) {
        perror("read() failed");
        exit(1);
    }

    /* Add markers after newline characters
     * in 'buf' to make it easier to identify
     * what was read from the file described
     * by fd */
    char *to_write = add_markers((char*)buf, '\n', "-> ");

    write_string(1, to_write);
    /* Write the string that has markers added */
    //if(write(1, to_write, len) == -1) {
    //    perror("write() failed");
    //    exit(1);
    //}

    /* I have to run empty_buffer() here because
     * the OS keeps allocating the same memory on
     * calls to malloc(), even across separate calls
     * to add_markers().  This meant that 'leftover'
     * characters from previous calls to add_markers()
     * kept printing if they weren't explicitly overwritten. */
    //empty_buffer(to_write, count);
    free(to_write);
    to_write = NULL;
}


int main (int argc, char **argv)
{
    /* Program requires >= 4 arguments */
    if(argc < 4)
        usage(stderr, argv[0], "Insufficient arguments", 1);

    /* Variable to hold flag currently
     * being ready by getopt() */
    int c;
    /* File descriptor, initialized to -1 */
    int fd = -1;
    /* Verbosity flag, initialized to 0 */
    int vflag = 0;
    /* Filename, initialized to NULL */
    char *fname = NULL;
    /* Various file read lengths, initialized to 0 */
    size_t len = 0, elen = 0, bufsize = 0;
    /* File offsets, initialized to 0 */
    off_t o_one = 0, o_two = 0;

    /* Number of mandatory options */
    int mand_opt_num = 4;
    /* Number of mandatory options seen */
    int seen = 0;

    /* Struct for use with GNU getopt_long() */
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

    /* Make sure all mandatory parameters
     * were entered */
    if(seen < mand_opt_num)
        usage(stderr, argv[0], "Insufficient number of arguments", 1);

    struct stat file_stats;
    if(stat(fname, &file_stats) == -1) {
        perror("fstat");
        exit(1);
    }

    off_t off_sum = o_one + o_two;
    if(off_sum < 0)
        usage(stderr, argv[0], "Attempt to read past file boundary", 1);
    if((size_t)off_sum + len > file_stats.st_size
            || (size_t)o_one + len > file_stats.st_size)
        usage(stderr, argv[0], "Attempt to read past file boundary", 1);


    /* Set bufsize to the larger of len and elen */
    bufsize = (len > elen ? len : elen) + 1;
    char buf[bufsize];

    if((fd = open(fname, O_RDONLY)) == -1) {
        perror("open() failed");
        exit(1);
    }

    /* Print back options entered if the '-v'
     * flag was invoked */
    write_verb(vflag, 1,
                "Options:\n -f: %s\n -l: %zu\n -o: %lld\n -O: %lld\n -e: "
                "%zu\n\n", fname, len, o_one, o_two, elen);

    char *demarc = "<%-8s>-------------------------------------------------\n";

    /* Print the two (or three, if elen was given)
     * strings defined by the options chosen by
     * the user */
    empty_buffer(buf, bufsize);
    write_verb(vflag, 1,
               "Printing %zu bytes offset %lld bytes from location defined "
               "by SEEK_SET\n", len, o_one);
    printf(demarc, "offset 1");
    seek_read_write(fd, o_one, SEEK_SET, (void*)buf, len);
    write_string(1, "\n\n");

    empty_buffer(buf, bufsize);
    write_verb(vflag, 1,
                "Printing %zu bytes offset %lld bytes from location defined "
                "by SEEK_CUR\n", len, o_two);
    printf(demarc, "offset 3");
    seek_read_write(fd, o_two, SEEK_CUR, (void*)buf, len);
    write_string(1, "\n\n");

    if(elen >= 1) {
        empty_buffer(buf, bufsize);
        write_verb(vflag, 1,
                    "Printing %zu bytes offset %lld bytes from location "
                    "defined by SEEK_END\n", elen, (off_t)(elen * -1));
        printf(demarc, "offset 3");
        seek_read_write(fd, (off_t)(elen * -1), SEEK_END, (void*)buf, elen);
        write_string(1, "\n\n");
    }

    if(close(fd) == -1) {
        perror("close() failed");
        exit(1);
    }

    return 0;
}
