#include<stdio.h>
#include <assert.h>
#include<stdlib.h>
#include "bst.h"

typedef struct _Node {

    int value;
    Node* left;
    Node* right;

} Node;

//initialising a new node
Node* createNode(int value) {

    Node* newNode = (Node*)malloc(sizeof(struct _Node));
    newNode -> value = value;
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;

}

Node* insertNode(Node *root, int value) {

    //if the tree is empty assign the new node the int value and return it
    if(root == NULL) {

        //allocating memory for a new node
        Node* newNode = createNode(value);
        return newNode;

    }

    //recur down the tree until a node is found which has empty left or right pointers
    if(root -> value > value) {

        if(root -> left == NULL) {

            Node* newNode = createNode(value);
            root -> left = newNode;
            return newNode;

        } else {

            return(insertNode(root -> left, value));

        }

    }else if(root -> value < value) {

        if(root -> right == NULL) {

            Node* newNode = createNode(value);
            root -> right = newNode;
            return newNode;

        } else {

            return(insertNode(root -> right, value));

        }

    }

    //if the value already exists return null
    return NULL;

}

//recurs throughout the tree to check if a specificed value is present
int isPresent(Node *root, int value) {

    if(root == NULL) {

        return 0;

    }

    if(root -> value == value) {

        return 1;

    } else {

        return isPresent(root -> right, value) + isPresent(root -> left, value);

    }

}

Node* deleteNode(Node *root, int value) {

    if(root == NULL) {

        return NULL;

    }

    //recur down the tree until the value is found making sure to check there aren't any null pointers
    if(root -> value > value) {

        root -> left = deleteNode(root -> left, value);

    } else if(root -> value < value) {

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

        //if it has two, replace the node to delete with the smallest value on its right subtree
        Node* newNode = root -> right;

        do {

            if(newNode -> left != NULL) {

                newNode = newNode -> left;

            }

        } while(newNode -> left != NULL);

        root -> value = newNode -> value;
        root -> right = deleteNode(root -> right, root -> value);

    }

    return root;

}

//recur through the tree starting on the left and print the values in the left, root, right order
void printSubtree(Node *N) {

    if(N -> left != NULL) {

        printSubtree(N -> left);

    }

    printf("%d\n", N -> value);

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

//find the root value to delete and just free it from memory
Node* deleteSubtree(Node *root, int value) {

    if(root == NULL) {

        return root;

    }

    if(root -> value > value) {

        root -> left = deleteSubtree(root -> left, value);

    } else if(root -> value < value) {

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

    if (!isPresent(root, N->value)) {

        return -1;

    }

    if (root == NULL || N == root) {

        return 0;

    }

    int depthLeft = depth(root->left, N);
    int depthRight = depth(root->right, N);

    if (root->value > N->value) {

        return depthLeft + 1;

    } else {

        return depthRight + 1;

    }

}



void main() {

    Node* myNodes[1001];
    srand(0);
    Node* root = insertNode(NULL, 500);

    for(int i = 0; i < 1000; i++) {

        int r = rand();
        r = r % 1000;

        myNodes[i] =  insertNode(root, r);


    }

    printSubtree(root);

    printf("Depth: %d\n", depth(root, myNodes[40]));
    printf("Count Leaves: %d\n", countLeaves(root));
    printSubtree(myNodes[0]);


}