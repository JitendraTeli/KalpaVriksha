#ifndef Required_Sorting_H

#define Requrired_Sorting_H

#include "Team.h"
#include "Player.h"

void swapPlayers(Player **greater, Player **lesser);

//recursive
void heapifyDown(Player **heap,int size,int rootIndex);

//iterative
void heapifyUp(Player **heap,int index);

void swapIntegers(int *lesser,int *greater);

int partition(int *array,int low,int high, int (*compare)(int,int));

void quickSort(int *array,int low,int high,int (*compare)(int,int));





#endif