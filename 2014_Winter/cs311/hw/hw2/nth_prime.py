#!/usr/bin/env python

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 2
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:57 $
#  $Log: nth_prime.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import sieve
import sys
import getopt


def nth(num):
    # Use generator to access next prime until nth prime reached
    count = 0
    prime_gen = sieve.gen_prime()
    while(count < num):
        prime = prime_gen.next()
        count += 1

    print prime
    return prime


prog = sys.argv[0]
usage = 'USAGE: nth_prime.py INTEGER'

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], '', [''])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)

if(len(remainder) == 1):
    num = int(remainder[0])
else:
    print("{}: improper number of arguments".format(prog))
    print usage
    exit(1)

nth(num)
