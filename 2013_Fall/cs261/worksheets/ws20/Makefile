CC = gcc
CFLAGS = -Wall -std=c99 -g
DEPS = dynArrayDeque.h
OBJ = dynArrayDeque.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


