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

void countFrequencies(int* array,int size) {
    int current,count;
    for(int i = 0; i<size; i++) {
        current = array[i];

        if(alreadyExplored(current,array,i)) continue;

        count = 0;
        for(int j = i; j<size; j++) {
            if(array[j] == current) count++;
        }

        printf("\nElement %d appears %d times.\n",current,count);

    }    
}

int* inputArray(int size) {
    int *array = (int *) malloc(size * sizeof(int));

    for(int i =0; i<size; i++) {
        scanf("%d",array + i);
    }

    return array;
}

int main() {
    int *array = NULL,size,secondBottom,secondTop;
    int error = 0;

    printf("\n Enter the Total Number of Element : ");
    scanf("%d",&size);

    if(size < 2) {
        printf("\nthere is no 2nd smallest or 2nd largest element in this array");
        return 0;
    }

    array = inputArray(size);

    countFrequencies(array,size);

    return 0;
}