#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

// Node declaration

typedef struct Node {
    char* word;
    int length;
    struct Node* next;
    struct Node* prev;

} Node;

// Create a new node

Node* createNode(char* word, int length) {

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->word = word;
    newNode->length = length;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

// Insert after current Node

void insertAfterNode(Node* currentNode, char* word, int length) {

    Node* newNode = createNode(word, length);
    newNode->next = currentNode->next;
    currentNode->next = newNode;
    newNode->prev = currentNode;

    if(newNode->next != NULL) {
        newNode->next->prev = newNode;
    }
}

void deleteCurrentNode(Node* currentNode, Node* head) {
    if(currentNode == head) {
        head = NULL;
    }
    else {
        currentNode->prev->next = currentNode->next;
        if(currentNode->next != NULL) {
            currentNode->next->prev = currentNode->prev;
        }
    }
}

// To convert the given file data to the list representation

Node* covertStringToDoublyLinkedList(char* fileData) {

    Node* head = createNode("Dummy", 0);
    Node* curr = head;

    for(int i = 0 ; fileData[i] != '\0'; i++) {

        int end = i;

        // find the next white space

        while(fileData[end] != '\0' && fileData[end] != ' ') {
            end++;
        }

        // Create a new Node

        char* word = (char*)calloc(end - i + 1, sizeof(char));

        for(int j = i ; j < end ; j++) {
            char curr[1] = { fileData[j] };
            strcat(word, curr);
        }

        Node* newNode = createNode(word, end - i + 1);
        curr->next = newNode;
        newNode->prev = curr;
        curr = curr->next;

        i = fileData[end] == '\0' ? end - 1 : end;
    }

    if(head->next != NULL) {
        head->next->prev = NULL;
    }

    return head->next;
}

// Convert the given doubly linked list to a string.

char* covertDoublyLinkedListToString(Node* head, int totalLength) {

    char* res = (char*)calloc(totalLength, sizeof(char)); 

    while(head->next != NULL) {

        strcat(res, head->word);
        strcat(res, " ");
        head = head->next;

    }

    return res;
}

// To calculate the size of the file

int getFileSize(char* filename) {

    struct stat st;
    stat(filename, &st);

    return st.st_size;
}


// To read the file data

char* readFile(char* filename, int fileSize) {

    // Open the file

    FILE *fp = fopen(filename, "r");

    // Result Buffer

    char* res = (char*)calloc(fileSize, sizeof(char));

    // Read the file

    if(fp == NULL) {
        printf("Unable to open the file.");
        exit(0);
    }
    else {

        while(!feof(fp)) {

            char* buff = (char*)calloc(100, sizeof(char));

            fread(buff, sizeof(char), 100, fp);
            strcat(res, buff);

            free(buff);
        }

    }

    // Close the file

    fclose(fp);

    return res;
}

// Print the user view

void printUserView(char* fileData, int cursorPos) {

    system("clear");
    printf("Current cursor position: %d\n\n", cursorPos);
    printf("Current file content:\n\n");
    printf("%s", fileData);
    printf("\n\n");
    printf("Available Options\n\n");
    printf("F - To move the cursor forward \n");
    printf("B - To move the cursor backward \n");
    printf("S - To move the cursor to the start \n");
    printf("E - To move the cursor to the end \n");
    printf("Q - To search get the position of a word in the file \n");
    printf("U - To undo the last move \n");
    printf("P - To save the changes \n\n");
    printf("Press any other key to exit \n\n");

}

// check if the operation is valid or not

int isValidOperation(char operation) {

    int result = operation == 'F' || operation == 'f' ||
                 operation == 'B' || operation == 'b' ||
                 operation == 'S' || operation == 's' ||
                 operation == 'E' || operation == 'e' ||
                 operation == 'Q' || operation == 'q' ||
                 operation == 'U' || operation == 'u' ||
                 operation == 'P' || operation == 'p';

    return result;             
}

// Move cursor forward

void moveCusrorForward(int* currPos) {
    *currPos = *currPos + 1;
}

// Move cursor backward

void moveCusrorBackward(int* currPos) {
    *currPos -= 1;
}

// Move cursor to the start

void moveCusrorStart(int* currPos) {
    *currPos = 0;
}

void moveCusrorEnd(int* currPos, int endPos) {
    *currPos = endPos;
}

// Perform the given task

void performTask(char opcode,char* fileData, int* currPos) {
    switch(opcode) {
        case 'f':
        case 'F':
            moveCusrorForward(currPos);
            break;
        case 'b':
        case 'B':
            moveCusrorBackward(currPos);
            break;
        case 's':
        case 'S':
            moveCusrorStart(currPos);
            break;
        case 'e':
        case 'E':
            moveCusrorEnd(currPos, 100);
            break;
        case 'q':
        case 'Q':
            break;
        case 'u':
        case 'U':
            break;
        case 'p':
        case 'P':
            break;  
        default:
            printf("Invalid Operation, Exiting...");                  
    }
}

// Driver code

int main()
{

    // Open the file and read te saved data

    int* fileSize = (int*)calloc(1, sizeof(int));
    *fileSize = getFileSize("abc.txt");

    char* fileData = readFile("abc.txt", *fileSize);
    
    // Convert the data to a doubly LinkedList

    Node* head = covertStringToDoublyLinkedList(fileData);
    Node* activeNode = head;

    int* currPos = (int*)calloc(1, sizeof(int));

    // Program begins

    char currenOperation = ' ';
    do {
        printUserView(fileData, *currPos);
        scanf(" %c", &currenOperation);
        performTask(currenOperation, fileData, currPos);
        fileData = covertDoublyLinkedListToString(head, *fileSize);

    } while(isValidOperation(currenOperation));

    // Ending screen

    system("clear");
    printf("\n\nThanks for using! \n");
    printf("Hope to see you soon :) \n");
    printf("Bye! \n\n");

    return 0;
}
