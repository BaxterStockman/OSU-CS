#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TYPE int

/* Placeholder for an actual hash function.
 * Casts vptr (a void pointer) to a pointer to
 * an int, then dereferences it. */
#define HASH(vptr) (*(int*)(vptr))

/* Macro which enables calling openHashTableAdd()
 * without having to explicitly create a variable
 * each time. */
#define tableAdd(tbl, val)         \
do {                               \
    TYPE temp = val;               \
    openHashTableAdd(tbl, &temp);  \
} while(0)                         \

struct openHashTable {
    void **table;
    int tablesize;
    int count;
};

static inline int _getHash(struct openHashTable *ht, void *value);
void initOpenHashTable(struct openHashTable *ht, int size);
int openHashTableSize(struct openHashTable *ht);
void openHashTableAdd(struct openHashTable *ht, void *newValue);
int openHashTableBagContains(struct openHashTable *ht, void *newValue, int (*compar)(const void *, const void *));
void _resizeOpenHashTable(struct openHashTable *ht);
void freeOpenHashTable(struct openHashTable *ht);
void printOpenHashTable(struct openHashTable *ht, char* delim, int (*print_t)(const char *, ...),
                        void (*u_print)(const void *, int (*u_print_t)(const char *, ...)));
void my_print(const void *value, int (*print_t)(const char *, ...));

/* Returns a hash value for *value, modded by ht->tablesize to make
 * sure that it is a valid index for ht->table. */
static inline int _getHash(struct openHashTable *ht, void *value) {
    int idx = HASH(value) % ht->tablesize;
    if(idx < 0)
        idx += ht->tablesize;
    return idx;
}

void initOpenHashTable(struct openHashTable *ht, int size) {
    int i;
    assert(size > 0);
    ht->table = malloc(size * sizeof(void*));
    assert(ht->table != NULL);
    for(i = 0; i < size; i++)
        ht->table[i] = NULL;
    ht->tablesize = size;
    ht->count = 0;
}

int openHashTableSize(struct openHashTable *ht) { return ht->count; }

void openHashTableAdd(struct openHashTable *ht, void *newValue) {
    if((ht->count / (double)ht->tablesize) > 0.75)
        _resizeOpenHashTable(ht);
    ht->count++;

    int idx = _getHash(ht, newValue);

    /* Find the first free index in the table at or after
     * hashed key, wrapping around to index 0 if necessary.
     * N.B. This will cause an infinite loop if,
     * for some reason, all positions are already full.
     * However, this condition should never happen. */
    while(ht->table[idx] != NULL) {
        idx++;
        if(idx >= ht->tablesize)
            idx = 0;
    }

    /* Allocate space for new value, then copy over new value. */
    //ht->table[idx] = newValue;
    ht->table[idx] = malloc(sizeof(void*));
    memcpy(ht->table[idx], newValue, sizeof(void*));
}

int openHashTableBagContains(struct openHashTable *ht, void *newValue, int (*compar)(const void *, const void *)) {
    int idx = _getHash(ht, newValue);

    /* If the while-loop below reaches index 'stop' and doesn't
     * match the value pointed to by 'newValue', then the hash table
     * doesn't contain that value. */
    int stop = idx - 1;

    while(idx != stop) {
        if(ht->table[idx] == NULL)
            return 0;
        if(idx >= ht->tablesize) {
            idx = 0;
            continue;
        }
        if(compar(ht->table[idx], newValue) == 0)
            return 1;
        idx++;
    }
    return 0;
}

void _resizeOpenHashTable(struct openHashTable *ht) {
    void **tempTable = ht->table;
    int tempSize = ht->tablesize;

    initOpenHashTable(ht, ht->tablesize * 2);

    int i;
    for(i = 0; i < tempSize; i++)
        if(tempTable[i] != NULL)
            openHashTableAdd(ht, tempTable[i]);

    for(i = 0; i < tempSize; i++)
        free(tempTable[i]);
    free(tempTable);
}

void freeOpenHashTable(struct openHashTable *ht) {
    int i;
    for(i = 0; i < ht->tablesize; i++)
        if(ht->table[i] != NULL)
            free(ht->table[i]);
    free(ht->table);
}

void printOpenHashTable(struct openHashTable *ht, char* delim, int (*print_t)(const char *, ...),
                        void (*u_print)(const void *, int (*u_print_t)(const char *, ...))) {
    int i = 0;
    int j = 1;
    while(ht->table[i] == NULL)
        i++;
    u_print(ht->table[i++], print_t);
    for( ; i < ht->tablesize && j <= ht->count; i++) {
        if(ht->table[i] == NULL)
            continue;
        print_t(delim);
        u_print(ht->table[i], print_t);
        j++;
    }
}

int my_compar(const void *left, const void *right) {
    int l_int = *(int*)left;
    int r_int = *(int*)right;

    if(l_int == r_int) return 0;
    return l_int > r_int ? 1 : -1;
}

void my_print(const void *value, int (*print_t)(const char *, ...)) {
    int value_int = *(int*)value;
    print_t("%d", value_int);
}

int main(void) {
    struct openHashTable *ht = malloc(sizeof(struct openHashTable));
    initOpenHashTable(ht, 1);

    int i;
    for(i = 0; i < 1000; i++)
        tableAdd(ht, i);

    printOpenHashTable(ht, " * ", printf, my_print);
    putchar('\n');

    printf("The size of the hash table is %g.\n", (double)openHashTableSize(ht));

    int three = 3;
    int oneohone = 1001;

    if(openHashTableBagContains(ht, &three, my_compar))
        printf("Table contains 3.\n");
    if(openHashTableBagContains(ht, &oneohone, my_compar))
        printf("Table contains 101.\n");

    freeOpenHashTable(ht);
    free(ht);

    return 0;
}
