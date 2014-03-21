#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "animal.h"

typedef enum { animal, question } STATUS;

struct node {
    STATUS type;
    char *contents;
};

void * xmalloc(unsigned n) {
    void *p;
    p = malloc(n);
    assert(p);
    return p;
}

int compare(const void *lhs_t, const void *rhs_t) {
    struct node *lhs = (struct node *)lhs_t;
    struct node *rhs = (struct node *)rhs_t;

    if(lhs->type == rhs->type && !strcmp(lhs->contents, rhs->contents))
        return 0;

    if(rhs->type == question) {
        if(lhs->type == question)
            return -1;
        else
            return 1;
    }
}
