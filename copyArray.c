#include<stdio.h>
#include<stdlib.h>


int *copyPaste(int *array,int size) {
    int *clone = (int *) malloc(size * sizeof(int));

    for(int i = 0; i<size; i++) *(clone + i) = *(array + i);

    return clone;
}

void printArray(int *array,int size) {
    printf("\n");
    for(int i = 0; i<size; i++) printf(" %d ",array[i]);
    printf("\n");
}

int* inputArray(int size) {
    int *array = (int *) malloc(size * sizeof(int));

    for(int i =0; i<size; i++) {
        scanf("%d",array + i);
    }

    return array;
}

int main() {
    int *array = NULL,*clone = NULL,size,secondBottom,secondTop;

    printf("\n Enter the Total Number of Element : ");
    scanf("%d",&size);

    array = inputArray(size);

    clone = copyPaste(array,size);

    printArray(array,size);
    printArray(clone,size);

    return 0;
}