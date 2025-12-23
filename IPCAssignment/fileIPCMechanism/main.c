#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// this is a unix specific program and doesn't work in windows
// the testing of this programm is done in a vmware workstation vm which is running RHEL

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
    printf("\n \n Printing the array of Size %zu :- \n -: ",gSize);
    for(size_t i = 0;i<gSize; i++) {
        printf("%d ",array[i]);
    }
    printf("\n");
}

void writeArray(int *array) {
    FILE *numsFile = fopen(NUMS_FILE,"w");

    fprintf(numsFile,"SIZE %zu \n",gSize);

    for(size_t i = 0; i<gSize; i++) {
        fprintf(numsFile,"%d ",array[i]);
    }

    fclose(numsFile);
}

void readArray(int *array) {
    FILE *numsFile = fopen(NUMS_FILE,"r");
    
    for(size_t i = 0; i<gSize; i++) {
        fscanf("%d",array + i);
    }

    fclose(numsFile);
}

int main() {

    int *nums = inputArrayFromCLI();

    printArray(nums);


    writeArray(nums);

    char command[200];
    sprintf(command,"./sorter %s",NUMS_FILE);
    
    system(command);  

    readArray(nums);
    

    printArray(nums);

    free(nums);
    return 0;
}