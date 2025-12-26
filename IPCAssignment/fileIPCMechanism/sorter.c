#include<stdio.h>
#include<stdlib.h>
#include<string.h>

size_t gSize = 0;

int* readArrayFromFile(char *fileName) {
    FILE *numsFile = fopen(fileName,"r");

    fscanf(numsFile,"%zu",&gSize);

    int *array = calloc(gSize,sizeof(int));

    for(size_t i = 0; i<gSize; i++) {
        fscanf(numsFile,"%d",array + i);
    }

    fclose(numsFile);

    return array;
}

void writeArray(char *fileName,int *nums) {
    FILE *numsFile  = fopen(fileName,"w");

    for(size_t i = 0; i<gSize; i++) {
        fprintf(numsFile,"%d ",nums[i]);
    }

    fclose(numsFile);
}

int comarator(const void *num1, const void *num2) {
    int val1 = *(const int*)num1;
    int val2 = *(const int*)num2;

    return (val1 > val2) - (val1 < val2);
}

int main(int argc,char *argv[]) {

    int *nums= readArrayFromFile(argv[1]);

    qsort(nums,gSize,sizeof(int),comarator);

    writeArray(argv[1],nums);

    free(nums);

    return 0;
}