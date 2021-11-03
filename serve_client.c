#include<stdio.h>
#include <string.h>

#include "bst.c"
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime(){

    sleep(1);
    //TODO: 1st downtime: Do balanceTree here

    sleep(1);
    //TODO: 2nd downtime: Do balanceTree here

    sleep(1);
    //TODO: 3rd downtime: Do balanceTree here

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

        int sLength = strlen(buffer);
        char *string = (char *) malloc((sLength + 1) * sizeof(char));

        strcpy(string, buffer);
        lines[i] = string;
        i++;

        printf(lines[i]);

    }

    fclose(file);
    *length = i;
    return lines;

}

void* ServeClient(char *clientCommands){

    // TODO: Open the fileContent and read commands line by line

    int length = 0;
    char **commands = 0;
    commands = loadFile(clientCommands, &length);


    int i = 0;

    while (commands[i] != "\0") {

        if (strstr(commands[i], "insertNode")) {

            char *stringValue = 0;
            memcpy(stringValue, &commands[i][11], strlen(commands[i]) - 12);
            int value = atoi(stringValue);

            Node *node = createNode(value);
            insertNode(node, value);

        } else if (strstr(commands[i], "deleteNode")) {


        } else if (strstr(commands[i], "countNodes")) {


        } else if (strstr(commands[i], "sumSubtree")) {


        }


    }



    // TODO: match and execute commands

    // TODO: Handle command: "insertNode <some unsigned int value>"
    // print: "[ClientName]insertNode <SomeNumber>\n"
    // e.g. "[client1_commands]insertNode 1\n"

    // TODO: Handle command: "deleteNode <some unsigned int value>"
    // print: "[ClientName]deleteNode <SomeNumber>\n"
    // e.g. "[client1_commands]deleteNode 1\n"

    // TODO: Handle command: "countNodes"
    // print: "[ClientName]countNodes = <SomeNumber>\n"
    // e.g. "[client1_commands]countNodes 1\n"


    // TODO: Handle command: "sumSubtree"
    // print: "[ClientName]sumSubtree = <SomeNumber>\n"
    // e.g. "[client1_commands]sumSubtree 1\n"


    return NULL;
}
