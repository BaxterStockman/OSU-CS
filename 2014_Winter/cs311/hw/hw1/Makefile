#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment 1
#
#
#  $RCSfile: Makefile,v $
#  $Revision: 1.2 $
#  $Author: schreibm $
#  $Date: 2014/01/13 21:06:49 $
#  $Log: Makefile,v $
#  Revision 1.2  2014/01/13 21:06:49  schreibm
#  Added personal details at top
#
#  Revision 1.1  2014/01/13 21:01:56  schreibm
#  Initial commit
#
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall --pedantic-errors -std=c99
PROGS=check_vowel helloworld odd_even read_file
SOURCES=$(addsuffix .c, $(PROGS))
OBJECTS=$(addsuffix .o, $(PROGS))


all: $(PROGS)

$@: $@.o
	$(CC) $(CFLAGS) -o $@ $@.o

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGS) *.o *~

