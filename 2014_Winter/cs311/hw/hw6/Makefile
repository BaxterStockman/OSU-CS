#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 6

CC=gcc
DEBUG=-g
CFLAGS= -Wall \
		-Wshadow \
		-Wunreachable-code \
		-Wredundant-decls \
		-Wmissing-declarations \
		-Wmissing-prototypes \
		-Wold-style-definition \
		-Wdeclaration-after-statement \
		--pedantic-errors \
		-g \
		-std=gnu99
PROGS=primePThread primeMProc
LIBS=rt m pthread
LINK=$(addprefix -l, $(LIBS))
OBJECTS=$(addsuffix .o, $(PROGS)) primeTwins.o

PROFILE ?= 0
ifeq ($(PROFILE), 1)
	CFLAGS += -pg
endif

all: $(PROGS)

primePThread: primePThread.o primeTwins.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINK)

primePThread.o: primePThread.c primeTwins.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)

primeMProc: primeMProc.o primeTwins.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINK)

primeMProc.o: primeMProc.c primeTwins.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)

primeTwins.o: primeTwins.c primeTwins.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)

archive: clean
	tar -jcvf project6-schreiber.bzip --exclude='*.o' --exclude='*.bzip' *

clean:
	-@rm -f $(PROGS) $(OBJECTS) *~ *# 2>/dev/null

