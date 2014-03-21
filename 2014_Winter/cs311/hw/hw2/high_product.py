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
#  $Log: high_product.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import getopt
import sys
import os
import superlative
import argparse

prog = sys.argv[0]
usage = 'USAGE: high_product.py [-i | --interval] INTEGER ' \
        '{ [-n | --number] INTEGER | [f | --file] FILE }'
number = filename = interval = None

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], 'i:n:f:', ['interval='
                                                                'number=',
                                                                'file='])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)

for opt, arg in opts:
    if opt in ('-i', '--interval'):
        try:
            interval = int(arg)
        except ValueError as err:
            print("'{}' is not an integer; interval must be integer").format(arg)
            print usage
            exit(1)
    if opt in ('-n', '--number'):
        try:
            number = int(arg)
        except ValueError as err:
            print("'{}' is not an integer").format(arg)
            print usage
            exit(1)
    elif opt in ('-f', '--file'):
        filename = arg

# Make sure that the user specified a number or file
if not(number or filename):
    print("{}: too few arguments".format(prog))
    print usage
    exit(1)

# Make sure that the user specified a number or file
if (number and filename):
    print("{}: specify either number or file".format(prog))
    print usage
    exit(1)

if(remainder):
    print usage
    exit(1)

# Attempt to read file and store number in file in 'args.number'
if(filename):
    try:
        f = open(os.path.realpath(filename), 'r')
    except (IOError, OSError) as err:
        print("Failure reading '{}': {}".format(filename, err.strerror))
        exit(1)

    try:
        in_val = f.read()
        number = int(in_val)
    except ValueError:
        print("'{}' is not an integer").format(in_val)
        print usage
        exit(1)

print superlative.get_superlative(number, interval,
                                  lambda x, y: x * y,
                                  lambda x, y: x > y)
