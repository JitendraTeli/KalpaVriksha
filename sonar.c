#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define True 1

void swap(int *pointer1, int *pointer2) {
    *pointer1 += *pointer2;
    *pointer2 = *pointer1 - *pointer2;
    *pointer1 -= *pointer2;
}

void reverseArray(int *array,int size) {
    for(int index = 0; index < size/2; index++) {
        swap(array + index, array + size - 1 - index);
    }
}

void transposeMatrix(int *matrix, int size) { 
    for(int row = 0; row < size; row++) {
        for(int column = row + 1; column < size; column++) {
            swap(matrix + row * size + column , matrix + column * size + row);          
        }
    }
}

void rotateMatrix(int *matrix,int size) {

    transposeMatrix(matrix,size);                              //to rotate matrix by 90 degree Step 1: transpose

    for(int row = 0; row < size; row++) {                       //Step 2:  reverse every array 
        reverseArray(matrix + row * size,size);                 //passing base address by pointer arithematics
    }   

}


void printMatrix(int *matrix,int size) {
    printf("\n");
    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            printf("%5d", *(matrix + row * size + column));     // printing matrix with pointer Arithematics
        }
        printf("\n");
    }
}

void applySmootheningFilterOnMatrix(int *matrix,int size) {       

    int *previousRow = (int *)calloc(size,sizeof(int));
    int previousValue = 0;
    int currentValue = 0;
    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            currentValue = *(matrix + row * size + column);

            int sum = 0;
            int count = 0;

            for(int adjacentRow = -1; adjacentRow <= 1; adjacentRow++) {
                for(int adjacentColumn = -1; adjacentColumn <= 1; adjacentColumn++) {
                    
                    if(row + adjacentRow >= 0 && row + adjacentRow < size && column + adjacentColumn >= 0 && column + adjacentColumn < size) {

                        if(adjacentRow == -1) {
                            sum += previousRow[column + adjacentColumn];
                        } else if( adjacentRow == 0 &&adjacentColumn == -1) {
                            sum += previousValue;
                        } else {
                            sum += *(matrix + ((row + adjacentRow)*size) + (column + adjacentColumn));
                        }
                        count++;
                    }
                }
            }

            if(column > 0) {
                previousRow[column - 1] = previousValue;
            }

            if(column == size - 1) {
                previousRow[column] = currentValue;
            }

            *(matrix + row * size + column) = sum/count;
            
            previousValue = currentValue;
        }
    }

    free(previousRow);
}

int* generateMatrixWithRandomValues(int size) {      

    int *matrix = (int *)malloc(size * size * sizeof(int));       //dynamic memory allocation for matrix N x N

    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            *(matrix + row * size + column) = rand() % 256;         // filling each cell with random value between 0 to 255 inclusive
        }
    }

    return matrix;

}

int main()  {
    
    int size = 0;                            // size of Square Matrix N x N
    int *matrix = NULL;
    srand(time(NULL));                        //passing current time as seed for random numbers

    while(True) {
        
        printf("Enter size of the image: ");
        scanf("%d",&size);

        if(size < 2 || size > 10) printf("\nInvalid size please try again\n");
        else break;
    }

    matrix = generateMatrixWithRandomValues(size);

    printf("\n\n Original Matrix :- \n");
    printMatrix(matrix,size);

    rotateMatrix(matrix,size);

    printf("\n\n Rotated Matrix :- \n");
    printMatrix(matrix,size);

    applySmootheningFilterOnMatrix(matrix,size);

    printf("\n\n Matrix after applying 3x3 Smoothening Filter :- \n");
    printMatrix(matrix,size);

    free(matrix);

    return 0;
}