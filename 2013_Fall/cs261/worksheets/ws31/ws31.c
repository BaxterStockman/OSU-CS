#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ws31.h"

#ifndef TYPE
#define TYPE void*
#endif

struct AVLnode {
    TYPE value;
    struct AVLnode *left;
    struct AVLnode *right;
    int height;
};

int _h(struct AVLnode *current) {
    if (!current) return -1;
    return current->height;
}

void _setHeight(struct AVLnode *current) {
    int lch = _h(current->left);
    int rch = _h(current->right);
    if(lch < rch) current->height = 1 + rch;
    else current->height = 1 + lch;
}

struct AVLnode * _AVLNodeAdd(struct AVLnode *current, TYPE newValue) {
    struct AVLnode * new_node;
    if(!current) {
        new_node = (struct AVLnode *) malloc(sizeof(struct AVLnode));
        assert(new_node);
        new_node->value = newValue;
        new_node->left = new_node->right = 0;
        return new_node;
    }
    else if (LT(newValue, current->value))
        current->left = _AVLNodeAdd(current->left, newValue);
    else
        current->right = _AVLNodeAdd(current->right, newValue);
    return _balance(current);
}

int _bf(struct AVLnode *current) {
    return _h(current->right) - _h(current->left);
}

struct AVLnode * _balance(struct AVLnode *current) {
    int cbf = _bf(current);
    if(cbf < -1) {
        if(_bf(current->left) > 0)
            current->left = _rotateLeft(current->left);
        return _rotateRight(current);
    }
    else if(cbf > 1) {
        if(_bf(current->right) < 0)
            current->right = _rotateRight(current->right);
        return _rotateLeft(current);
    }
    _setHeight(current);
    return current;
}

struct AVLnode * _rotateLeft(struct AVLnode *current) {
    struct AVLnode * new_left = current;
    current = current->right;
    new_left->right = current->left;
    current->left = new_left;
   _setHeight(new_left);
   _setHeight(current);
    return current;
}

struct AVLnode * _rotateRight(struct AVLnode *current) {
    struct AVLnode * new_right = current;
    current = current->left;
    new_right->left = current->right;
    current->right = new_right;
   _setHeight(new_right);
   _setHeight(current);
    return current;
}

void removeAVLtree(struct AVLtree * tree, TYPE val) {
    if(containsAVLtree(tree, val)) {
        tree->root = _removeNode(tree->root, val);
        tree->cnt--;
    }
}

TYPE _leftMost(struct AVLnode * cur) {
    while(cur->left)
        cur = cur->left;
    return cur->val;
}

struct AVLnode * _removeLeftMost(struct AVLnode *current) {
    struct AVLnode * temp;
    if(current->left) {
        cur->left = _removeLeftMost(cur->left);
        return _balance(current);
    }
    temp = current->right;
    free(current);
    return temp;
}

struct AVLnode * _removeNode(struct AVLnode *current, TYPE value) {
    // make sure we've been passed a valueid argument
    assert(current);
    //int comp = d_compare(cur->value, value);
    int comp = compare(current->value, value);
    // if we've found value...
    if(comp == 0) {
        if(!current->right) return current->left;
        else {
            current->value = _leftMost(current->right);
            current->right = _removeLeftMost(current->right);
        }
    }
    // if value is less than the value of current, recurse on current's left leaf.
    else if(comp == 1) current->left = _removeNode(current->left, value);
    // otherwise, recurse on current's left leaf.
    else if(comp == -1) current->right = _removeNode(current->right, value);
    _balance(current);
    return current;
}
