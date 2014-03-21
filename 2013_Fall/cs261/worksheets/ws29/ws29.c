#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define TYPE double

struct Node {
    TYPE value;
    struct Node *left;
    struct Node *right;
};

struct BinarySearchTree {
    struct Node *root;
    int size;
};

void initBST(struct BinarySearchTree *tree, TYPE newValue) {
    tree->size = 0;
    tree->root = NULL;
}

struct Node * _nodeAddBST(struct Node *current, TYPE newValue) {
    // if current is NULL...
    if(!current) {
        // allocate memory for a new Node
        current = malloc(sizeof(struct Node));
        // set node's value
        current->value = newValue;
        // set left and right leaves to NULL
        current->left = NULL;
        current->right = NULL;
    }
    else if (newValue < current->value)
        current->left = _nodeAddBST(current->left, newValue);
    else
        current->right = _nodeAddBST(current->right, newValue);
    return current;
}

void addBST(struct BinarySearchTree *tree, TYPE newValue) {
    // If the tree has no 'active' nodes, this will create one.
    // Otherwise, _nodeAddBST will recurse over the tree until it
    // locates the proper insertion point for the node containing
    // newValue
    tree->root = _nodeAddBST(tree->root, newValue);
    tree->size++;
}

int sizeBST(struct BinarySearchTree *tree) {
    return tree->size;
}

// This could have been done recursively if it took a Node
// as an argument rather than a tree.  As it is, we can still
// do this recursive by declaring a new tree each call, but that would
// be ugly and probably costly too.  So instead the function
// accomplishes its purpose using iteration.
int containsBST(struct BinarySearchTree *tree, TYPE d) {
    // starting at the root...
    struct Node *current = tree->root;
    // while we're not dealing with a NULL pointer
    while(current) {
        // return 1 if we've found d
        if(current->value == d) return 1;
        // head left if d is less than current node's value
        if(d < current->value) {
            current = current->left;
            continue;
        }
        // otherwise head to the right leaf
        else current = current->right;
    }
    return 0;
}

TYPE _leftMostChild(struct Node *current) {
    // make sure we've been passed a valid argument
    assert(current);
    // while the left leaf is not a NULL pointer...
    if(!current->left) return current->value;
        // keep heading down the left leaf
    else _leftMostChild(current->left);
}

struct Node * _removeLeftMostChild(struct Node *current) {
    if(!current->left) {
        struct Node *delete = current;
        current = current->right;
        free(delete);
    }
    else current->left = _removeLeftMostChild(current->left);
    return current;
}

// N.B. Without passing a BinarySearchTree as a parameter
// there's no way to decrease the size of the tree as directed
// in the worksheet.  However, this is dealt with by decreasing
// the size in the function removeBST.
struct Node * _nodeRemoveBST(struct Node *current, TYPE d) {
    // make sure we've been passed a valid argument
    assert(current);
    // if we've found d...
    if(current->value == d) {
        // if there's no right leaf...
        if(!current->right) {
            // if there's no left leaf...
            if(!current->left) {
                // delete current because there's nothing after it
                current = NULL;
                return current;
            }
            // otherwise, delete current and substitute current->left
            struct Node * temp = current->left;
            free(current);
            current = NULL;
            return temp;
        }
        // get the value of the leftmost child of the right leaf of current
        // and set it as the value of current, then delete the leftmost child
        // of the right leaf.
        else {
            current->value = _leftMostChild(current->right);
            current->right = _removeLeftMostChild(current->right);
        }
    }
    // if d is less than the value of current, recurse on current's left leaf.
    else if(d < current->value) current->left = _nodeRemoveBST(current->left, d);
    // otherwise, recurse on current's left leaf.
    else current->right = _nodeRemoveBST(current->right, d);
    return current;
}

void removeBST(struct BinarySearchTree *tree, TYPE d) {
    if(containsBST(tree, d)) {
        tree->root = _nodeRemoveBST(tree->root, d);
        tree->size--;
    }
}


