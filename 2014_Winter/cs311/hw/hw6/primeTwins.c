/*
 * Name: Matt Schreiber
 * Email: schreibm@onid.oregonstate.edu
 * Class: CS311-400
 * Assignment: 6
 */

#include "primeTwins.h"

void
shm_print_primes(int *prime, size_t max_bit) {
    size_t i;
    printf("%u\n", 2);
    for(i = 3; i <= max_bit; i += 2)
        if(testbit(prime, i))
            printf("%zu\n", i);
}

void
shm_print_twin_primes(int *prime, size_t max_bit) {
    size_t i;
    for(i = 3; i <= max_bit; i += 2)
        if(testbit(prime, i) && testbit(prime, (i - 2)))
            printf("%zu %zu\n", (i - 2), i);

}

void
free_twins_array(unsigned ***twin_pairs, size_t len)
{
    size_t i;
    for(i = 0; i < len; i++)
        free((*twin_pairs)[i]);
    free(*twin_pairs);
}

int
fill_twins_array(unsigned **twin_pairs, int *map, size_t max_bit)
{
    size_t i, j = 0;
    for(i = 0; i <= max_bit; i++) {
        if(testbit(map, i) && testbit(map, (i - 2))) {
            twin_pairs[j] = xmalloc(sizeof(unsigned) * 2);
            twin_pairs[j][0] = i - 2;
            twin_pairs[j][1] = i;
            j++;
        }
    }
    /* Return the number of pairs stored */
    return j;
}

void
print_twins_array(unsigned **twin_pairs, size_t len)
{
    size_t i;
    for(i = 0; i < len; i++)
        printf("%u %u\n", twin_pairs[i][0], twin_pairs[i][1]);
}

void
print_bits(int *map, size_t max_bit)
{
    size_t i, j;
    for(i = 0, j = 0; i < max_bit; i++, j++) {
        if(j == 78) {
            //putchar('\n');
            j = 0;
        }
        printf("%zu, %c\n", i, (testbit(map, i) == 1 ? '1' : '0'));
    }
    //putchar('\n');
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  xmalloc
 *  Description:  From the tsearch(3) Linux man page.  Wraps malloc(), retuning
 *                a void pointer on success and exiting on failure.
 * =====================================================================================
 */
void *
xmalloc(unsigned n)
{
    void *p = NULL;
    if((p = malloc(n)))
        return p;
    perror("xmalloc");
    exit(EXIT_FAILURE);
}
