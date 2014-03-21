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
#  $Log: who.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import sys
import subprocess
import shlex
#import os
#import argparse


## Set up command line argument parsing
#parser = argparse.ArgumentParser()
## Create switch for symlink
#opts = [('-b', '--boot', 'time of last system boot'),
#        ('-d', '--dead', 'print dead processes'),
#        ('-l', '--login', 'print system login processes'),
#        ('-p', '--process', 'print active processes spawned by init'),
#        ('-r', '--runlevel', 'print current runlevel'),
#        ('-T', '--mesg', 'add user\'s message status as +, - or ?'),
#        ('-t', '--time', 'print last system clock change'),
#        ('-u', '--users', 'list users logged in')]
#
#for opt in opts:
#    flag, lflag, desc = opt
#    parser.add_argument(flag, lflag,
#                        action='store_true',
#                        default=False,
#                        help=desc)
#
## Parse arguments
#args = parser.parse_args()
#arg_vals = vars(args)
#
#arg_list = ["--" + arg for arg in arg_vals.keys() if arg_vals[arg] is True]
#arg_list.insert(0, 'who')

# These three lines appear to do a better job than the larger chunk of code above

# Split argument vector into individual strings for passing
# to the subprocess
arg_list = shlex.split(' '.join(sys.argv))
# Replace 'who.py' with 'who' in the first position of the argument list
arg_list[0] = 'who'
# Pass argument list to the subprocess
proc = subprocess.Popen(arg_list)
