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
#  $Log: wget.py,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

import os
import sys
import getopt
import re
import urllib2

prog = sys.argv[0]
usage = 'USAGE: wget.py URL [-O | --output-document] FILE'
URL = target = None

try:
    opts, remainder = getopt.gnu_getopt(sys.argv[1:], 'O:', ['output-document'])
except getopt.GetoptError as err:
    print("{}: {}".format(prog, err.msg))
    print usage
    exit(1)

for opt, arg in opts:
    if opt in ('-O', '--output-document'):
        target = arg

if(len(remainder) == 1):
    URL = remainder[0]
else:
    print("{}: improper number of arguments".format(prog))
    print usage
    exit(1)

# If no target was specified, get the last part of the URL and use that
if(not target):
    target = re.search('^.*/(.+?)/??$', URL).group(1)

# Attempt to access and open URL
try:
    req = urllib2.Request(URL)
    response = urllib2.urlopen(req)
except urllib2.URLError as err:
    print "Error accessing {}: {}".format(URL, err.reason)
    exit(1)
except urllib2.HTTPError as err:
    print "Error accessing {}: {} {}".format(URL, err.code, err.reason)
    exit(1)

# Don't stomp on data
dest_path = os.path.realpath(target)
if os.path.exists(dest_path):
    print "{} already exists".format(dest_path)
    print usage
    exit(1)

# Try to open open output file for writing
try:
    dest = open((dest_path), 'w')
except (OSError, IOError) as err:
    "Error creating {}: {}".format(target, err.strerror)
    exit(1)

# Try to write to output file
try:
    dest.write(response.read())
except (OSError, IOError) as err:
    "Error writing to {}: {}".format(target, err.strerror)
    exit(1)
