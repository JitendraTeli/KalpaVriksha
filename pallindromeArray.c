#include<stdio.h>
#include<stdlib.h>

#define True 1
#define False 0

int isPallindrome(int *array,int size) {
    for(int i = 0; i<size/2; i++) {
        if(array[i] != array[size - 1 - i]) return False;
    }

    return True;
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

    printf("\n Enter the Total Number of Element : ");
    scanf("%d",&size);

    array = inputArray(size);

    if(isPallindrome(array,size)) {
        printf("\nPallindrome\n");
    } else {
        printf("\nNot Pallindrome");
    }

    return 0;
}