#include<stdio.h>
#include<stdlib.h>


#define True 1
#define False 0

int alreadyExplored(int value,int* array,int size) {
    for(int i=0;i<size; i++) {
        if(array[i] == value) return True;
    }
    return False;
}

int shrink(int *array,int size) {
    int index = 0;

    for(int i = 1;i<size;i++) {
        if(alreadyExplored(array[i],array,i)) continue;
        
        array[++index] = array[i];
    }

    return ++index;
}

int* inputArray(int size) {
    int *array = (int *) malloc(size * sizeof(int));

    for(int i =0; i<size; i++) {
        scanf("%d",array + i);
    }

    return array;
}

void printArray(int *array,int size) {
    printf("\n");
    for(int i = 0; i<size; i++) printf(" %d ",array[i]);
    printf("\n");
}

int main() {
    int *array = NULL,size,secondBottom,secondTop;
    int error = 0;

    printf("\n Enter the Total Number of Element : ");
    scanf("%d",&size);

    array = inputArray(size);
    size = shrink(array,size);

    printf("\n new array :- ");
    printArray(array,size);
    printf("\n new size : %d",size);

    return 0;
}