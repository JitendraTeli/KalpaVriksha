#include<stdio.h>
#include<stdlib.h>
#include<string.h>


size_t gSize = 0;  //gloabal size variable;

#define NUMS_FILE "nums.txt"

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

void writeArray(int *array) {
    FILE *numsFile = fopen(NUMS_FILE,"w");

    fprintf(numsFile,"%zu ",gSize);

    for(size_t i = 0; i<gSize; i++) {
        fprintf(numsFile,"%d ",array[i]);
    }

    fclose(numsFile);
}

void readArray(int *array) {
    FILE *numsFile = fopen(NUMS_FILE,"r");
    
    for(size_t i = 0; i<gSize; i++) {
        fscanf(numsFile,"%d",array + i);
    }

    fclose(numsFile);
}

int main() {

    int *nums = inputArrayFromCLI();
    
    printf("\nBefore Sorting");
    printArray(nums);


    writeArray(nums);

    char command[200];
    sprintf(command,"sorter %s",NUMS_FILE);

    system(command);  

    readArray(nums);
    
    printf("\nAfter Sorting");
    printArray(nums);

    free(nums);
    return 0;
}