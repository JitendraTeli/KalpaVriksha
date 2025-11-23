#include<stdio.h>
#include<string.h>

#include "RequiredSortingAndSearching.h"



Team* getTeamById(Team *teamArray,int totalTeams,int id) {
    int low = 0;
    int high = totalTeams-1;
    int mid = 0;
    
    while(low <= high) {
        mid = low + (high - low)/2;
        if(teamArray[mid].id == id ) return teamArray + mid;

        if(teamArray[mid].id > id) high = mid-1;
        else low = mid + 1;
    }
    return NULL;
}

Team* getTeamByName(Team *teamArray,char *name,int totalTeams) {
    for(int i = 0; i < totalTeams; i++)
        if(strcmp(teamArray[i].name, name) == 0 ) 
            return teamArray + i;

    return NULL;
}

void swapPlayers(Player **greater, Player **lesser) {
    Player *temp = *greater;
    *greater = *lesser;
    *lesser = temp;
}

//recursive
void heapifyDown(Player **heap,int size,int rootIndex) {
    if(rootIndex >= size) return;

    int leftChild = rootIndex * 2 + 1;
    int rightChild = rootIndex * 2 + 2;
    int largest = rootIndex;

    if(leftChild < size) if(heap[largest]->performanceIndex < heap[leftChild]->performanceIndex) largest = leftChild;

    if(rightChild < size) if( heap[largest]->performanceIndex < heap[rightChild]->performanceIndex) largest = rightChild;

    if(largest != rootIndex) {
        swapPlayers(heap+largest,heap+rootIndex);
        heapifyDown(heap,size,largest);
    } 

}

//iterative
void heapifyUp(Player **heap,int index) {
    while(index > 0) {
        int parent = (index - 1)/2;

        if(heap[parent]->performanceIndex >= heap[index]->performanceIndex) break;
        
        swapPlayers(heap+parent,heap+index);
        index = parent;
    }
}


void swapIntegers(int *lesser,int *greater) {
    int temp = *lesser;
    *lesser = *greater;
    *greater = temp;
}

int partition(int *array,int low,int high, int (*compare)(int,int)) {

    int index = low-1;

    for(int i = low; i<high; i++) {
        if(compare(array[i],array[high])) {
            index++;
            
            swapIntegers(array+index,array+i);
        }
    }
    index++;
    swapIntegers(array+index,array+high);

    return index;
}

void quickSort(int *array,int low,int high,int (*compare)(int,int)) {
    if(low < high) {
        int pivot = partition(array,low,high,compare);

        quickSort(array,low,pivot-1,compare);
        quickSort(array,pivot+1,high,compare);
    }
    
}