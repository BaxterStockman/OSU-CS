/*
 * Name: Matt Schreiber
 * Email: schreibm@onid.oregonstate.edu
 * Class: CS311-400
 * Assignment: 6
 */

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "primeTwins.h"

#define SHM_CREAT_FLAGS (O_CREAT | O_RDWR)
#define SHM_OPEN_FLAGS (O_RDWR)
#define SHM_OPEN_PERMS (S_IRUSR | S_IWUSR)
#define MMAP_RDWR (PROT_READ | PROT_WRITE)

static const char *shm_name = "schreibm-primeMProc";
static int procs = 1;
static int shm_fd = -1;
static int quiet_flag;
static size_t limit = UINT_MAX;
static unsigned limit_sqrt;
/* Shared memory address */
static int *bitmap = NULL;
/* List of process ids */
static pid_t *pids = NULL;
/* Parent pid */
static pid_t parent_pid = -1;
/* Child pid */
static pid_t child_pid = -1;
/* Length of bit array */
static size_t bitmap_len;
/* Mutex for locking get_next_prime() */
static pthread_mutex_t *mtx = NULL;
static void *mtx_addr = NULL;
/* Address for holding state variables */
static int *state_addr = NULL;
/* Counter variable for keeping track
 * of how many processes are marking
 * composite numbers */
static int *counter = NULL;
static unsigned *highest_curr_prime = NULL;

/*
 * A hardcoded array of prime numbers used as the starting
 * point for each subprocess.  For instance, if there
 * were three subprocesses, the first would start at
 * '3', the second at '5', and the third at '7'.  '2'
 * is not included because it's easier to just set even
 * bit positions when initializing the bitmap
 */
static unsigned init_primes[] = {   3,  5,  7,  11, 13,  17,  19,  23,  29,  31,
                                    37, 41, 43, 47, 53,  59,  61,  67,  71,  73,
                                    79, 83, 89, 97, 101, 103, 107, 109, 113, 127    };

static inline void parse_opts(int argc, char *argv[]);
static inline void usage(FILE *stream, const char *prog_name, const char *err, int exit_code);

static void sig_handler(int sig);
static void subproc_exec(int num_procs);
static void clean_up_procs(pid_t *pidp);

