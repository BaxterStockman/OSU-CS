CC = gcc
CFLAGS = -Wall -std=c99 -g
DEPS =
OBJ = time_test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


