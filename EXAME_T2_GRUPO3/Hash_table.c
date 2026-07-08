#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

HashNode *hashTable[HASH_SIZE];
BSTNode *bstRoot = NULL;
AVLNode *avlRoot = NULL;

void initHashTable() { for (int j = 0; j < HASH_SIZE; j++) hashTable[j] = NULL; }

unsigned int hashFunction(char *name)
{
    unsigned int hash = 0;
    while (*name) { hash = (hash * 31) + *name; name++; }
    return hash % HASH_SIZE;
}

void insertHash(const char *name, const char *type)
{
    unsigned int index = hashFunction((char*)name);
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    if (node) {
        strcpy(node->name, name);
        strcpy(node->type, type);
        node->next = hashTable[index];
        hashTable[index] = node;
    }
}

void printHashTable()
{
    printf("\n===== HASH TABLE =====\n");
    for (int j = 0; j < HASH_SIZE; j++) {
        HashNode *node = hashTable[j];
        while (node) { printf("%s -> %s\n", node->name, node->type); node = node->next; }
    }
}

void initBST() { bstRoot = NULL; }

BSTNode *createBSTNode(char *name, char *type)
{
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node) { strcpy(node->name, name); strcpy(node->type, type); node->left = NULL; node->right = NULL; }
    return node;
}

BSTNode *insertBST(BSTNode *root, char *name, char *type)
{
    if (root == NULL) return createBSTNode(name, type);
    if (strcmp(name, root->name) < 0) root->left = insertBST(root->left, name, type);
    else if (strcmp(name, root->name) > 0) root->right = insertBST(root->right, name, type);
    return root;
}

void printBST(BSTNode *root)
{
    if (root == NULL) return;
    printBST(root->left);
    printf("%s -> %s\n", root->name, root->type);
    printBST(root->right);
}

int heightAVL(AVLNode *node) { return (node == NULL) ? 0 : node->height; }
int max(int a, int b) { return (a > b) ? a : b; }

AVLNode *createAVLNode(char *name, char *type)
{
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (node) { strcpy(node->name, name); strcpy(node->type, type); node->left = node->right = NULL; node->height = 1; }
    return node;
}

AVLNode *rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *temp = x->right;
    x->right = y;
    y->left = temp;
    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));
    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));
    return x;
}

AVLNode *rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *temp = y->left;
    y->left = x;
    x->right = temp;
    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));
    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));
    return y;
}

int getBalance(AVLNode *N) { return (N == NULL) ? 0 : heightAVL(N->left) - heightAVL(N->right); }

AVLNode *insertAVL(AVLNode *node, char *name, char *type)
{
    if (node == NULL) return createAVLNode(name, type);
    if (strcmp(name, node->name) < 0) node->left = insertAVL(node->left, name, type);
    else if (strcmp(name, node->name) > 0) node->right = insertAVL(node->right, name, type);
    else return node;
    node->height = 1 + max(heightAVL(node->left), heightAVL(node->right));
    int balance = getBalance(node);
    if (balance > 1 && strcmp(name, node->left->name) < 0) return rotateRight(node);
    if (balance < -1 && strcmp(name, node->right->name) > 0) return rotateLeft(node);
    if (balance > 1 && strcmp(name, node->left->name) > 0) { node->left = rotateLeft(node->left); return rotateRight(node); }
    if (balance < -1 && strcmp(name, node->right->name) < 0) { node->right = rotateRight(node->right); return rotateLeft(node); }
    return node;
}

void printAVL(AVLNode *root)
{
    if (root == NULL) return;
    printAVL(root->left);
    printf("%s -> %s\n", root->name, root->type);
    printAVL(root->right);
}

void initAVL() { avlRoot = NULL; }