int main(int argc, char *argv[])
{
    int i;
    int zero_twos = 0xAAAAAAAA;
    unsigned bits_set;
    pthread_mutexattr_t mtx_attr;

    /* Set up handler for SIGCHLD,
     * SIGINT, and SIGTERM */
    signal(SIGCHLD, &sig_handler);
    signal(SIGINT, &sig_handler);
    signal(SIGTERM, &sig_handler);

    /* Read and store command line options */
    parse_opts(argc, argv);

    /* Get square root of 'limit' */
    limit_sqrt = (unsigned)sqrt_ceiling(limit);

    /* Allocate space for an array of pid_t's */
    pids = malloc(sizeof(pid_t) * (procs + 1));
    if(pids == NULL)
        pexit("malloc", EXIT_FAILURE);

    /* Store process id of parent */
    pids[0] = parent_pid = getpid();

    /* 'limit' represents the largest number to
     * check for primeness.  bitmap_len returns the
     * number of bytes necessary to store a bitmap
     * capable of containing at least 'limit + 1' bits
     * (The '+ 1' is to account for the fact that
     * zero gets included in the bitmap). */
    bitmap_len = get_byte_len(limit + 1);
    /* My methods for checking, setting, and clearing
     * bits do reads of 4 bytes long.  The following line
     * protects against attempts to read past the end of
     * the bitmap */
    bitmap_len += sizeof(int);

    /* Create a shared memory area and map it */
    shm_init(shm_name, SHM_CREAT_FLAGS, SHM_OPEN_PERMS, &shm_fd);
    if(ftruncate(shm_fd, (off_t)bitmap_len) == -1)
        pexit("ftruncate", EXIT_FAILURE);
    bitmap = mmap_init(NULL, bitmap_len, PROT_READ | PROT_WRITE,
            MAP_SHARED, shm_fd, 0);

    /* Initialize all even numbers to 0 */
    memset(bitmap, zero_twos, bitmap_len);
    /* Set the bit representing '1' to 0 */
    clearbit(bitmap, 1);
    /* Set the bit representing '2' to 1 */
    setbit(bitmap, 2);

    /* Thanks to Roy at
     * http://royontechnology.blogspot.com/2007/06/comparison-of-pthreadprocessshared-in.html
     * for this means of setting up a mutex that can be
     * shared between processes */
    mtx_addr = mmap_init(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mtx = (pthread_mutex_t*)mtx_addr;
    if(pthread_mutexattr_init(&mtx_attr) != 0)
        pexit("pthread_mutexattr_init", EXIT_FAILURE);
    //if(pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_ERRORCHECK) != 0)
    //    pexit("pthread_settype", EXIT_FAILURE);
    if(pthread_mutexattr_setpshared(&mtx_attr, PTHREAD_PROCESS_SHARED) != 0)
        pexit("pthread_setpshared", EXIT_FAILURE);
    if(pthread_mutex_init(mtx, &mtx_attr) != 0)
        pexit("pthread_mutex_init", EXIT_FAILURE);
    if(pthread_mutexattr_destroy(&mtx_attr) != 0)
        pexit("pthread_mutexattr_destroy", EXIT_FAILURE);

    /* Map a segment of shared memory for
     * the counter that monitors whether
     * subprocesses have finished marking
     * composites. */
    state_addr = mmap_init(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    counter = state_addr;
    highest_curr_prime = (unsigned*)&state_addr[1];
    *counter = 0;
    *highest_curr_prime = 0;

    /* Execute composite-finding subprocesses */
    subproc_exec(procs);

    for (i = 0; i < procs; i++)
        wait(NULL);

    /* We divide bitmap_len by 4 because there are
     * 4 bytes in every int */
    bits_set = get_bits_set(bitmap, bitmap_len / 4);
    clear_singletons(bitmap, limit, bits_set);

    if(quiet_flag != 1 )
        shm_print_twin_primes(bitmap, limit);

    clean_up_procs(pids);

    return 0;
}

static void parse_opts(int argc, char *argv[])
{
    /* Variable for getting user-defined
     * limit on size of largest twin prime */
    unsigned long in_limit;

    /* Struct for use with GNU getopt_long() */
    struct option longopts[] = {
        { "quiet",          no_argument,        &quiet_flag,    1   },
        { "max",            required_argument,  0,              'm' },
        { "concurrency",    required_argument,  0,              'c' },
        { 0, 0, 0, 0 }
    };

    /* Variable to hold flag currently
     * being ready by getopt() */
    int c;

    /* Index of option currently
     * being considered */
    int opt_index = 0;

    /*
     * Loop adapted from example given at
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     */
    while((c = getopt_long(argc, argv, "qm:c:",
                    longopts, &opt_index)) != -1) {
        switch(c) {
            case 'q':
                quiet_flag = 1;
                break;
            case 'm':
                in_limit = strtoul(optarg, NULL, 10);
                if(in_limit > UINT_MAX)
                    usage(stderr, argv[0], "specified maximum not in range", EXIT_FAILURE);
                limit = in_limit;
                break;
            case 'c':
                procs = atoi(optarg);
                if(procs > 30 || procs < 1)
                    usage(stderr, argv[0], "possible number of subprocesses: 1-30", EXIT_FAILURE);
                break;
            case('?'):
            default:
                usage(stderr, argv[0], "unrecognized option", EXIT_FAILURE);
                break;
        }
    }
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  usage
 *  Description:  Prints a usage message and exits
 * =====================================================================================
 */
static void
usage(FILE *stream, const char *prog_name, const char *err, int exit_code)
{
    fprintf(stream, "Error: %s\n", err);
    fprintf(stream, "Usage: %s [-q] [-m MAX TWIN] [-c PROCESSES]\n", prog_name);
    fprintf(stream, "   -q  --quiet         Suppress output\n");
    fprintf(stream, "   -m  --max           Value of maximum twin prime to find\n");
    fprintf(stream, "   -c  --concurrency   Number of subprocesses to fork\n");
    exit(exit_code);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_handler
 *  Description:  Signal handler function.  Cleans up the processes and queues
 *                on receipt of SIGINT and SIGTERM, as well as SIGCHLD so long
 *                as errno does not indicate successful child exit.
 * =====================================================================================
 */
static void
sig_handler(int sig)
{
    switch(sig) {
        case SIGCHLD:
            if(errno == 0)
                break;
        case SIGINT:
        case SIGTERM:
            clean_up_procs(pids);
            exit(EXIT_FAILURE);
            break;
        default:
            break;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subproc_exec
 *  Description:  fork()s 'num_procs' children, each of which gets its own
 *                message queue.  Children process messages through the
 *                process_queue() function, then exit.
 * =====================================================================================
 */
static void
subproc_exec(int num_procs)
{
    int i;
    int status;
    unsigned curr_prime;

    *counter = 0;
    __sync_add_and_fetch(counter, num_procs);
    for(i = 0; i < num_procs; i++) {
        switch(pids[i] = fork()) {
            case -1:
                perror("fork");
                _exit(EXIT_FAILURE);
                break;
            case 0:
                child_pid = pids[i];
                curr_prime = init_primes[i];

                /* Lock this section of code  */
                status = pthread_mutex_lock(mtx);
                if(status != 0)
                    pexit("pthread_mutex_lock", EXIT_FAILURE);
                if(curr_prime > (*highest_curr_prime))
                    (*highest_curr_prime) = curr_prime;
                status = pthread_mutex_unlock(mtx);
                if(status != 0)
                    pexit("pthread_mutex_unlock", EXIT_FAILURE);

                while(curr_prime <= limit_sqrt) {
                    /* Decrement counter on entry. */
                    if((status = pthread_mutex_lock(mtx)) != 0)
                        pexit("pthread_mutex_lock", EXIT_FAILURE);
                    atomic_decr(counter);
                    if((status = pthread_mutex_unlock(mtx)) != 0)
                        pexit("pthread_mutex_unlock", EXIT_FAILURE);

                    mark_composites(bitmap, curr_prime, limit);

                    /*
                     * Wait for all subprocesses to enter mark_composites().
                     * This helps to ensure that a process doesn't inadvertently
                     * choose as its next prime a multiple of a prime whose multiples
                     * are currently being marked by another subprocess, but which
                     * has not yet been marked itself.
                     *
                     * This is not a foolproof means of guaranteeing that a process gets
                     * a valid prime number when it calls get_next_prime(), but testing
                     * (printing out the return values of all calls to get_next_prime() and
                     * checking them against a list of known primes) indicates that it works
                     * well enough, at least on the Flip servers.
                     */
                    while((*counter) != 0);

                    /* Lock this section of code  */
                    if((status = pthread_mutex_lock(mtx)) != 0)
                        pexit("pthread_mutex_lock", EXIT_FAILURE);
                    curr_prime = get_next_prime(bitmap, limit, highest_curr_prime);
                    atomic_incr(counter);
                    if((status = pthread_mutex_unlock(mtx)) != 0)
                        pexit("pthread_mutex_unlock", EXIT_FAILURE);
                }
                atomic_decr(counter);

                free(pids);
                _exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  clean_up_procs
 *  Description:  Handles terminating children, destroys message queues, and
 *                releases dynamically-allocated global arrays.
 * =====================================================================================
 */
static void
clean_up_procs(pid_t *pidp)
{
    int i;
    if(child_pid == -1) {
        for(i = 1; i < procs; i++)
            kill(pidp[i], SIGTERM);
        shm_unlink(shm_name);
    }
    close(shm_fd);
    unlink(shm_name);
    if(pthread_mutex_destroy(mtx) != 0)
        pexit("pthread_mutex_destroy", EXIT_FAILURE);
    free(pids);
}
