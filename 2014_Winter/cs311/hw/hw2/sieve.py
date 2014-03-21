#!/usr/bin/env python2

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400

#  $RCSfile: sieve.py,v $
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:57 $
#  $Log: sieve.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#
#  Revision 1.2  2014/01/14 04:24:08  schreibm
#  Fixed error in crank_prime_array()'s while loop
#
#  Revision 1.1  2014/01/13 21:02:18  schreibm
#  Initial revision
#


# for sqrt()
import math


# I found this at http://forums.xkcd.com/viewtopic.php?f=11&t=61300
# while searching for information about Python's list comprehension
# facilities as applied to prime numbers.  Turns out I was barking up
# the wrong tree, but user ichaleybin's approach to implementing
# the Sieve of Eratosthenes using a generator function is really quite
# clever.  I've reproduced it below with clarifying/illuminating changes
# to variable names and explanatory comments.
def gen_prime():
    # Declare empty dictionary
    nums = {}
    # Skip 0 and 1 because we know they're not prime
    curr_num = 2

    while True:
        if curr_num not in nums:
            # Curr_num is prime; yield it
            yield curr_num
            # Set the key curr_num^2 to point to curr_num
            # (e.g., the key '4' would  correspond to value '2')
            #
            # (We can skip all numbers less than this since we
            # already know that those numbers are multiples of primes
            # we've already encountered)
            nums[curr_num * curr_num] = [curr_num]

        else:
            # If curr_num is a key in the dictionary, its corresponding
            # values will all be prime factors of curr_num
            for prime_factor in nums[curr_num]:
                # For each prime factor, set the value of key 'prime_factor
                # + curr_num' to an empty list (if no values exist yet)
                # and append the prime factor to the list.
                #
                # This ensures that, for every composite number encountered,
                # we generate all composite numbers that are equal to the given
                # composite numbers plus one of its prime factors.
                nums.setdefault(prime_factor + curr_num, [])    \
                    .append(prime_factor)
            # Delete the curr_num key from the dictionary so that it doesn't
            # grow inefficiently large
            del nums[curr_num]

        # Move on to the next-highest number
        curr_num += 1


def gen_prime_array(lim):
    prime_array = [prime for prime in gen_prime(lim)]
    return prime_array, len(prime_array)


# This was my first-ish attempt at the Sieve.  It's slow.
def make_prime_array(lim):
    # Declare list filled with 1s
    prime_locs = [1] * lim
    # Get the square root of the 'lim' argument
    sqrt_lim = math.sqrt(lim)
    # Set the value of the first index
    # (which corresponds to the number '1') to 0
    prime_locs[0] = 0

    # Loop through all numbers less that the square
    # root of 'lim', beginning at 2
    base_num = 1
    while base_num < sqrt_lim:
        base_num += 1
        # If the number at the index corresponding to
        # base_num -1 is 1, then base_num is prime and
        # we don't touch the '1' at index base_num - 1
        if prime_locs[base_num - 1] == 1:
            # Set curr_num to twice the value of base_num
            curr_base = base_num * 2
            # Step through the array, setting all multiples
            # of base_num (other than base_num itself) to 0
            for num in range(curr_base, lim, base_num):
                prime_locs[num - 1] = 0

    prime_array = [n for n in range(1, lim) if prime_locs[n - 1] == 1]
    return prime_array, len(prime_array)


# Note: in revising the xkcd link given above, I realized that I
# inadvertently reimplemented user PM 2Ring's approach.  Subconscious
# influences aside, I did come up with this myself! :)
def crank_prime_array(lim):
    # Declare list and initialize
    prime_locs = [False, False, True, True] +  \
        ([False, True] * ((lim / 2) - 2))
    # Get the square root of the 'lim' argument
    sqrt_lim = int(math.sqrt(lim))
    # Loop through all numbers less that the square
    # root of 'lim', beginning at 2
    for base_num in range(3, sqrt_lim):
        if(prime_locs[base_num]):
            prime_locs[(base_num * base_num):lim:base_num] =    \
                [False] * (1 + (lim - 1) / base_num - base_num)

    prime_array = []
    i = 0
    while(i < lim):
        if prime_locs[i] is True:
            prime_array.append(i)
        i += 1
    return prime_array, len(prime_array)
