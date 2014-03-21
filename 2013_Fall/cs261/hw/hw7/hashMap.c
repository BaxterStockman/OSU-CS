#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"

#if HASHING_FUNCTION == 1
#define _compare(arg1, arg2) strcmp((arg1), (arg2))
#endif

#if HASHING_FUNCTION == 2
#define _compare(arg1, arg2) strcmp((arg1), (arg2))
#endif

#if HASHING_FUNCTION == 3
#define hashFunc djb2
#define _compare(arg1, arg2) strcmp((arg1), (arg2))
#endif

/*the first hashing function you can use*/
int stringHash1(char * str) {
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += str[i];
    return r;
}


/*the second hashing function you can use*/
int stringHash2(char * str) {
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
    return r;
}

/* An old favorite: Dan Bernstein's hashing function from comp.lang.c */
int djb2(char *str) {
    int hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c; /*  == hash * 33 + c */

    return hash;
}

/* Returns the index corresponding to the key value k */
static inline int _getHash(struct hashMap *ht, KeyType k, int (*f)(KeyType)) {
    int i = f(k) % ht->tableSize;
    if(i < 0)
        i += ht->tableSize;
    return i;
}

/* Returns the first link in the bucket corresponding to key value k. */
static inline struct hashLink * _getLink(struct hashMap *ht, KeyType k) {
    int i = _getHash(ht, k, hashFunc);
    if(i >= 0 && i < ht->tableSize)
        return ht->table[i];
    return NULL;
}

/* Returns the first link in the bucket where hl is located. */
static inline struct hashLink * _firstLink(struct hashMap *ht, struct hashLink *hl) {
    return _getLink(ht, hl->key);
}

/* Returns the next link in the bucket where hl is located. */
static inline struct hashLink * _nextLink(struct hashLink *hl) {
    if(hl != NULL)
        return hl->next;
    return NULL;
}

/* Returns the previous link in the bucket where hl is located. */
static inline struct hashLink * _prevLink(struct hashMap *ht, struct hashLink *hl) {
    struct hashLink *prev = _firstLink(ht, hl);
    if(hl != NULL) {
        if(hl->next != NULL && hl != prev)
            while(prev->next != hl)
                prev = _nextLink(hl);
        return prev;
    }
    return NULL;
}

/* Returns the last link in the bucket where link hl is located. */
static inline struct hashLink * _lastLink(struct hashLink *hl) {
    if(hl != NULL) {
        if(hl->next != NULL)
            _lastLink(hl->next);
        else
            return hl;
    }
    return NULL;
}

/* Applies function 'f' to link hl and any links that come after hl in the bucket. */
void _iterLinks(struct hashLink *hl, void * (*f)(struct hashLink *, void *), void *state) {
    /* Starting at hl, call 'f', then recurse on hl->next. */
    if(hl) {
        f(hl, state);
        _iterLinks(hl->next, f, state);
    }
}

/*  Returns the first link in the table. */
struct hashLink * _getFront(struct hashMap *ht) {
    /* If there are no links in the table, return NULL */
    if(ht->count == 0)
        return NULL;

    /* Loop through the table, starting at the index of the first element.
     * If the element at ht->table[i] is not NULL, return it. */
    int i;
    for(i = 0; i < ht->tableSize; i++)
        if(ht->table[i] != NULL)
            return ht->table[i];

    /* If no elements were found, return NULL.  The function should never
     * reach this point, however, since if there were no elements it would
     * have exited at the ht->count check above. */
    return NULL;
}

/* Returns the last link in the table. */
struct hashLink * _getBack(struct hashMap *ht) {
    /* If there are no links in the table, return NULL */
    if(ht->count == 0)
        return NULL;

    /* Loop through the table, starting at the index of the last element.
     * If the element at ht->table[i] is not NULL, return the last link
     * in the bucket. */
    int i;
    for(i = ht->tableSize; i >= 0; i--)
        if(ht->table[i] != NULL)
            return _lastLink(ht->table[i]);

    /* If no elements were found, return NULL.  The function should never
     * reach this point, however, since if there were no elements it would
     * have exited at the ht->count check above. */
    return NULL;
}

/* Returns the next link in the table. */
struct hashLink * _getNext(struct hashMap *ht, struct hashLink *hl) {
    /* Make sure we don't try to dereference a null pointer. */
    assert(ht);

    /* No sense in going any further if there are no other links */
    if(ht->count == 0)
        return NULL;

