/*
 * Name: Matt Schreiber
 * Email: schreibm@onid.oregonstate.edu
 * Class: CS311-400
 * Assignment: 6
 *
 * Thanks to Cameron Hilton for sharing the GCC built-in
 * atomic operations, such as __sync_add_and_fetch().
 * Link to post:
 * https://piazza.com/class/hpkbw91sckj3cr?cid=187
 */

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * This variable and the function get_bits_set() are from:
 * http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable
 */
static const unsigned char bit_table[256] =
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

static inline void pexit(const char *error, const int status);

static inline size_t get_byte_len(size_t limit);

static inline int shm_init(const char *name, int oflag, mode_t mode, int *fd);
static inline void * mmap_init(void *addr, size_t len, int prot, int flags, int fd, off_t offset);

void shm_print_primes(int *map, size_t max_bit);
void shm_print_twin_primes(int *map, size_t max_bit);

static inline void clearbit(int *map, unsigned bit);
static inline void setbit(int *map, unsigned bit);
static inline int testbit(int *map, unsigned bit);

static inline int atomic_incr(int *num);
static inline int atomic_decr(int *num);

static inline unsigned mark_composites(int *map, unsigned init_prime, size_t max_bit);
static inline unsigned get_bits_set(int *map, size_t len);
static inline void clear_singletons(int *map, size_t max_bit, unsigned count);
static inline unsigned get_next_prime(int *map, size_t max_bit, unsigned *curr_val);
static inline long long unsigned sqrt_ceiling(double n);

int fill_twins_array(unsigned **twin_pairs, int *map, size_t len);
void print_bits(int *map, size_t max_bit);
void print_twins_array(unsigned **twin_pairs, size_t max_bit);
void free_twins_array(unsigned ***twin_pairs, size_t max_bit);
void * xmalloc(unsigned n);

static inline void
pexit(const char *error, const int status)
{
    perror(error);
    exit(status);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_byte_len
 *  Description:  Returns the number of bytes necessary to create a bitmap capable of
 *                representing a list of numbers 'limit' bits long.
 * =====================================================================================
 */
static inline size_t get_byte_len(size_t limit)
{
    double limit_dub = (double)limit;
    return ceil(limit_dub / 8);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  shm_init
 *  Description:  Wrapper for shm_open()
 * =====================================================================================
 */
static inline int
shm_init(const char *name, int oflag, mode_t mode, int *fd)
{
    if(((*fd) = shm_open(name, oflag, mode)) == -1)
        pexit("shm_open", EXIT_FAILURE);
    return *fd;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  mmap_init
 *  Description:  Wrapper for mmap()
 * =====================================================================================
 */
static inline void *
mmap_init(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void * ret_addr;
    if((ret_addr = mmap(addr, len, prot, flags, fd, offset)) == MAP_FAILED)
        pexit("mmap", EXIT_FAILURE);
    return ret_addr;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  setbit
 *  Description:  Sets the bit'th bit in 'map'
 * =====================================================================================
 */
static inline void
setbit(int *map, unsigned bit)
{
    __sync_or_and_fetch(&map[bit / 32], 1 << (bit % 32));
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  clearbit
 *  Description:  Clears the bit'th bit in 'map'.
 * =====================================================================================
 */
static inline void
clearbit(int *map, unsigned bit)
{
    __sync_and_and_fetch(&map[bit / 32], ~(1 << (bit % 32)));
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  testbit
 *  Description:  Tests whether the bit'th bit in 'map' is set.  Returns '1' if so, '0'
 *                if not.
 * =====================================================================================
 */
static inline int
testbit(int *map, unsigned bit)
{
    return ((map[bit / 32] & (1 << (bit % 32))) != 0);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  atomic_incr
 *  Description:  Atomically increments 'num'.
 * =====================================================================================
 */
static inline int
atomic_incr(int *num)
{
    return __sync_add_and_fetch(num, 1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  atomic_decr
 *  Description:  Atomically decrements 'num'.
 * =====================================================================================
 */
static inline int
atomic_decr(int *num)
{
    return __sync_sub_and_fetch(num, 1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  mark_composites
 *  Description:  Marks all multiples of 'init_prime' in 'map' up to 'max_bit', beginning
 *                with the square of 'init_prime'
 * =====================================================================================
 */
static inline unsigned
mark_composites(int *map, unsigned init_prime, size_t max_bit)
{
    unsigned curr;
    unsigned interval = 2 * init_prime;
    size_t max_bit_less_interval = max_bit - interval;

    for(curr = (init_prime * init_prime); curr <= max_bit_less_interval; curr += interval)
        clearbit(map, curr);

    curr -= interval;

    /* Protect against accidental overflow */
    if((max_bit_less_interval) >= curr && (UINT_MAX - interval) >= curr)
        clearbit(map, curr + interval);
    return curr;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_singletons
 *  Description:  Clears the bits in 'map' representing non-twin primes.
 *                'count' is the total number of primes in the map.
 * =====================================================================================
 */
static inline void
clear_singletons(int *map, size_t max_bit, unsigned count) {
    unsigned i;
    int has_lower_twin = 0;
    size_t max_lower_twin = max_bit - 2;

    for(i = 3; i <= max_lower_twin && count > 0; i += 2) {
        if(testbit(map, i) == 1) {
            count--;
            if(testbit(map, i + 2)) {
                has_lower_twin = 1;
            } else if(has_lower_twin) {
                has_lower_twin = 0;
            } else {
                clearbit(map, i);
            }
        }
    }
    if(!(testbit(map, i) && testbit(map, i - 2)))
        clearbit(map, i);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_next_prime
 *  Description:  Returns the next-highest prime number after the number pointed to
 *                by 'curr_val'.
 *          Pre:  'curr_val' must be odd-numbered.
 * =====================================================================================
 */
static inline unsigned
get_next_prime(int *map, size_t max_bit, unsigned *curr_val) {
    /* Increment curr_val */
    for((*curr_val) += 2; (*curr_val) <= max_bit; (*curr_val) += 2)
        /* Check whether current bit is marked as prime */
        if(testbit(map, *curr_val) == 1)
            break;
    return *curr_val;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  sqrt_ceiling
 *  Description:  Returns the ceiling of the square root of 'n'
 * =====================================================================================
 */
static inline long long unsigned
sqrt_ceiling(double n) {
    return (unsigned long long)ceil((sqrt(n)));
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_bits_set
 *  Description:  Returns the number of bits set in 'map'
 * =====================================================================================
 */
static inline unsigned
get_bits_set(int *map, size_t len)
{
    int i;
    unsigned bits_set = 0;
    unsigned bit_check;

    for(i = 0; i < len; i++) {
        bit_check = map[i];
        bits_set += bit_table[bit_check & 0xff] +
                    bit_table[(bit_check >> 8) & 0xff] +
                    bit_table[(bit_check >> 16) & 0xff] +
                    bit_table[bit_check >> 24];
    }

    return bits_set;
}

