#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

//initialising a new node
Node* createNode(int value) {

    Node* newNode = (Node*)malloc(sizeof(struct Node));
    newNode -> data = value;
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;

}

Node* insertNode(Node *root, int value) {

    //if the tree is empty assign the new node the int data and return it
    if(root == NULL) {

        //allocating memory for a new node
        Node* newNode = createNode(value);
        return newNode;

    }

    //recur down the tree until a node is found which has empty left or right pointers
    if(root -> data > value) {

        if(root -> left == NULL) {

            Node* newNode = createNode(value);
            root -> left = newNode;
            return newNode;

        } else {

            return(insertNode(root -> left, value));

        }

    }else if(root -> data < value) {

        if(root -> right == NULL) {

            Node* newNode = createNode(value);
            root -> right = newNode;
            return newNode;

        } else {

            return(insertNode(root -> right, value));

        }

    }

    //if the data already exists return null
    return NULL;

}

//recurs throughout the tree to check if a specificed data is present
int isPresent(Node *root, int value) {

    if(root == NULL) {

        return 0;

    }

    if(root -> data == value) {

        return 1;

    } else {

        return isPresent(root -> right, value) + isPresent(root -> left, value);

    }

}

Node* deleteNode(Node *root, int value) {

    if(root == NULL) {

        return NULL;

    }

    //recur down the tree until the data is found making sure to check there aren't any null pointers
    if(root -> data > value) {

        root -> left = deleteNode(root -> left, value);

    } else if(root -> data < value) {

        root -> right = deleteNode(root -> right, value);

    } else {

        //if it has none or one, just set the parent (of the node to delete) pointer to the child (of node to delete)
        if(root -> right == NULL) {

            Node* newNode = root -> left;
            free(root);
            return newNode;

        } else if(root -> left == NULL) {

            Node* newNode = root -> right;
            free(root);
            return newNode;

        }

        //if it has two, replace the node to delete with the smallest data on its right subtree
        Node* newNode = root -> right;

        do {

            if(newNode -> left != NULL) {

                newNode = newNode -> left;

            }

        } while(newNode -> left != NULL);

        root -> data = newNode -> data;
        root -> right = deleteNode(root -> right, root -> data);

    }

    return root;

}

//recur through the tree starting on the left and print the values in the left, root, right order
void printSubtree(Node *N) {

    if(N -> left != NULL) {

        printSubtree(N -> left);

    }

    printf("%d\n", N -> data);

    if(N -> right != NULL) {

        printSubtree(N -> right);

    }

}

//recur through the whole tree, add 1 if it is a leaf, add 0 if it isn't
int countLeaves(Node *N) {

    if(N == NULL) {

        return 0;

    }
    if(N -> right == NULL && N -> left == NULL) {

        return 1;

    } else {

        return countLeaves(N -> right) + countLeaves(N -> left);

    }

}

//find the root data to delete and just free it from memory
Node* deleteSubtree(Node *root, int value) {

    if(root == NULL) {

        return root;

    }

    if(root -> data > value) {

        root -> left = deleteSubtree(root -> left, value);

    } else if(root -> data < value) {

        root -> right = deleteSubtree(root -> right, value);

    } else {

        free(root);
        return NULL;

    }

    return root;

}

//first find the wanted node and each time it goes down a level of the tree, increment depthCount, when found just
//return depthCount
int depth (Node *root, Node *N) {

    if (!isPresent(root, N->data)) {

        return -1;

    }

    if (root == NULL || N == root) {

        return 0;

    }

    int depthLeft = depth(root->left, N);
    int depthRight = depth(root->right, N);

    if (root->data > N->data) {

        return depthLeft + 1;

    } else {

        return depthRight + 1;

    }

}

//this function counts all the nodes in the tree N
int countNodes (Node *N) {

    if(N == NULL) {

        return 0;

    }

    int count = 0;

    count = countNodes(N -> right) + countNodes(N -> left) + 1;
    return count;

}

//linked list implementation
typedef struct listNode {

    int value;
    struct listNode *next;

} listNode;

//tail function for linked list
listNode* tail(listNode *node) {

    if(node == NULL) {

        return NULL;

    }

    return node -> next;

}

//function to join together left, root and right
listNode* append(listNode *leftHead, int value, listNode* rightHead) {

    if(leftHead == NULL && rightHead == NULL) {

        listNode *middle = NULL;
        middle = (listNode*) malloc(sizeof(listNode));
        middle -> value = value;
        middle -> next = NULL;

        return middle;

    }

    if (leftHead == NULL) {

        listNode *middle = NULL;
        middle = (listNode*) malloc(sizeof(listNode));
        middle -> value = value;
        middle -> next = rightHead;

        return middle;

    }

    if(value == NULL && rightHead == NULL) {

        return NULL;

    }

    listNode* node = leftHead;

    while(node -> next != NULL) {

        node = node -> next;

    }

    if(leftHead != NULL && rightHead == NULL) {

        listNode *middle = NULL;
        middle = (listNode*) malloc(sizeof(listNode));
        middle -> value = value;
        middle -> next = NULL;

        node -> next = middle;

        return leftHead;

    }

    listNode *middle = NULL;
    middle = (listNode*) malloc(sizeof(listNode));
    middle -> value = value;
    middle -> next = rightHead;
    node -> next = middle;

    return leftHead;

}

//converts a tree into an inorder linked list
listNode* inOrder(Node *node){

    if (node == NULL) {

        return NULL;

    }

    listNode *leftList = NULL;
    listNode *rightList = NULL;

    leftList = inOrder(node -> left);
    rightList = inOrder(node -> right);

    listNode *head = NULL;

    head = append(leftList, node -> data, rightList);

    return head;

}

//gets data at given index in a linked list
int getValue(listNode *listNode, int index) {

    if(index == 0) {

        return listNode -> value;

    } else {

        return getValue(tail(listNode), index - 1);

    }

}

//builds a balanced tree from an in order linked list of values
Node* buildTree (listNode *listHead, int start, int end) {

    if(start > end) {

        return NULL;

    }

    int middle = (start + end) / 2;
    int value = getValue(listHead, middle);
    Node *root = createNode(value);

    root -> left = buildTree(listHead, start, middle - 1);
    root -> right = buildTree(listHead, middle + 1, end);

    return root;

}

//gets number of nodes in the tree, converts the tree into an in order linked list, then converts that into a balance tree
Node* balanceTree(Node *root) {

    int nodeCount = countNodes(root);
    listNode *listHead = inOrder(root);
    Node *balancedRoot = buildTree(listHead, 0, nodeCount - 1);

    return balancedRoot;

}

int sumSubtree(Node *N) {

    listNode *listHead = inOrder(N);
    int nodeCount = countNodes(N);

    int sum = 0;

    for(int i = 0; i < nodeCount; i++) {

        sum += listHead -> value;
        listHead = tail(listHead);

    }

    free(listHead);
    return sum;

}

int main() {

    Node* myNodes[11];
    srand(0);
    Node* root = insertNode(NULL, 5);

    for(int i = 0; i < 12; i++) {

        int r = rand();
        r = r % 10;

        myNodes[i] =  insertNode(root, r);

    }

    Node* balancedRoot = balanceTree(root);
    printSubtree(root);
    printSubtree(balancedRoot);
    printf("Sum %d\n", sumSubtree(root));

//    printf("Depth: %d\n", depth(root, myNodes[40]));
//    printf("Count Leaves: %d\n", countLeaves(root));
//    printSubtree(myNodes[0]);


}