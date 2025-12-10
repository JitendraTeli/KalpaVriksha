#include "DataStructures.h"

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

void addEvent(Type event,int id,int time);

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

void addProcess(char *name,int id,int time,int startIO,int timeIO);

void commenceInput();

void handle(Event *event);

Process* getNextProcess();

void beginExecution();

void printDetails(Process *Process);

void printHeader();

void showOutput();

void showList(List *list) {
    Node *head = list->head; 
    while(head) {
        printf("%s -> ",((Process *) head->value)->name);
        head = head->next;
    }

    printf("\n");
}

void showAllLists() {
    printf("\n All Lists \n");

    printf("R : ");
    showList(fcfs->ready);
    printf("W : ");
    showList(fcfs->waiting);
    printf("T : ");
    showList(fcfs->terminated);
}

int main() {
    Events = makeMap(8);

    Schedular FCFS = {makeMap(8),makeList(),makeList(),makeList()};
    fcfs = &FCFS;

    commenceInput();

    showAllLists();

    beginExecution();

    showAllLists();

    showOutput();

    return 0;
}

void addEvent(Type event,int id,int time) {
    printf("clock %d ",clock);

    showAllLists();

    Event *newEvent = malloc(sizeof(Event));
    newEvent->event = event;
    newEvent->id = id;
    newEvent->next = NULL;

    Event *eventList = get(Events,time);

    if(eventList == NULL) {
        eventList = newEvent;
    } else {
        Event *temp = eventList;

        while(temp->next) temp = temp->next;

        temp->next = newEvent;
    }

    put(Events,time,eventList);
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

    Node *node = malloc(sizeof(Node));

    node->prev = node->next = NULL;
    node->value = process;

    put(fcfs->processes,process->id,node);
    enque(fcfs->ready,node);

}

void commenceInput() {
    char *buffer = malloc(sizeof(char) * BUFFER_LENGTH);

    int id,time,startIO,timeIO;
    char *name = malloc(NAME_LENGTH * sizeof(char));

    while(fgets(buffer,BUFFER_LENGTH-1,stdin) != NULL) {
        startIO = timeIO = -1;  //no io

        int allocations = sscanf(buffer,"%19s %d %d %d %d",name,&id,&time,&startIO,&timeIO);

        if(allocations == -1) break;

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

void handle(Event *event) {
    if(event == NULL) return;
    Event *temp;

    while(event) {
        temp = event;
        
        Node *processNode = get(fcfs->processes,event->id);

        if(!processNode) continue;

        Process *process = processNode->value;

        if(current == processNode->value) current = NULL;
        if(process->status != kill) {

            if(process->status == wait) detach(fcfs->waiting,processNode);
            else if(process->status == ready) detach(fcfs->ready,processNode);
            
            if(event->event == kill) {
                process->status = kill;
                enque(fcfs->terminated,processNode);
    
            } else if(event->event == ready) {
                process->status = ready;
                enque(fcfs->ready,processNode);
    
            } else if(event->event == wait) {
                process->status = wait;

                addEvent(ready,process->id,clock + process->timeIO);
                enque(fcfs->waiting,processNode);
            } 
        }
        event = event->next;
        free(temp);
    }
}

Process* getNextProcess() {
    Process* nextProcess = fcfs->ready->head ? deque(fcfs->ready)->value: NULL;

    if(nextProcess) {
        if(!nextProcess->waitingTime && nextProcess->startIO != -1) {
            addEvent(wait,nextProcess->id,clock + nextProcess->startIO);
        }
        nextProcess->waitingTime = clock - nextProcess->turnAroundTime;
    }

    return nextProcess;
}

void beginExecution() {
    while(fcfs->ready->head || fcfs->waiting->head || current) {

        handle(pop(Events,clock));
        
        if(current && current->turnAroundTime == current->time) {
            current->turnAroundTime += current->waitingTime;
            enque(fcfs->terminated,get(fcfs->processes,current->id));
            current = NULL;
        }

        if(current == NULL) {
            if(current = getNextProcess()) continue;
        }

        if(current) {
            current->turnAroundTime++;
        }
        //sleep(1);
        clock++;
    }
}

void printDetails(Process *Process) {
    printf("%-20s",Process->name);
    printf(" %-5d",Process->id);
    printf(" %-7s",Process->status == kill ? "Killed":"OK");
    printf(" %-10d",Process->time);
    printf(" %-10d",Process->timeIO);
    printf(" %-15d",Process->waitingTime);
    printf(" %d",Process->turnAroundTime);
    printf("\n");
}

void printHeader() {
    printf("%-20s","Name");
    printf(" %-5s","ID");
    printf(" %-7s","Status");
    printf(" %-10s","Burst");
    printf(" %-10s","IO");
    printf(" %-15s","Waiting");
    printf(" %s","Turn Around Time");
    printf("\n");
}

void showOutput() {
    Node *node;

    printHeader();

    while((node = deque(fcfs->terminated))) {
        printDetails(node->value);
        node = NULL;
    }
}