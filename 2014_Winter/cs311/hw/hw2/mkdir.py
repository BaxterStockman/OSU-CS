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
#  $Log: mkdir.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import os
import sys
import getopt
import fsutils

prog = sys.argv[0]
usage = 'USAGE: mkdir.py [-p | --parents] DIRECTORY'
parents = False

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], 'p', ['parents'])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)

for opt, arg in opts:
    if opt in ('-p', '--parents'):
        parents = True

if(len(remainder) == 1):
    directory = remainder[0]
else:
    print("{}: no directory specified".format(prog))
    print usage
    exit(1)

fsutils.create_dirs(parents, directory)
