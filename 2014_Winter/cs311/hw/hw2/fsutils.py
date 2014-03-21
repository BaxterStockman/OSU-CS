#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 2
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:56 $
#  $Log: fsutils.py,v $
#  Revision 1.1  2014/01/21 06:27:56  schreibm
#  Initial revision
#

import os


# Attempts to create directories in 'dirs', emitting an error message but not
# dying on failure.
#
# Pre:
#   - 'recurs' is a Boolean value which represents whether or
#     not to create parent directories for directories in 'dirs'
#   - 'dirs' contains a list of directory names
#
# Post:
#   - All directories in 'dirs' have been created, except those for
#     which generate OS errors
def create_dirs(recurs, dirs):
    # Create dictionary to use for pseudo-switch statement
    # depending on whether '-p' flag is chosen
    dir_func = {True: os.makedirs,
                False: os.mkdir,
                }
    if str(dirs):
        dirs = [dirs]

    # Attempt to create directories, but don't die if errors occur
    for elem in dirs:
        try:
            dir_func[recurs](os.path.realpath(elem))
        except OSError, err:
            print "Error creating directory {}: {}".format(elem, err.strerror)


# Attempts to create link 'target' pointing at 'source'.
#
# Pre:
#   - 'symbolic' is a Boolean value which represents whether or
#     not to create a symbolic link rather than a hard link
#   - 'link' is the name of the link to be created
#   - 'source' is the name of the file that 'link' points to
#
# Post:
#   - Link has been created, except if the OS produces an error
def create_link(symbolic, target, link):
    # Create dictionary to use for pseudo-switch statement
    # depending on whether '-s' flag is chosen
    link_func = {True: os.symlink,
                 False: os.link,
                 }
    # Attempt to create link
    try:
        link_func[symbolic](os.path.realpath(target), os.path.realpath(link))
    except OSError, err:
        print "Error linking {} to {}: {}".format(link, target, err.strerror)

