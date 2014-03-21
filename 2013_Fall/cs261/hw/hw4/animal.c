#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "animal.h"

struct Node {
    const void *key;
    struct Node *left;
    struct Node *right;
};

void * tree_search(const void *key, void **rootp, int (*compar)(const void *, const void *)) {
    struct Node **root = (struct Node **) rootp;

    if(root == NULL)
        return NULL;

    struct Node *cur = (struct Node *)(*root);

    if(cur == NULL) {
        // allocate memory for a new Node
        cur = (struct Node *) malloc(sizeof(struct Node));
        if(cur != NULL) {
            // set node's val
            cur->key = key;
            // set left and right leaves to NULL
            cur->left = NULL;
            cur->right = NULL;
            return cur;
        }
    }

    int comp = compar(key, cur->key);
    if(comp == 0) return cur;
    else if (compar(key, cur->key) <= 0)
        cur->left = tree_search(key, (void *)&cur->left, compar);
    else
        cur->right = tree_search(key, (void *)cur->left, compar);
}

void * tree_find(const void *key, void **rootp, int (*compar)(const void *, const void *)) {
    struct Node **root = (struct Node **) rootp;

    if(root == NULL)
        return NULL;

    struct Node *cur = (struct Node *)(*root);

    if(cur == NULL) return NULL;

    int comp = compar(key, cur->key);
    if(comp == 0) return cur;
    else if (compar(key, cur->key) <= 0)
        cur->left = tree_search(key, (void *)&cur->left, compar);
    else
        cur->right = tree_search(key, (void *)cur->left, compar);
}

/*
 helper function to find the left most child of a node
 return the value of the left most child of cur
 param: cur     the current node
 pre:   cur is not null
 post: none
 */

/*----------------------------------------------------------------------------*/
void * _leftMost(struct Node *cur) {
    // make sure we've been passed a valid argument
    assert(cur);
    // while the left leaf is not a NULL pointer...
    if(cur->left != NULL) return _leftMost(cur->left);
    return (void*)cur->key;
}


/*
 recursive helper function to remove the left most child of a node
 HINT: this function returns cur if its left child is NOT NULL. Otherwise,
 it returns the right child of cur and free cur.

Note:  If you do this iteratively, the above hint does not apply.

 param: cur the current node
 pre:   cur is not null
 post:  the left most node of cur is not in the tree
 */
/*----------------------------------------------------------------------------*/
struct Node * _removeLeftMost(struct Node *cur) {
    if(!cur->left) {
        struct Node *delete = cur;
        cur = cur->right;
        free(delete);
    }
    else cur->left = _removeLeftMost(cur->left);
    return cur;
}

/*
 recursive helper function to remove a node from the tree
 HINT: You have to use the d_compare() function to compare values.
 param: cur the current node
        key the value to be removed from the tree
 pre:   key is in the tree
        cur is not null
        key is not null
 */
/*----------------------------------------------------------------------------*/
void * tree_delete(const void *key, void **rootp, int (*compar)(const void *, const void *)) {
    struct Node **root = (struct Node **)rootp;

    if(root == NULL) return NULL;

    struct Node *cur = (struct Node *)(*root);
    // make sure we've been passed a keyid argument
    assert(cur);
    //int comp = d_compare(cur->key, key);
    int comp = compar(cur->key, key);
    // if we've found key...
    if(comp == 0) {
        if(!cur->right) return cur->left;
        else {
            cur->key = _leftMost(cur->right);
            cur->right = _removeLeftMost(cur->right);
        }
    }
    // if key is less than the key of cur, recurse on cur's left leaf.
    else if(comp == 1) cur->left = tree_delete(key, (void*)&cur->left, compar);
    // otherwise, recurse on cur's left leaf.
    else if(comp == -1) cur->left = tree_delete(key, (void*)&cur->right, compar);
    return cur;
}

void _tree_walk_recurse(const void *root, void (*action)(const void *nodep, const VISIT which, const int depth), int level) {
    const struct Node *cur = (const struct Node *)root;
    if(cur->left == NULL && cur->right == NULL)
        action(cur, leaf, level);
    else {
        action(cur, preorder, level);
        if(cur->left != NULL)
            _tree_walk_recurse(cur->left, action, level + 1);
        action(cur, postorder, level);
        if(cur->right != NULL)
            _tree_walk_recurse(cur->right, action, level + 1);
        action(cur, endorder, level);
    }
}

void tree_walk(const void *root, void (*action)(const void *nodep, const VISIT which, const int depth)) {
    const struct Node *cur = (const struct Node *)(root);

    if(cur != NULL && action != NULL)
        _tree_walk_recurse(cur, action, 0);
}

void _tree_destroy_recurse(struct Node *root, void (*free_node)(void *nodep)) {
    if(root->left != NULL)
        _tree_destroy_recurse(root->left, free_node);
    if(root->right != NULL)
        _tree_destroy_recurse(root->right, free_node);
    free_node((void *)root->key);
    free(root);
}

void tree_destroy(void * root, void (*free_node)(void *nodep)) {
    struct Node *cur = (struct Node *)root;
    if(cur != NULL)
        _tree_destroy_recurse(cur, free_node);
}
