#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "bst.h"

//initialising a new node
Node* createNode(int value) {

    Node *newNode = NULL;
    newNode = (Node*)malloc(sizeof(struct Node));
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

        root -> left = insertNode(root -> left, value);

    } else if(root -> data < value) {

        root -> right = insertNode(root -> right, value);
     
    }

    //if the data already exists return the root
    return root;

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

Node * deleteNode(Node * root, int value) {
   
	if(root == NULL) {

        return root;

    }
	
	if(root -> data == value && root -> left == NULL && root -> right == NULL) {

		free(root);
		return NULL;

	}

	//if the root value is less than the root value traverse left of the tree, if more than traverse right
	if(value < root -> data) {

		root -> left = deleteNode(root -> left, value);

	} else if(value > root -> data) {

		root -> right = deleteNode(root -> right, value);

	} else {

		//if the root value is equal to the value to delete with less than two children, replace it with its child or null respectively
		if(root -> left == NULL) {
			
			struct Node *temp = root -> right;
			free(root);
			return temp;

		} else if(root -> right == NULL) {

			struct Node *temp = root -> left;
			free(root);
			return temp;

		}

		//if it has two children find the largest value on its right subtree to replace the data with
		struct Node *current = root -> right;

		while (current && current -> left != NULL) {
			
			current = current -> left;

		}
		
		//delete the now duplicate node
		root -> data = current -> data;

		root -> right = deleteNode(root -> right, current -> data);

	}
	
  	return root;

}

//recur through the tree starting on the left and print the values in the left, root, right order
void printSubtree(Node *N) {

	if(N == NULL) {

		return;

	}

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

Node* freeSubtree(Node *root) {

    if(root == NULL) {

	return NULL;

    }

    freeSubtree(root -> left);
    freeSubtree(root -> right);

    free(root);
    
    return NULL;

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

void freeList(listNode *listHead) {

    listNode *tempNode = NULL;

    while(listHead != NULL) {

	tempNode = listHead;
	listHead = listHead -> next;
	free(tempNode);

    }

}


//function to join together left, root and right
listNode* append(listNode *leftHead, int value, listNode* rightHead) {

    if(leftHead == NULL && rightHead == NULL) {

        listNode *middle = NULL;
        middle = (listNode*) malloc(sizeof(struct listNode));
        middle -> value = value;
        middle -> next = NULL;

        return middle;

    }

    if (leftHead == NULL) {

        listNode *middle = NULL;
        middle = (listNode*) malloc(sizeof(struct listNode));
        middle -> value = value;
        middle -> next = rightHead;

        return middle;

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
Node* buildTree(listNode *listHead, int start, int end) {

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
    
    freeList(listHead);
    
    return balancedRoot;

}

//to sum the subtree convert it into a linked list and then iterate through
int sumSubtree(Node *N)
{

    listNode *listHead = NULL;
    listHead = inOrder(N);
    int nodeCount = countNodes(N);

    int sum = 0;

    for(int i = 0; i < nodeCount; i++) {

        sum += listHead -> value;
        listNode *tempNode = listHead;
        listHead = tail(listHead);
        free(tempNode);

    }

    return sum;

}





