#include <stdio.h>
#include <stdlib.h>

struct mazeNode {
    void *value;
    struct mazeNode *north;
    struct mazeNode *east;
    struct mazeNode *south;
    struct mazeNode *west;
};


