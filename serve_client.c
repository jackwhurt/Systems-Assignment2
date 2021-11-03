#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include "bst.h"


//pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime(Node *root){

    sleep(1);
    balanceTree(root);

    sleep(1);
    balanceTree(root);

    sleep(1);
    balanceTree(root);
    printf("\n");

    return NULL;
}

const int STEP_SIZE = 100;

char **loadFile(char *fileName, int *length) {

    FILE *file = fopen(fileName, "r");

    if(file == NULL) {

        return NULL;

    }

    int arrayLength = 0;
    char **lines = NULL;
    char buffer[1000];
    int i = 0;

    while(fgets(buffer, 1000, file)) {

        if(i == arrayLength) {

            arrayLength += STEP_SIZE;
            lines = realloc(lines, arrayLength * sizeof(char *));

        }

        if(buffer[strlen(buffer) - 1] == '\n') {

            buffer[strlen(buffer) - 1] = '\0';

        }

        unsigned long sLength = strlen(buffer);
        char *string = (char *) malloc((sLength + 1) * sizeof(char));

        strcpy(string, buffer);
        lines[i] = string;
        i++;

    }

    fclose(file);
    *length = i;
    return lines;

}

void* ServeClient(char *clientCommands){

    Node *root = (Node*)malloc(sizeof(struct Node));
    root -> data = 0;
    root -> left = NULL;
    root -> right = NULL;

    int length = 0;
    char **commands = 0;
    commands = loadFile(clientCommands, &length);

    int i = 0;

    while (i != length) {

        if (strstr(commands[i], "insertNode")) {

            unsigned long strLength = strlen(commands[i]);
            char *command = commands[i];
            char stringValue[strLength - 11];

            memcpy(stringValue, &command[11], strLength - 11);
            stringValue[strLength - 11] = '\0';
            int value = atoi(stringValue);

            insertNode(root, value);

            printf("[%s]%s\n", clientCommands, commands[i]);

            // print: "[ClientName]insertNode <SomeNumber>\n"
            // e.g. "[client1_commands]insertNode 1\n"


        } else if (strstr(commands[i], "deleteNode")) {

            unsigned long strLength = strlen(commands[i]);
            char *command = commands[i];
            char stringValue[strLength - 11];

            memcpy(stringValue, &command[11], strLength - 11);
            stringValue[strLength - 11] = '\0';
            int value = atoi(stringValue);

            deleteNode(root, value);

            printf("[%s]%s\n", clientCommands, commands[i]);

            // print: "[ClientName]deleteNode <SomeNumber>\n"
            // e.g. "[client1_commands]deleteNode 1\n"


        } else if (strstr(commands[i], "countNodes")) {

            // print: "[ClientName]countNodes = <SomeNumber>\n"
            // e.g. "[client1_commands]countNodes 1\n"

            int count = countNodes(root);

            printf("[%s]%s = %d\n", clientCommands, commands[i], count);


        } else if (strstr(commands[i], "sumSubtree")) {

            // print: "[ClientName]sumSubtree = <SomeNumber>\n"
            // e.g. "[client1_commands]sumSubtree 1\n"

            int sum = sumSubtree(root);

            printf("[%s]%s = %d\n", clientCommands, commands[i], sum);


        }

        i++;


    }

    free(commands);
    free(root);
    return NULL;
}


int main() {

    ServeClient("/Users/jackhurt/Desktop/Uni/CS/Year2/C/Systems-Assignment2/client1_commands");



}