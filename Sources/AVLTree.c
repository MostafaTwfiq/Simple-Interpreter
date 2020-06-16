#include "../Headers/AVLTree.h"

AVLTree *avlTreeInitialization(){
    AVLTree *tree = (AVLTree *) malloc(sizeof(AVLTree));
    tree->root = NULL;
    return tree;
}

AVLNode *BSRemoveNode(AVLNode *root, char *key) {
    if (root == NULL) {
        exit(-10);
    }

    if (!strcmp(root->key, key)) {
        if (root->right == NULL && root->left == NULL) {
            free(root->key);
            free(root);
            return NULL;
        }
        else if (root->right != NULL && root->left != NULL) {
            AVLNode *tempNode = root->right;
            AVLNode *tempNodeParent = root;
            while (tempNode->left != NULL) {
                tempNodeParent = tempNode;
                tempNode = tempNode->left;
            }
            strcpy(root->key, tempNode->key);
            root->value = tempNode->value;

            if (root->right == tempNode) {
                root->right = BSRemoveNode(tempNode, tempNode->key);
            } else
                tempNodeParent->left = BSRemoveNode(tempNode, tempNode->key);

        }
        else if (root->right != NULL) {
            AVLNode *nodeToFree = root;
            root = root->right;

            free(nodeToFree->key);
            free(nodeToFree);

        }
        else {
            AVLNode *nodeToFree = root;
            root = root->left;

            free(nodeToFree->key);
            free(nodeToFree);

        }
    }
    else if (strcmp(root->key, key) > 0) {
        root->left = BSRemoveNode(root->left, key);
    }

    else if (strcmp(root->key, key) < 0) {
        root->right = BSRemoveNode(root->right, key);
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    if (isUnbalanced(root))
        return rotation(root);

    return root;

}

int BSFindByValue(AVLNode *root, NodeValue value){
    if (root == NULL)
        return 0;

    if (root->value.data == value.data)
        return 1;

    else if (root->value.data > value.data)
        return BSFindByValue(root->left, value);

    else
        return BSFindByValue(root->right, value);

}

int BSFindByVariableName(AVLNode *root, char *key) {
    if (root == NULL)
        return 0;

    if (!strcmp(root->key, key))
        return 1;

    else if (strcmp(root->key, key) > 0)
        return BSFindByVariableName(root->left, key);

    else
        return BSFindByVariableName(root->right, key);

}

NodeValue BSGetNodeValue(AVLNode *root, char *key) {
    if (root == NULL) {
        NodeValue nodeValue = {0, "\0"};
        return nodeValue;
    }

    if (!strcmp(root->key, key))
        return root->value;

    else if (strcmp(root->key, key) > 0)
        return BSGetNodeValue(root->left, key);

    else
        return BSGetNodeValue(root->right, key);

}

AVLNode *createNode(char *key, NodeValue value) {
    AVLNode *node = (AVLNode *) malloc(sizeof(AVLNode));
    node->key = (char *) malloc(sizeof(char) * (strlen(key) + 1));
    node->key[0] = '\0';
    strcpy(node->key, key);
    node->value.data = value.data;
    strcpy(node->value.type, value.type);
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLNode *insert(AVLNode *root, char *key, NodeValue value){
    if (root == NULL)
        root = createNode(key, value);

    else if (!strcmp(root->key, key)) {
        root->value.data = value.data;
        strcpy(root->value.type, value.type);
    }

    else if (strcmp(root->key, key) < 0)
        root->right = insert(root->right, key, value);

    else if (strcmp(root->key, key) > 0)
        root->left = insert(root->left, key, value);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    if (isUnbalanced(root))
        return rotation(root);

    return root;
}

int getHeight(AVLNode *node){
    return (node == NULL) ? (-1) : (node->height);
}

int isUnbalanced(AVLNode *node){
    return ((getHeight(node->left) - getHeight(node->right) > 1) ||
            (getHeight(node->left) - getHeight(node->right) < -1));
}

AVLNode *rotation(AVLNode *node){
    if (getHeight(node->left) - getHeight(node->right) > 1){ //left heavy
        if (getHeight(node->left->right) > getHeight(node->left->left)){ //
            node->left = leftRotation(node->left);
            return rightRotation(node);
        }
        else return rightRotation(node);
    }

    else if (getHeight(node->left) - getHeight(node->right) < -1){ // right heavy
        if (getHeight(node->right->right) < getHeight(node->right->left)){
            node->right = rightRotation(node->right);
            return leftRotation(node);
        }
        else return leftRotation(node);
    }
    return node;
}

AVLNode *leftRotation(AVLNode *node){
    AVLNode *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left= node;
    newRoot = reCalculateHeight(newRoot);
    return newRoot;
}

AVLNode *rightRotation(AVLNode *node){
    AVLNode *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    newRoot = reCalculateHeight(newRoot);
    return newRoot;
}

AVLNode *reCalculateHeight(AVLNode *node){
    if (node == NULL) return node;
    node->left = reCalculateHeight(node->left);
    node->right = reCalculateHeight(node->right);
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    if (isUnbalanced(node)){
        return rotation(node);
    }
    return node;
}

void getNodesAtDepth(int depth, AVLNode *root){
    if (root == NULL) return;
    else if (depth == 0) printf("%s ", root->key);
    getNodesAtDepth(depth - 1, root->left);
    getNodesAtDepth(depth - 1, root->right);
}

int depth(AVLNode *root){
    if (root == NULL)
        return 0;

    else if (root->left == NULL && root->right == NULL)
        return 0;

    return 1 + max(depth(root->left), depth(root->right));
}

int max(int a, int b){
    return (a > b) ? (a) : (b);
}

void preOrderTraversal(AVLNode *root){
    if (root == NULL) return;
    if (!strcmp(root->value.type, "int"))
        printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %d  Type: %s\n" ANSI_COLOR_RESET, root->key, (int) root->value.data, root->value.type);
    else
    printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %f  Type: %s\n" ANSI_COLOR_RESET, root->key, root->value.data, root->value.type);

    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}

void inOrderTraversal(AVLNode *root){
    if (root == NULL) return;
    inOrderTraversal(root->left);
    if (!strcmp(root->value.type, "int"))
        printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %d  Type: %s\n" ANSI_COLOR_RESET, root->key, (int) root->value.data, root->value.type);
    else
    printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %f  Type: %s\n" ANSI_COLOR_RESET, root->key, root->value.data, root->value.type);

    inOrderTraversal(root->right);
}

void postOrderTraversal(AVLNode *root){
    if (root == NULL) return;
    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    if (!strcmp(root->value.type, "int"))
        printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %d  Type: %s\n" ANSI_COLOR_RESET, root->key, (int) root->value.data, root->value.type);
    else
    printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %f  Type: %s\n" ANSI_COLOR_RESET, root->key, root->value.data, root->value.type);
}

void BreadthFirst(AVLNode *root){
    for (int i = 0 ; i <= depth(root); i++){
        getNodesAtDepth(i, root);
        printf("\n");
    }
}

void avlTreeDestroy(AVLTree *tree) {
    avlDestroyNodes(tree->root);
    free(tree);
}

void avlDestroyNodes(AVLNode *root) {
    if (root == NULL)
        return;

    avlDestroyNodes(root->left);
    avlDestroyNodes(root->right);
    free(root->key);
    free(root);
}

VariablesVector *avlTreeToVariablesVector(AVLTree *tree) {
    VariablesVector *vector = variablesVectorInitialization(10);
    avlTreeCopyNodesToVariablesVector(tree->root, vector);

    return vector;
}

void avlTreeCopyNodesToVariablesVector(AVLNode *root, VariablesVector *vector) {
    if (root == NULL) return;
    avlTreeCopyNodesToVariablesVector(root->left, vector);
    avlTreeCopyNodesToVariablesVector(root->right, vector);

    VariableBox box;
    box.key = root->key;
    strcpy(box.type, root->value.type);
    box.value = root->value.data;
    variablesVectorInsert(vector, box);
}

AVLTree *copyAVLTreeIntoNewTree(AVLTree *tree) {
    AVLTree *newTree = avlTreeInitialization();

    copyAVLTreeNodesToAnotherTree(tree->root, newTree);

    return newTree;

}

void copyAVLTreeNodesToAnotherTree(AVLNode *root, AVLTree *tree) {
    if (root == NULL)
        return;

    copyAVLTreeNodesToAnotherTree(root->left, tree);
    copyAVLTreeNodesToAnotherTree(root->right, tree);
    tree->root = insert(tree->root, root->key, root->value);
}