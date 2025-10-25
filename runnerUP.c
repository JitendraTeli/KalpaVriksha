#include<stdio.h>
#include<stdlib.h>

// q : find the 2nd smallest and 2nd largest element 


int getSeconds(int *array,int size,int *secondLargest,int *secondSmallest) {
    int largest = INT_MIN, smallest = INT_MAX;

    for(int i = 0;i<size; i++) {

        if(largest < array[i] && array[i] != largest) {
            *secondLargest = largest;
            largest = array[i];
        } else if(*secondLargest < array[i]) {
            *secondLargest = array[i];
        }

        if(smallest > array[i]) {
            *secondSmallest = smallest;
            smallest = array[i];
        } else if(*secondSmallest > array[i] && array[i] != smallest) {
            *secondSmallest = array[i];
        }
    }
    
    return (smallest == largest);  
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

    error = getSeconds(array,size,&secondTop,&secondBottom);


    if(error) {
        printf("\nthere is no 2nd smallest or 2nd largest element in this array");
    } else {
        printf("\n\nsecond Largest : %d",secondTop);
        printf("\nSecond Smallest : %d",secondBottom);
    }

    return 0;
}