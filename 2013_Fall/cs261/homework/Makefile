CC = gcc
CFLAGS = -Wall -std=c99 -lm
DEPS =
OBJ = math_test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.c: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


