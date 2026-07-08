#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include "compiler.h"

// Hash
void initHashTable(void);
void insertHash(const char *name, const char *type);
void printHashTable(void);

// BST
void initBST(void);
BSTNode *insertBST(BSTNode *root, char *name, char *type);
void printBST(BSTNode *root);

// AVL
void initAVL(void);
AVLNode *insertAVL(AVLNode *node, char *name, char *type);
void printAVL(AVLNode *root);

#endif
