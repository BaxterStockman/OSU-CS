#!/usr/bin/env python2

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 2
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:57 $
#  $Log: superlative.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#


# A generator function which yields the results of an (arithmetic)
# operation on consecutive digits of 'num' in groups of 'gap'.
#
# Pre:
#   - num is an integer
#   - gap is an integer
#   - func is a function of two arguments that returns the result of an
#     operation on the digits of 'num'
#
# Post:
#   - yields the results of sequentially applying 'func' to a given group
#     of consecutive digits using the reduce() built-in
def yield_interval(num, gap, func):
    # split 'num' into a list of individual integers
    digits = [int(n) for n in str(num)]
    i = 0
    lim = len(digits)
    # Apply 'func' to group of digits from 'i' to 'i+gap'-1
    # until the end of 'digits' is reached
    while ((i + gap) <= lim):
        yield reduce(func, digits[i:i + gap])
        i += 1


# Returns a 'superlative' value over sets of consecutive digits
# in a number based upon a comparison function and an (arithmetic) operation.
# For instance, if the comparison function is c(x, y) = x > y, the
# (arithmetic) operation is f(x, y) = x * y, and 'gap' = 5, get_superlative() will
# return the largest product of five consecutive digits in 'num'.
#
# Pre:
#   - num is an integer
#   - gap is an integer
#   - func is a function of two arguments that returns the result of an
#     operation on the digits of 'num'
#   - compar is a function of two arguments which returns True or False
#
# Post:
#   - returns the subset of digits of 'num' which displays the 'most' of
#     whatever property is tested for by 'compar'
def get_superlative(num, gap, func, compar):
    # Assign generator to 'values'
    values = yield_interval(num, gap, func)
    # Initialize variable which will eventually
    # hold the return value to the value of the
    # first result yielded by the generator
    superlative = values.next()
    # Iterate over the rest of the values yielded
    # by the generator, running the comparison function
    # on it and the heretofore superlative value
    for n in values:
        if compar(n, superlative):
            superlative = n
    return superlative
