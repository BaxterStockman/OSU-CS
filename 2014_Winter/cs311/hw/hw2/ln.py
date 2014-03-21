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
#  $Log: ln.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import sys
import getopt
import fsutils

prog = sys.argv[0]
usage = 'USAGE: ln.py [-s | --symbolic] TARGET LINK_NAME'
symbolic = False

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], 's', ['symbolic'])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)

for opt, arg in opts:
    if opt in ('-s', '--symbolic'):
        symbolic = True

if(len(remainder) == 2):
    target, link = remainder
else:
    print("{}: improper number of arguments".format(prog))
    print usage
    exit(1)

fsutils.create_link(symbolic, target, link)
