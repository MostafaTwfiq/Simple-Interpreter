#ifndef SIMPLEINTERPRETER_AVLTREE_H
#define SIMPLEINTERPRETER_AVLTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/VariablesVector.h"
#include "ANSI_Colors.h"

typedef struct NodeValue {

    float data;
    char type[6];

} NodeValue;

typedef struct AVLNode{
    int height;
    char *key;
    NodeValue value;
    struct AVLNode *right;
    struct AVLNode *left;
} AVLNode;

typedef struct AVLTree{
    AVLNode *root;
} AVLTree;

typedef struct HeapValue{
    char *key;
    NodeValue nodeValue;
} HeapValue;

AVLNode *insert(AVLNode *root, char *key, NodeValue value);
AVLNode *BSRemoveNode(AVLNode *root, char *key);
AVLNode *createNode(char *key, NodeValue value);
AVLTree *avlTreeInitialization();
void inOrderTraversal(AVLNode *root);
int BSFindByValue(AVLNode *root, NodeValue value);
int BSFindByVariableName(AVLNode *root, char *key);
NodeValue BSGetNodeValue(AVLNode *root, char *key);
void getNodesAtDepth(int depth, AVLNode *root);
int depth(AVLNode *root);
void BreadthFirst(AVLNode *root);
int max(int a, int b);
AVLNode *rotation(AVLNode *node);
int getHeight(AVLNode *node);
int isUnbalanced(AVLNode *node);
AVLNode *rotation(AVLNode *node);
AVLNode *leftRotation(AVLNode *node);
AVLNode *rightRotation(AVLNode *node);
AVLNode *reCalculateHeight(AVLNode *node);
void preOrderTraversal(AVLNode *root);
void postOrderTraversal(AVLNode *root);
void avlTreeDestroy(AVLTree *tree);
void avlDestroyNodes(AVLNode *root);

VariablesVector *avlTreeToVariablesVector(AVLTree *tree);
void avlTreeCopyNodesToVariablesVector(AVLNode *root, VariablesVector *vector);

AVLTree *copyAVLTreeIntoNewTree(AVLTree *tree);
void copyAVLTreeNodesToAnotherTree(AVLNode *root, AVLTree *tree);

#endif //SIMPLEINTERPRETER_AVLTREE_H
