#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 5

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
		-std=gnu99
PROGS=pipeline uniqify
OBJECTS=$(addsuffix .o, $(PROGS))

PROFILE ?= 0
ifeq ($(PROFILE), 1)
	CFLAGS += -pg
endif

all: $(PROGS)

pipeline: pipeline.o
	$(CC) $(CFLAGS) $< -o $@

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) -c $^ -o $@

uniqify: uniqify.o
	$(CC) $(CFLAGS) $^ -o $@

uniqify.o: uniqify.c
	$(CC) $(CFLAGS) -c $^ -o $@

test: pipeline uniqify
	sh pipeline.bash -f pipeout.txt < words.txt > wc.txt 2>/dev/null
	./pipeline -f mypipeout.txt < words.txt > mywc.txt 2>/dev/null
	diff wc.txt mywc.txt
	diff pipeout.txt mypipeout.txt
	sh uniqify.bash < websters.txt > uniq.txt
	./uniqify -n 5 < websters.txt > myuniq.txt
	diff uniq.txt myuniq.txt

# Note: this Perl script requires
# the IPC::Exe CPAN module
time: uniqify
	perl time_uniqify.pl

clean:
	-@rm -f $(PROGS) $(OBJECTS) *~ *# 2>/dev/null

