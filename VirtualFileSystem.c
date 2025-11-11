#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAME_LENGTH 51
#define BLOCK_SIZE 8
#define TOTAL_BLOCKS 500

#define True 1
#define False 0

typedef struct FreeNode {
    int index;
    struct FreeNode *next;
    struct FreeNode *previous;
} FreeNode;

typedef struct FileNode {
    char type;
    char name[NAME_LENGTH];
    
    struct FileNode *next;
    struct FileNode *previous;
    struct FileNode *parent;

    union {
        struct FileNode *child;
        struct FreeNode *block;
    };

} FileNode;

char Memory[TOTAL_BLOCKS][BLOCK_SIZE];        //virtual Memory
int allocations = 0;

FreeNode *start = NULL;

int countBlocks(FreeNode* block) {
    FreeNode *traverser = block;
    int count = 0;

    do {
        count++;
        traverser = traverser->next;
    } while(traverser != block);

    return count;
}

void appendFreeBlocks(FreeNode* newNode) {

    if(start == NULL) {
        start = newNode;
        return;
    }

    start->previous->next = newNode;
    newNode->previous->next = start;
    
    FreeNode *temp = newNode->previous;
    newNode->previous = start->previous;
   
    start->previous = temp;
    
}

FreeNode* allocateFreeBlocks(int count) {
    if(count > (TOTAL_BLOCKS - allocations)) return NULL;

    if(start == NULL) {
        return NULL;
    }
    
    allocations += count;
    FreeNode *freeBlock = start;
    
    while(count-- > 0)  {
        if(start->next == freeBlock) {
            start = NULL;
            return freeBlock;
        } 

        start = start->next;
    }
    
    freeBlock->previous->next = start;
    start->previous->next = freeBlock;
    
    FreeNode *temp = freeBlock->previous;
    
    freeBlock->previous = start->previous;
    start->previous = temp;
    
    return freeBlock;
}

void destroyBlock(FreeNode *current) {
    if(current->next != start) {
        destroyBlock(current->next);
    }

    free(current);
}

void destroyMemory() {
    destroyBlock(start);
    start = NULL;
}

void initializeFreeBlocks() {
    for(int i = 0; i < TOTAL_BLOCKS; i++) {
        FreeNode *newNode = (FreeNode *) malloc(sizeof(FreeNode));
        newNode->index = i;
        newNode->next = newNode;
        newNode->previous = newNode;
        appendFreeBlocks(newNode);
    }

}



// Doubly Circular List file System

FileNode *cwd = NULL;    //current working Directory
FileNode *rootDirectory = NULL;

FileNode* appendFileNode(FileNode *root,FileNode* newNode) {  //placing of newNode
    
    if(root == cwd) {
        root = newNode;
        root->next = newNode;
        root->previous = newNode;
        
        return root;
    }
    
    newNode->previous = root->previous;
    newNode->next = root;
    
    root->previous->next = newNode;
    root->previous = newNode;
    
    return root;
}
FileNode* findNode(char *name) {
    
    if(cwd->child == cwd) {
        return NULL;
    }
    
    FileNode *temp = cwd->child;
    
    while(temp->next != cwd->child) {
        if(strcmp(temp->name,name) == 0) return temp;
        temp = temp->next;
    }
    
    if(strcmp(temp->name,name) == 0) return temp;
    
    return cwd;
}

FileNode* getFileNode(char *name) {
    
    FileNode *foundNode = findNode(name);
    
    if(foundNode == NULL) {
        printf("\nDirectory is empty");
        return cwd;
    }
    
    if(foundNode == cwd) {
        printf("\n%s not found",name);
        return cwd;
    }
    
    return foundNode;
}

void insertChild(char *name,char type) {
    
    if(cwd != NULL) {
        if(cwd->child != cwd) {
            if(findNode(name) != cwd) {
                printf("\n%s already exists",name);
                return;
            }
        }
    }
    
    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    
    
    strcpy(newNode->name,name);
    newNode->type = type;
    
    if(cwd == NULL) { //case : root
        cwd = newNode;
        newNode->next = newNode;
        newNode->previous = newNode;
    } else cwd->child = appendFileNode(cwd->child,newNode);
    
    
    newNode->parent = cwd;
    if(type == 'd') newNode->child = newNode;
    else newNode->block = NULL;
}



FileNode* removeFileNode(FileNode *root,FileNode *temp) {
    if(temp == root) {
        return cwd;
    }
    temp->previous->next = temp->next;
    temp->next->previous = temp->previous;
    
    return root;
}



void removeChild(FileNode *badChild) {
    cwd->child = removeFileNode(cwd->child,badChild);
    free(badChild);
}

void clearFile(FileNode *file) {
    if(file->block != NULL) {
        allocations -= countBlocks(file->block);
    
        appendFreeBlocks(file->block);
        file->block = NULL;
    }
}

void removeFile(char *name) {
    FileNode *temp = getFileNode(name);
    
    if(temp != cwd) {
        if(temp->type != 'f') {
            printf("%s is not a File",name);
        } else {
            clearFile(temp);
            removeChild(temp);
            printf("removed %s",name);
        }
    }
}

void removeDirectory(char *name) {
    FileNode *temp = getFileNode(name);

    if(temp != cwd) {
        if(temp->type != 'd') {
            printf("\n%s is not a Directory",name);
        } else if(temp->child != temp) {
            printf("\n %s Directory is not Empty",name);
        } else {
            removeChild(temp);
            printf("removed %s",name);
        }
    }
}

char *writeBlock(char *string,FreeNode *block) {

    if(block != NULL) 
    for(int i = 0; i<BLOCK_SIZE; i++) {
        Memory[block->index][i] = *string;
        
        if(*string == '\0') break;
        string++;
    }

    return string;
}

