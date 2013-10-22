#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include<ctype.h>
#include<errno.h>
#include<stdint.h>

typedef struct stack {
    int *data;
    int size;
    int cap;
} stack;

void die(const char *message) {
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

int is_int(const char *in) {
    while(*in)
        if(!isdigit(*in++))
            return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc != 3 || !is_int(argv[1]) || !is_int(argv[2]))
        die("Usage: time_test <array capacity> <max value>");

    int max = atoi(argv[2]);
    int init_cap = atoi(argv[1]);
    int i, dbls=0;

    struct timeval *start = malloc(sizeof(struct timeval));
    struct timeval *end = malloc(sizeof(struct timeval));
    long start_ms, end_ms;

    stack *test = malloc(sizeof(stack));
    test->data = malloc(sizeof(int) * init_cap);
    test->size = 0;
    test->cap = init_cap;
    printf ("Max size = %d; initial cap = %d.\n", max, test->cap);

    gettimeofday(start, NULL);
    for(i = 1; i <= max; i++, (test->size)++) {
        if(test->size >= test->cap) {
            test->cap *= 2;
            test->data = realloc(test->data, sizeof(int) * test->cap);
            dbls++;
        }
        test->data[i-1] = i;
    }
    gettimeofday(end, NULL);

    start_ms = 1000000 * start->tv_sec + start->tv_usec;
    end_ms = 1000000 * end->tv_sec + end->tv_usec;
    long double cost = (end_ms - start_ms)/(long double)(max);

    printf("It took %ld microseconds to add %d values to array of start size %d through %d doublings.\n",
            end_ms - start_ms, max, init_cap, dbls);
    printf("Average cost of one addition to array is %Lf microseconds.\n", cost);

    free(start);
    free(end);
    free(test->data);
    free(test);
}
