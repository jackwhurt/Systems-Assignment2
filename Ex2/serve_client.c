#include<pthread.h>
#include<string.h>

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime(){

	//each night-time, lock the rw lock, balance the tree and free the old unbalanced tree
	sleep(1);
	pthread_rwlock_wrlock(&lock);
	Node *temp = root;
	root = balanceTree(root);
	freeSubtree(temp);
	temp = NULL;
	pthread_rwlock_unlock(&lock);

	sleep(1);
	pthread_rwlock_wrlock(&lock);
	temp = root;
	root = balanceTree(root);
	freeSubtree(temp);
	temp = NULL;
	pthread_rwlock_unlock(&lock);
	
	sleep(1);
	pthread_rwlock_wrlock(&lock);
	temp = root;
	root = balanceTree(root);
	freeSubtree(temp);
	temp = NULL;
	pthread_rwlock_unlock(&lock);
	
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

	//fgets loads each line of the file into a buffer of length 1000
    while(fgets(buffer, 1000, file)) {

		//dynamically increase the size of the lines array if the file has more lines than it has elements
        if(i == arrayLength) {

            arrayLength += STEP_SIZE;
            lines = (char **) realloc(lines, arrayLength * sizeof(char *));

        }

		//each element in the array needs a null pointer to indicate the end of array. so if instead it has the end of line character replace it with null pointer
        if(buffer[strlen(buffer) -1] == '\n') {

            buffer[strlen(buffer) - 1] = '\0';

        }       
		
		//allocate memory for the string and load it into the double pointer
        unsigned long sLength = strlen(buffer);
        char *string = (char *) malloc((sLength + 1) * sizeof(char));

        strcpy(string, buffer);
        lines[i] = NULL;
        lines[i] = string;
        i++;

    }

    fclose(file);
    *length = i;

    return lines;

}

void* ServeClient(char *clientCommands){
		
	//lock the rw lock for each file to load, as some of the functions used in loadFile are not thread safe
	pthread_rwlock_wrlock(&lock);
	
    int length = 0;
    char **commands = 0;
    commands = loadFile(clientCommands, &length);
    
    pthread_rwlock_unlock(&lock);

	//for each line / string in the array perform its given command
    unsigned long i = 0;
    while (i != length) {
    	
		//accessing the root so will need to lock the rw lock so no old / dirty data will be read
    	pthread_rwlock_wrlock(&lock);

		//duplicate the command then separate it from the numbers
		char *command = commands[i];
		size_t commandLen = strlen(command);
		char *commandToComp = (char *) malloc(commandLen + 1);
		memcpy(commandToComp, command, commandLen + 1);

		char *commandToken = strtok(commandToComp, " ");

		commandToComp[commandLen - 1] = '\0';

		//if the command is to insert the node it will parse the number from the command and update the root and print the command
        if (strcmp(commandToken, "insertNode") == 0) {

			char *token = strtok(command, " ");
			token = strtok(NULL, " ");
			int value = atoi(token);

            root = insertNode(root, value);

            printf("[%s]%s %d\n", clientCommands, commands[i], value);
		
            // print: "[ClientName]insertNode <SomeNumber>\n"
            // e.g. "[client1_commands]insertNode 1\n"


		//if the command is to delete a node, it will parse the number from the command and delete it from the root and print the command
        } else if (strcmp(commandToken, "deleteNode") == 0) {

			char *token = strtok(command, " ");
			token = strtok(NULL, " ");
			int value = atoi(token);
            
            root = deleteNode(root, value);
			
            printf("[%s]%s %d\n", clientCommands, commands[i], value);
			
			
            // print: "[ClientName]deleteNode <SomeNumber>\n"
            // e.g. "[client1_commands]deleteNode 1\n"

		//if the command is to count the nodes it will just print the command along with how many nodes are currently in the tree
        } else if (strcmp(commandToken, "countNode") == 0) {

            // print: "[ClientName]countNodes = <SomeNumber>\n"
            // e.g. "[client1_commands]countNodes 1\n"

            int count = countNodes(root);
			
            printf("[%s]%s = %d\n", clientCommands, commands[i], count);
			
		//if the command is sum the tree then it will print the value of all the nodes added together
        } else if (strcmp(commandToken, "sumSubtre") == 0) {

            // print: "[ClientName]sumSubtree = <SomeNumber>\n"
            // e.g. "[client1_commands]sumSubtree 1\n"

            int sum = sumSubtree(root);
            
            printf("[%s]%s = %d\n", clientCommands, commands[i], sum);
           
        }

		free(commands[i]);
		free(commandToComp);

        i++;

        pthread_rwlock_unlock(&lock);


    }
    
    free(commands);
    return NULL;
    
}
