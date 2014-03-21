#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "primeTwins.h"

#define MAXLEN(l) (((l) % 8 == 0 ? ((l / 8) + 2) + 1)
#define SHM_CREAT_FLAGS (O_CREAT | O_RDWR)
#define SHM_OPEN_FLAGS (O_RDWR)
#define SHM_OPEN_PERMS (S_IRUSR | S_IWUSR)
#define MMAP_RDWR (PROT_READ | PROT_WRITE)

static int threads = 1;
static int quiet_flag;
static size_t limit = UINT_MAX;
static unsigned limit_sqrt;
/* Shared memory address */
static int *bitmap = NULL;
/* Length of bit array */
static size_t bitmap_len;
/* Array to store thread identifiers */
pthread_t *thread_arr = NULL;
/* Mutex for locking get_next_prime() */
static pthread_mutex_t *mtx = NULL;
/* Counter variable for keeping track
 * of how many processes are marking
 * composite numbers */
static int *counter = NULL;
static unsigned *highest_curr_prime = NULL;

/*
 * A hardcoded array of prime numbers used as the starting
 * point for each thread.  For instance, if there
 * were three threads, the first would start at
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

static void * thread_exec(void *arg);

static void clean_up(void);

int main(int argc, char *argv[])
{
    int i;
    int zero_twos = 0xAAAAAAAA;
    unsigned bits_set;

    /* Read and store command line options */
    parse_opts(argc, argv);

    /* Get square root of 'limit' */
    limit_sqrt = (unsigned)sqrt_ceiling(limit);
    //printf("Ceiling of square root of %u: %u\n", limit, limit_sqrt);

    /* 'limit' represents the largest number to
     * check for primeness.  bitmap_len returns the
     * number of bytes necessary to store a bitmap
     * capable of containing at least 'limit + 1' bits
     * (The '+ 1' is to account for the fact that
     * zero gets included in the bitmap). */
    bitmap_len = get_byte_len(limit + 1);
    //size_t lim_plus = limit + 1;
    //printf("We need %zu bytes to hold %zu bits.\n", bitmap_len, lim_plus);
    //exit(EXIT_SUCCESS);
    /* My methods for checking, setting, and clearing
     * bits (apparently) do reads of 4 bytes long.  The
     * following line protects against attempts to read
     * past the end of the bitmap by allocating just enough
     * extra space. */
    bitmap_len += sizeof(int);

    bitmap = xmalloc(bitmap_len);

    /* Initialize all even numbers to 0 */
    memset(bitmap, zero_twos, bitmap_len);
    /* Set the bit representing '1' to 0 */
    clearbit(bitmap, 1);
    /* Set the bit representing '2' to 1 */
    setbit(bitmap, 2);

    mtx = xmalloc(sizeof(pthread_mutex_t));
    if(pthread_mutex_init(mtx, NULL) != 0)
        pexit("pthread_mutex_init", EXIT_FAILURE);

    highest_curr_prime = xmalloc(sizeof(unsigned));
    *highest_curr_prime = 0;

    counter = xmalloc(sizeof(int));
    /* Set counter to zero, then increment it
     * by the number of threads */
    *counter = 0;
    __sync_add_and_fetch(counter, threads);

    /* Allocate space for array holding
     * ids of threads */
    thread_arr = xmalloc(sizeof(pthread_t) * threads);

    /* Set up handler for
     * SIGINT and SIGTERM */
    signal(SIGINT, &sig_handler);
    signal(SIGTERM, &sig_handler);

    /* Execute composite-finding threads */
    for(i = 0; i < threads; i++)
        if(pthread_create(&thread_arr[i], NULL, &thread_exec, (void*)(intptr_t)i) != 0)
            pexit("pthread_create", EXIT_FAILURE);

    for(i = 0; i < threads; i++)
        if(pthread_join(thread_arr[i], NULL) != 0)
            pexit("pthread_join", EXIT_FAILURE);


    /* We divide bitmap_len by 4 because there are
     * 4 bytes in every int and get_bits_set reads
     * in lengths of sizeof(int) */
    bits_set = get_bits_set(bitmap, bitmap_len / 4);
    clear_singletons(bitmap, limit, bits_set);

    if(quiet_flag != 1 )
        shm_print_twin_primes(bitmap, limit);

    clean_up();

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
                threads = atoi(optarg);
                if(threads > 30 || threads < 1)
                    usage(stderr, argv[0], "possible number of threads: 1-30", EXIT_FAILURE);
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
    fprintf(stream, "   -c  --concurrency   Number of threads to create\n");
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
    int i;
    switch(sig) {
        case SIGINT:
        case SIGTERM:
            for(i = 0; i < threads; i++)
                pthread_join(thread_arr[i], NULL);
            clean_up();
            exit(EXIT_FAILURE);
            break;
        default:
            break;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  thread_exec
 *  Description:  Passed to pthread_create().  Marks composite numbers.
 *
 *  Thanks to user Dietrich Epp at
 *  http://stackoverflow.com/questions/8487380/how-to-cast-an-integer-to-void-pointer
 *  for the tip on using <stdint.h> and 'intptr_t' to avoid compiler warnings
 *  about casting to pointer from an integer of different size.
 * =====================================================================================
 */
static void *
thread_exec(void *arg)
{
    int i;
    int status;
    unsigned curr_prime;
    //int *i_p = (int *)arg;

    //i = *i_p;
    i = (intptr_t)arg;
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
         * Wait for all threads to enter mark_composites().
         * This helps to ensure that a process doesn't inadvertently
         * choose as its next prime a multiple of a prime whose multiples
         * are currently being marked by another thread, but which
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

    return NULL;
}

static void clean_up(void) {
    if(pthread_mutex_destroy(mtx) != 0)
        pexit("pthread_mutex_destroy", EXIT_FAILURE);

    free(bitmap);
    free(mtx);
    free(counter);
    free(highest_curr_prime);
    free(thread_arr);
}