    int i;
    if(hl != NULL) {
        if(hl->next != NULL)
            return hl->next;
        /* Hash the value of curr to make sure that we don't try to do
         * pointer arithmetic on a link that's not in the array */
        i = _getHash(ht, hl->key, hashFunc);
        /* Starting from the next index higher than the hashed index,
         * loop through the array until either we reach the end or encounter
         * a non-null position. */
        for(i++; i < ht->tableSize; i++)
            if(ht->table[i] != NULL)
                break;
    }

    /* If we've been passed a null pointer, but one that's in the range of
     * the pointers allocated to ht->table, then increment until we get a
     * non-null pointer or we go out-of-range. */
    else if(hl >= ht->table[0])
        while(hl++ && hl <= ht->table[ht->tableSize-1])
            if(hl != NULL)
                i = _getHash(ht, hl->key, hashFunc);

    /* If we've gotten past the last index in ht->table, return NULL */
    if(hl == NULL || i >= ht->tableSize)
        return NULL;

    /* Return the value of ht->table at the first non-null index */
    return ht->table[i];
}

/* Applies function 'f' to each link in the table, starting at the first link. */
void _iterOver(struct hashMap *ht, void * (*f)(struct hashLink *, void *), void *state) {
    /* Because some operations passed as function 'f' may alter the link
     * passed to them, we maintain the link 'this' to pass to 'f', and the
     * link 'curr' to iterate over. This avoids orphaning this->next. */
    struct hashLink *this = _getFront(ht);
    struct hashLink *curr = _getNext(ht, this);

    /* 'this' should never be null, unless there are no links in the table or
     * we've already processed the last non-null link in the table. */
    while(this != NULL) {
        f(this, state);
        /* Bail out if we've reached the last link in the table. */
        if(this == _lastLink(ht->table[ht->tableSize-1]))
            break;
        /* Go to next link. */
        this = curr;
        /* As long as curr is not a null pointer, assign the next
         * link to curr. */
        if(curr != NULL)
            /* This will return NULL once we've processed the last link
             * in the table. */
            curr = _getNext(ht, curr);
    }
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize) {
    /* Avoid trying to dereference a null pointer. */
    if(ht == NULL)
        return;

    ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
    /* Make sure memory was allocated properly. */
    assert(ht->table != NULL);
    ht->tableSize = tableSize;
    ht->count = 0;

    /* Iterate through table elements, assigning NULL to each. */
    int i;
    for(i = 0; i < tableSize; i++)
        ht->table[i] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
    /* Don't try to create an empty map. */
    assert(tableSize > 0);

    hashMap *ht;
    ht = malloc(sizeof(hashMap));
    /* Make sure memory was allocated properly. */
    assert(ht != NULL);
    _initMap(ht, tableSize);
    return ht;
}

