#!/usr/bin/env python2

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 2
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:56 $
#  $Log: class_link.py,v $
#  Revision 1.1  2014/01/21 06:27:56  schreibm
#  Initial revision
#

import os
import sys
import getopt
import fsutils

prog = sys.argv[0]
usage = 'USAGE: class_link.py [-c | --class] CLASS_NAME ' \
        '[-t | --term] TERM_NAME'
symbolic = False

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], 'c:t:', ['class=',
                                                               'term='])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)


for opt, arg in opts:
    if opt in ('-c', '--class'):
        course = arg
    if opt in ('-t', '--term'):
        term = arg

if(len(remainder) != 0 or len(opts) != 2):
    print("{}: improper number of arguments".format(prog))
    print usage
    exit(1)

home_dir = os.environ['HOME']
class_dir = "{}/{}/{}".format(home_dir, term, course)

for folder in ['assignments', 'examples', 'exams',
               'lecture_notes', 'submissions']:
    fsutils.create_dirs(1, "{}/{}".format(class_dir, folder))

fsutils.create_link(1, "/usr/local/classes/eecs/{}/{}/src/README"
                    .format(term, course),
                    "{}/README"
                    .format(class_dir))
fsutils.create_link(1, "/usr/local/classes/eecs/{}/{}/src"
                    .format(term, course),
                    "{}/src_class"
                    .format(class_dir))
