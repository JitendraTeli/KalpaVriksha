#include "SchedularFCFS.h"

#include<stdlib.h>


void addProcess(Schedular *fcfs,Process *process) {
    Node *newNode = malloc(sizeof(Node));
    newNode->prev =  newNode->next = NULL;
    newNode->value = process;

    put(fcfs,process->id,newNode);
    enque(fcfs->ready,newNode);
}