void putData(char *string,FreeNode *block) {
    if(block == NULL) {
        printf("error : not enough memory write operation failed");
        return;
    }

    FreeNode *temp = block;
    do{
        string = writeBlock(string,temp);
        temp = temp->next;
    } while(temp != block);
} 

void writeFile(char *name,char *string) {
    FileNode *file = getFileNode(name);

    if(file != cwd) {

        if(file->type != 'f') {
            printf("%s is not a file",name);
            return;
        }

        clearFile(file);
        int length = strlen(string);

        file->block = allocateFreeBlocks(length/BLOCK_SIZE + (length%BLOCK_SIZE != 0));

        if(file->block != NULL)  putData(string,file->block);
        else printf("memory overflow, cannot write in file");
    }
}

void printData(FreeNode *block) {

    if(block == NULL) {
        printf("no memory for this file");
        return;
    }

    FreeNode *temp = block;
    do{
        for(int i = 0; i<BLOCK_SIZE; i++) {
            if(Memory[temp->index][i] == '\0') return;

            putchar(Memory[temp->index][i]);
        }
        temp = temp->next;
    } while(temp != block);
}

void readFile(char *name) {
    FileNode *file = getFileNode(name);

    if(file != cwd) {

        if(file->type != 'f') {
            printf("%s is not a file",name);
            return;
        }

        printData(file->block);
    }
}

void changeDirectory(char *name) {
    if(!strcmp(name,"..")) {
        cwd = cwd->parent;
        return;
    }

    FileNode *directory = getFileNode(name);

    if(directory != cwd) {
        if(directory->type != 'd') {
            printf("%s is not a directory",name);
        } else {
            cwd = directory;
        }
    }
}

void listDirectory() {
    if(cwd->child == cwd) {
        printf("(empty)");
    } else  {
        FileNode *iterator = cwd->child;
        
        printf("\n ");

        do {
            printf("%s  ",iterator->name);
            iterator = iterator->next;

        } while(iterator != cwd->child);
        
    } 
}


void CurrentWorkingDirectory(FileNode *current) {
    if(current == rootDirectory) {
        printf("/");
        return;
    }
    CurrentWorkingDirectory(current->parent);
    printf("%s/",current->name);
    return;
}

void presentWorkingDirectory() {
    printf("\n");
    CurrentWorkingDirectory(cwd);
}

void clearInput() {
    int something;
    while((something = getchar()) != '\n' && something != EOF);
}

char* getName() {
    char *name = (char *) malloc(NAME_LENGTH + 10 * sizeof(char));

    scanf("%60s",name);
    
    if(strlen(name) > 50 ) {
        printf("error : name length cannot exceed 50");
        clearInput();
        free(name);
        return NULL;
    }

    return name;
}

void moveDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        changeDirectory(directory);
        free(directory);
    }
}

void makeDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        insertChild(directory,'d');
        free(directory);
    }
}

void deleteDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        removeDirectory(directory);
        free(directory);
    } 
}

void createFile() {
    char *name = getName();

    if(name != NULL) {
        insertChild(name,'f');
        free(name);
    }
} 

void deleteFile() {
    char *name = getName();

    if(name != NULL)  {
        removeFile(name);
        free(name);
    }
}

void writeInFile() {
    char *name = getName();

    if(name != NULL) {
        char string[512];
        fgets(string,511,stdin);

        writeFile(name,string);
        free(name);
    }
}

void readFromFile() {
    char *name = getName();
    
    if(name != NULL) {
        readFile(name);
        free(name);
    }
}

void diskInfo() {
    printf("\nTotal Blocks: %d",TOTAL_BLOCKS);
    printf("\nUsed Blocks: %d",allocations);
    printf("\nFree Blocks: %d",TOTAL_BLOCKS - allocations);
    printf("\nDisk Usage: %.2f%c",(float)(((float)allocations/(float)TOTAL_BLOCKS)*100),'%');
}

void execute(char *command) {
    if(strcmp(command,"ls") == 0) listDirectory();
    else if(strcmp(command,"pwd") == 0) presentWorkingDirectory();
    else if(strcmp(command,"cd") == 0) moveDirectory();
    else if(strcmp(command,"mkdir") == 0) makeDirectory();
    else if(strcmp(command,"rmdir") == 0) deleteDirectory();
    else if(strcmp(command,"create") == 0) createFile();
    else if(strcmp(command,"delete") == 0) deleteFile();
    else if(strcmp(command,"df") == 0) diskInfo();
    else if(strcmp(command,"write") == 0) writeInFile();
    else if(strcmp(command,"read") == 0) readFromFile();
    else printf("%s is not a valid command",command);
}

void getInput() {
    while(True) {
        char command[20];
        printf("\n@-@ %s >",cwd->name);
        scanf(" %19s",command);


        if(strlen(command) > 9) {
            printf("\n%s is not a valid command",command);
            clearInput();
        } else if(strcmp(command,"exit") == 0) {
            break;
        } else {
            execute(command);
        }
    }
}

// This function traverses whole VFS (a nested Linked List) and frees every node 
void destroySelf(FileNode *prey) {

    if(prey->type == 'd') {
        if(prey->child != prey) {
            cwd = prey;

            destroySelf(prey->child);

            prey->child = prey;
            cwd = prey->parent;
        }
    }

    if(prey->next != cwd->child) {

        destroySelf(prey->next);
    }

    if(prey->type == 'f') clearFile(prey);

    free(prey);

    return;
}

void destroyVFS() {
    cwd = rootDirectory;
    destroySelf(rootDirectory);
}

int main() {
    insertChild("/",'d');

    rootDirectory = cwd;

    initializeFreeBlocks();

    getInput();


    destroyVFS();


    destroyMemory();

    return 0;

}