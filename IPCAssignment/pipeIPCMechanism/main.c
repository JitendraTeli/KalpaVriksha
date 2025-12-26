#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



size_t gSize = 0;  


int* inputArrayFromCLI() {
    printf("Enter the size of Array : ");
    scanf("%zu",&gSize);

    int *array = calloc(gSize,sizeof(int));

    printf("\n Enter %zu elements : ",gSize);

    for(size_t i = 0; i<gSize; i++) {
        scanf("%d",array+i);
    }

    return array;
}

void printArray(int *array) {
    printf("\nPrinting the array of Size %zu :- \n -: ",gSize);
    for(size_t i = 0;i<gSize; i++) {
        printf("%d ",array[i]);
    }
    printf("\n");
}

int comarator(const void *num1, const void *num2) {
    int val1 = *(const int*)num1;
    int val2 = *(const int*)num2;

    return (val1 > val2) - (val1 < val2);
}

void read_All(int pipe,void *var,size_t size) {
    char *buffer = var;
    ssize_t byteCount = 0;

    while(size > 0) {
        byteCount = read(pipe,buffer,size);
        
        size -= byteCount;
        buffer += byteCount;
    }
}

void write_All(int pipe,void *var,size_t size) {
    char *buffer = var;
    ssize_t byteCount = 0;

    while(size > 0) {
        byteCount = write(pipe,buffer,size);

        size -= byteCount;
        buffer += byteCount;
    }
}


int main() {

    int *nums = inputArrayFromCLI();
    
    printf("\nBefore Sorting");
    printArray(nums);

    int pipeStream[2];

    if(pipe(pipeStream)) return 0;

    int id = fork();

    if(id < 0) return 0;

    if(id == 0) {         //child process
        qsort(nums,gSize,sizeof(int),comarator);

        close(pipeStream[0]);        //reading end

        write_All(pipeStream[1],nums,gSize * sizeof(int));

        close(pipeStream[1]);

    } else { //parent process

        waitpid(id,NULL,0);

        close(pipeStream[1]);               //writing end

        read_All(pipeStream[0],nums,gSize * sizeof(int));

        close(pipeStream[0]);

        printf("\nAfter Sorting");
        printArray(nums);
    }

    free(nums);
    return 0;
}