#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/*
caution: in this implementation of Sonar Image processing the memory block assigned to one single Image is continous single block of size N x N 
since normal matrix indexing like Matrix[i][j] doesn't work on dynamically allocated continuous block
and also due to the requirement of using pointer arithematic for this Assignment
only pointer Arithematic is used throuout the implementation

though int *matrix is an array of size N x N , it is treated as a matrix using pointer Arithematics 

where matrix[i][j]  = *(matrix  +  i * N + j);

*/


void swap(int *a, int *b) {
    *a += *b;
    *b = *a - *b;
    *a -= *b;
}

void reverseArray(int *array,int size) {
    for(int index = 0; index < size/2; index++) {
        swap(array + index, array + size - 1 - index);
    }
}

void transposeMatrix(int *matrix, int size) {
    for(int row = 0; row < size; row++) {
        for(int column = row + 1; column < size; column++) {
            swap(matrix + row * size + column , matrix + column * size + row);          //replacing matrix[i][j] with matrix[j][i];
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
            printf(" %d ", *(matrix + row * size + column));     // printing matrix with pointer Arithematics
        }
        printf("\n");
    }
}


//unfinished function 

void applySmootheningFilterOnMatrix(int *matrix,int size) {       //using pointer arithematics instead of indexing

    int previousRow[] = {0};
    int previousValue = 0;
    int currentValue = 0;
    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            currentValue = *(matrix + row * size + column);
            if(row == 0)                                              //top row
            {
                previousRow[column] = *(matrix + row * size + column);  //matrix[row][column];

                if(column == 0)       //Top-Left Corner
                {
                    *(matrix + row * size + column) += *(matrix + row * size + column + 1) + *(matrix +  )
                    *(matrix + row * size + column) /= 4;
                }
                else if(column == size - 1)  
                {

                }
                else {

                }
            } 
            else if(row == size - 1)                                  //bottom row
            {
                if(column == 0)       
                {

                }
                else if(column == size - 1)  
                {

                }
                else {

                }
            }
            else                                                      //middle rows
            {
                if(column == 0)       
                {

                }
                else if(column == size - 1)  
                {

                }
                else {

                }
            }

            previousValue = currentValue;
        }
    }
}

int* generateMatrixWithRandomValues(int size) {       // this function allocates continuous block of memory of size of N x N for integers

    int *matrix = (int *)malloc(size * size * sizeof(int));       //dynamic memory allocation for matrix N x N

    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            *(matrix + row * size + column) = rand() % 256;         // filling each cell with random value between 0 to 255 inclusive
        }
    }

}

int main()  {
    
    int size = 0;                            // size of Square Matrix N x N

    srand(time(NULL));                        //passing current time as seed for random numbers

    printf("Enter size of the image: ");
    scanf("%d",&size);

    


    return 0;
}