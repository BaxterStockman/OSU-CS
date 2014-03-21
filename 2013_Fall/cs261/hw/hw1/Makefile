CC = gcc
CFLAGS = -Wall -std=c99 -g
DEPS = student.h

default: all

%.c: $^ $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

Q0 : Q0.c
	$(CC) -o $@ $^ $(CFLAGS)
Q1 : Q1.c
	$(CC) -o $@ $^ $(CFLAGS)
Q2 : Q2.c
	$(CC) -o $@ $^ $(CFLAGS)
Q3 : Q3.c
	$(CC) -o $@ $^ $(CFLAGS)
Q4 : Q4.c student.h
	$(CC) -o $@ $^ $(CFLAGS)
Q5 : Q5.c
	$(CC) -o $@ $^ $(CFLAGS)
all:
	gcc -Wall -std=c99 -o Q0 Q0.c
	gcc -Wall -std=c99 -o Q1 Q1.c
	gcc -Wall -std=c99 -o Q2 Q2.c
	gcc -Wall -std=c99 -o Q3 Q3.c
	gcc -Wall -std=c99 -o Q4 Q4.c
	gcc -Wall -std=c99 -o Q5 Q5.c

