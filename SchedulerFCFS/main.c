#include "DataStructures.h"
#include "SchedularFCFS.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAME_LENGTH 20
#define BUFFER_LENGTH 1024

typedef enum type {
    wait,
    kill,
    ready,
    stop,
} Type;

typedef struct Event {
    Type event;
    int id;
    struct Event *next;
} Event;

HashMap *Events = NULL;

void addEvent(Type event,int id,int time) {
    Event *newEvent = malloc(sizeof(Event));
    newEvent->event = event;
    newEvent->id = id;
    newEvent->next = NULL;

    Event *eventList = get(Events,time);
    if(eventList) eventList->next = newEvent; else eventList = newEvent;

    put(Events,time,eventList);
}

Event* getEvent(int time) {
    return (Event*) get(Events,time);
}


typedef struct Process {
    char *name;
    int id;
    int time;
    int startIO;
    int timeIO;

    Type status;
    int waitingTime;
    int turnAroundTime;

} Process;

typedef struct Schedular {
    HashMap *processes;
    List *waiting;
    List *ready;
    List *terminated;
} Schedular;

Schedular *fcfs;

Process *current = NULL;
int clock = 0;



void commenceInput();



int main() {
    Events = makeMap(8);

    Schedular FCFS = {makeMap(),NULL,NULL,NULL};




    return 0;
}

void addProcess(char *name,int id,int time,int startIO,int timeIO) {
    Process *process = malloc(sizeof(Process));

    process->id = id;
    process->name = strdup(name);
    process->time = time;
    process->startIO = startIO;
    process->timeIO = timeIO;

    process->status = ready;
    process->turnAroundTime = 0;
    process->waitingTime = 0;

    put(fcfs->processes,process->id,process);

}

void commenceInput() {
    char *buffer = malloc(sizeof(char) * BUFFER_LENGTH);

    int id,time,startIO,timeIO;
    char *name = malloc(NAME_LENGTH * sizeof(char));

    while(fgets(buffer,BUFFER_LENGTH-1,stdin) != NULL) {
        startIO = timeIO = -1;  //no io

        int allocations = sscanf(buffer,"%s %d %d %d %d",name,id,time,startIO,timeIO);

        if(allocations < 3) continue;

        if(strcmp(name,"kill") == 0) {
            addEvent(kill,id,time);
        } else {
            addProcess(name,id,time,startIO,timeIO);
        }

    }   
    free(name);
    free(buffer);   
}

void killProcess(Process *process) {
    if(current == process) {
        
    }
}

void handle(Event *event) {
    Event *temp;

    while(event) {
        temp = event;

        if(event->event == kill) {
            killProcess(get(fcfs->processes,event->id));
        } else if(event->event == ready) {

        } else if(event->event == wait) {

        } 

        
        
        event = event->next;
        free(temp);
    }
}

Process* getNextProcess() {
    Process* nextProcess = deque(fcfs->ready);

    if(nextProcess) {
        if(!nextProcess->waitingTime && nextProcess->startIO != -1) {
            addEvent(wait,nextProcess->id,clock + nextProcess->startIO);
        }
    }

    return nextProcess;
}

void beginExecution() {
    while(fcfs->ready || fcfs->waiting) {
        handle((Event *) pop(Events,clock));
        
        if(current && current->turnAroundTime == current->time) {
            enque(fcfs->terminated,get(fcfs->processes,current->id));
            current = NULL;
        }

        if(current == NULL) {
            current = getNextProcess();
        }

        if(current) {
            current->turnAroundTime++;
        }

        sleep(1);
        clock++;
    }
}