#ifndef SCHEDULAR_FCFS_H

#define SCHEDULAR_FCFS_H

#include "DataStructures.h"

typedef struct Process {
    char *name;
    int id;
    int time;
    int startIO;
    int timeIO;

    int totaltimeIO;

} Process;

typedef struct Schedular {
    HashMap processes;
    List *startIO;
    List *ready;
    List *terminated;
} Schedular;

void addProcess(Schedular *fcfs,Process *process);



#endif