void * _freeLink(struct hashLink *hl, void *state) {
    assert(hl != NULL);

    /* If this function was called properly, 'state' should
     * refer to the map in which hl is located. */
    struct hashMap *ht = (struct hashMap *)state;
    assert(ht != NULL);
    assert(ht->tableSize != 0);

    /* Make sure we don't try to free a null pointer. */
    int i = _getHash(ht, hl->key, hashFunc);
    struct hashLink *prev = _prevLink(ht, hl);

    /* Close the gap between the previous and next nodes. */
    if(prev != NULL)
        prev->next = hl->next;

    /* Make sure that the first of the leftover nodes gets
     * assigned to a position in ht->table. */
    if(hl == ht->table[i])
        ht->table[i] = hl->next;

    free(hl->key);
    hl->key = NULL;
    free(hl);
    hl = NULL;

    /* Decrement ht's count of links to reflect freed node. */
    ht->count--;
    return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void freeMap (struct hashMap * ht) {
    /* Iterate over the links in ht, freeing each one sequentially. */
    _iterOver(ht, _freeLink, ht);
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
    /* Make sure we don't try to free a null pointer. */
    assert(ht != NULL);

    /* Free all memory used by the buckets */
    freeMap(ht);
    free(ht->table);

    /* free the hashMap struct */
    free(ht);
    ht = NULL;
}

/* Essentially a wrapper for insertMap() that can be used in _iterLinks().
 * Useful for rehashing an entire table when it is resized.  */
void * _rehash(struct hashLink *hl, void *table) {
    struct hashMap *ht = (struct hashMap*)table;
    insertMap(ht, hl->key, hl->value);
    return (void*)ht;
}

/* Resizes the hash table to be the size newTableSize */
struct hashMap * _setTableSize(struct hashMap * ht, int newTableSize) {
    /* Check that new table size is large enough to accommodate
     * all of the elements in the current table */
    assert(newTableSize >= ht->count);

    /* Create a temporary hashMap */
    struct hashMap *tempTable = malloc(sizeof(struct hashMap));

    /* Assign the members of ht to tempTable */
    tempTable->table = ht->table;
    tempTable->tableSize = ht->tableSize;
    tempTable->count = ht->count;

    /* Reinitialize ht with a table of the new size. */
    _initMap(ht, newTableSize);

    /* Go through each of the elements in the old table,
     * rehashing them based on the size of the new table
     * and inserting them in the new table. */
    _iterOver(tempTable, _rehash, ht);

    /* Get rid of temporary table */
    deleteMap(tempTable);
    return ht;
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".

 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.

 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v) {
    /* First step is to increment the node count, just in case
     * the table needs to be resized. */
    ht->count++;

    /* Resize the table if the load factor is at or exceeds
     * the defined acceptable threshold */
    if(tableLoad(ht) >= (float)LOAD_FACTOR_THRESHOLD)
        _setTableSize(ht, ht->tableSize*2);

    int i = _getHash(ht, k, hashFunc);
    struct hashLink *curr = ht->table[i];
    struct hashLink *prev = NULL;

    /* Iterate over the links in a given bucket until an empty
     * position is found. */
    while(curr != NULL) {
        /* If the key already exists, change the value for that key
         * to v and return. */
        if(_compare(curr->key, k) == 0) {
            curr->value = v;
            return;
        }
        /* Set prev to curr so that we'll be able to set prev->next
         * to curr if curr is not the first node in the bucket. */
        prev = curr;
        curr = _nextLink(curr);
    }

    curr = malloc(sizeof(struct hashLink));
    /* Make sure memory allocation worked. */
    assert(curr != NULL);

    /* Initialize member variables of new link. */
    size_t keyMemBuf = (strlen(k) + 1) * sizeof(char);
    curr->key = malloc(keyMemBuf);
    memcpy(curr->key, k, keyMemBuf);
    curr->value = v;
    curr->next = NULL;

    /* if curr is not the first link in the bucket, set the
     * previous link to point to it. */
    if(prev != NULL)
        prev->next = curr;
    /* Otherwise, set curr to be the first element in the bucket. */
    else
        ht->table[i] = curr;
}

/* Returns a pointer to the first link in the bucket having the key 'k'. */
struct hashLink * _hasKey(struct hashMap *ht, KeyType k) {
    /* Get the first link in the bucket corresponding to key 'k'. */
    struct hashLink *curr = _getLink(ht, k);

    /* Iterate over the links in the bucket, breaking either when curr
     * is a null pointer, or curr->key matches 'k'. */
    while(curr != NULL) {
        if(_compare(curr->key, k) == 0)
            return curr;
        curr = _nextLink(curr);
    }

    /* The return value is NULL if no link has key 'k'. */
    return NULL;
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.

 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.

 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k) {
    /* Get the link, if any, having key 'k'. */
    struct hashLink *ret = _hasKey(ht, k);
    if(ret != NULL)
        return &ret->value;
    return NULL;
}

/*
 a simple yes/no if the key is in the hashtable.
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k) {
    /* Get the link, if any, having key 'k'. */
    struct hashLink *ret = _hasKey(ht, k);
    if(ret != NULL)
        return 1;
    return 0;
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k) {
    struct hashLink *curr = _getLink(ht, k);
    if(curr != NULL)
        _freeLink(curr, ht);
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht) {
    return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht) {
    return ht->tableSize;
}

void * _countBuckets(struct hashLink *hl, void *state) {
    if(hl != NULL && hl->next == NULL)
        (*(int*)state)++;
    return state;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht) {
    int filled = 0;
    _iterOver(ht, _countBuckets, &filled);
    return ht->tableSize - filled;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)

 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht) {
    return (double)ht->count/ht->tableSize;
}

void * _printLink(struct hashLink *hl, void *state) {
    *(int*)state = printf("%s: %d\n", hl->key, hl->value);
    return state;
}

void printMap(struct hashMap *ht) {
    int printedChars;
    _iterOver(ht, _printLink, &printedChars);
}


/* print the hashMap */
//void printMap (struct hashMap * ht) {
//    int i;
//    struct hashLink *temp;
//    for(i = 0;i < capacity(ht); i++){
//        temp = ht->table[i];
//        if(temp != 0) {
//            printf("\nBucket Index %d -> ", i);
//        }
//        while(temp != 0){
//            printf("Key:%s|", temp->key);
//            printValue(temp->value);
//            printf(" -> ");
//            temp=temp->next;
//        }
//    }
//}

void printValue(ValueType v) {
    printf("%d", (int)v);
}
