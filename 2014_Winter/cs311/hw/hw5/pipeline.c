/*
*  Name: Matt Schreiber
*  Email: schreibm@onid.oregonstate.edu
*  Class: CS311-400
*  Assignment: 5
*/

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Print usage message */
static void
usage(FILE *stream, const char *prog_name, const char *err, int exit_code)
{
    fprintf(stream, "Error: %s\n", err);
    fprintf(stream, "Usage: %s -f FILE\n", prog_name);
    fprintf(stream, "   -f  --file     Program output destination file\n");
    exit(exit_code);
}

static inline void
close_pipe(int pfd, int is_child)
{
    if (close(pfd) == -1) {
        perror("close");
        if(is_child)
            _exit(EXIT_FAILURE);
        else
            exit(EXIT_FAILURE);
    }
}

static inline void
connect_pipe(int oldfd, int newfd, int is_child)
{
    if (oldfd != newfd) {
        if (dup2(oldfd, newfd) == -1) {
            perror("dup2");
        if(is_child)
            _exit(EXIT_FAILURE);
        else
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    /* Number of processed to exec */
    int num_procs = 5;
    /* Used to keep track of which
     * program to exec */
    int proc;
    /* Filename to write to
     * from exec'd 'tee' process*/
    char *filename;
    /* Pipe file descriptors */
    int newpfd[2], oldpfd[2];
    /* Status holder for call to waitpid() */
    int status;
    /* Parent pid -- used to check whether
     * we're in the parent or child process */
    pid_t parent, child, sto_pid;


    /* Struct for use with GNU getopt_long() */
    struct option longopts[] = {
        { "file", required_argument,  0,  'f'   },
        { 0, 0, 0, 0 }
    };

    /* Variable to hold flag currently
     * being ready by getopt() */
    int c;

    /* Index of option currently
     * being considered */
    int opt_index = 0;

    if(argc < 3)
        usage(stderr, argv[0], "insufficient number of options", EXIT_FAILURE);

    /*
     * Loop adapted from example given at
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     */
    while((c = getopt_long(argc, argv, "f:",
                    longopts, &opt_index)) != -1) {
        switch(c) {
            case 'f':
                filename = optarg;
                break;
            case('?'):
            default:
                usage(stderr, argv[0], "unrecognized option", EXIT_FAILURE);
                break;
        }
    }

    /* Get pid of parent process
     * and store so that we can tell
     * in the following loop whether
     * we're in a parent or child */
    parent = sto_pid = getpid();

    /* Loop 'num_procs' times, creating a pipe and
     * child process each time. */
    for(proc = 0; proc < num_procs; proc++) {
        /* Bail out if we're in a child process */
        if(sto_pid != parent)
            break;
        if (pipe(newpfd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        /* Create child process */
        switch (child = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                /* Store child pid */
                sto_pid = getpid();
                /* If there is a previous child: */
                if (proc > 0) {
                    /* Connect the read side of the old pipe
                     * to the stdin of the current child */
                    connect_pipe(oldpfd[0], STDIN_FILENO, 1);
                    /* Close both ends of the old pipe */
                    close_pipe(oldpfd[0], 1);
                    close_pipe(oldpfd[1], 1);
                }

                /* If there is going to be a subsequent child: */
                if (proc < num_procs - 1) {
                    /* Close the read side of the current pipe
                     * (the file description itself is still open
                     * because it )*/
                    close_pipe(newpfd[0], 1);
                    /* Connect the stdout of the current child
                     * to the write side of the current pipe */
                    connect_pipe(newpfd[1], STDOUT_FILENO, 1);
                    /* Close the duplicated file descriptor */
                    close_pipe(newpfd[1], 1);
                }

                /* Thanks to questioner Dale and commenters at
                 * http://stackoverflow.com/questions/15188115/multiple-execlp-not-working
                 * for the following switch statement.
                 * Uses 'proc' to decide which executable
                 * to call for current child */
                switch(proc) {
                    case 0:
                        execlp("rev", "rev", NULL);
                        break;
                    case 1:
                        execlp("sort", "sort", NULL);
                        break;
                    case 2:
                        execlp("uniq", "uniq", "-c", NULL);
                        break;
                    case 3:
                        execlp("tee", "tee", filename, NULL);
                        break;
                    case 4:
                        execlp("wc", "wc", NULL);
                        break;
                    default:
                        break;
                }

            default:
                /* If there is a previous child,
                 * close old pipe file descriptors */
                if(proc > 0) {
                    close_pipe(oldpfd[0], 0);
                    close_pipe(oldpfd[1], 0);
                }
                /* If there is a next child, copy
                 * new pipe file descriptors to old
                 * pipe file descriptors. This enables
                 * linking the old output to the new input
                 * on the next loop. */
                if(proc < num_procs - 1) {
                    oldpfd[0] = newpfd[0];
                    oldpfd[1] = newpfd[1];
                }
                /* Wait until the last child has completed */
                if(proc == num_procs - 1) {
                    waitpid(child, &status, 0);
                    /* Close last pipe */
                    close_pipe(newpfd[0], 0);
                    close_pipe(newpfd[1], 0);
                }
                break;
        }
    }

    /* If we're in a child process,
     * call _exit() instead of return */
    if(sto_pid != parent)
        _exit(EXIT_SUCCESS);

    return 0;
}
