#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 2
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/21 06:27:57 $
#  $Log: Makefile,v $
#  Revision 1.1  2014/01/21 06:27:57  schreibm
#  Initial revision
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall --pedantic-errors -std=c99
PROGS=Problem7
SOURCES=$(addsuffix .c, $(PROGS))
OBJECTS=$(addsuffix .o, $(PROGS))


all: $(PROGS)

$@: $@.o
	$(CC) $(CFLAGS) -o $@ $@.o

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGS) *.o *~

