#ifndef _ANIMAL_H
#define _ANIMAL_H

typedef enum { FIND, ENTER } ACTION;
typedef enum { preorder, postorder, endorder, leaf } VISIT;

struct tree;
/* Declared in the c source file to hide the structure members from the user. */

/* -- BST management functions -- */
void * tree_search(const void *key, void **rootp, int (*compar)(const void *, const void *));
void * tree_find(const void *key, void **rootp, int (*compar)(const void *, const void *));
void * tree_delete(const void *key, void **rootp, int (*compar)(const void *, const void *));
void tree_walk(const void *root, void (*action)(const void *nodep, const VISIT which, const int depth));
void tree_destroy(void * root, void (*free_node)(void *nodep));

#endif
