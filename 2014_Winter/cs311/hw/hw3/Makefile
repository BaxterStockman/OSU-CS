#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 3
#
#  $RCSFile$
#  $Revision: 1.1 $
#  $Author: schreibm $
#  $Date: 2014/01/28 05:49:57 $
#  $Log: Makefile,v $
#  Revision 1.1  2014/01/28 05:49:57  schreibm
#  Initial revision
#
#

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall --pedantic-errors -std=c99
PROGS=homework3 homework3_short
OBJECTS=$(addsuffix .o, $(PROGS))

all: $(PROGS)

homework3: homework3.o
	$(CC) $(CFLAGS) -o $@ $@.o

homework3.o: homework3.c
	$(CC) $(CFLAGS) -c $^ -o $@

homework3_short: homework3_short.o
	$(CC) $(CFLAGS) -o $@ $@.o

homework3_short.o: homework3_short.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(PROGS) $(OBJECTS) *~ *#

