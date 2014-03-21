#ifndef _WS_31_H
#define _WS_31_H

#ifndef TYPE
#define TYPE void*
#endif

struct AVLnode;
struct AVLtree;

struct AVLnode;
int _h(struct AVLnode * current);
void _setHeight(struct AVLnode * current);
struct AVLnode * _AVLNodeAdd(struct AVLnode * current, TYPE newValue);
int _bf(struct AVLnode * current);
struct AVLnode * _balance(struct AVLnode * current);
struct AVLnode * _rotateLeft(struct AVLnode * current);
struct AVLnode * _rotateRight(struct AVLnode * current);
void removeAVLTree(struct AVLTree * tree, TYPE val);
TYPE _leftMost(struct AVLnode * cur);
struct AVLnode * _removeLeftMost(struct AVLnode *current);
struct AVLnode * _removeNode(struct AVLnode *current, TYPE val);

