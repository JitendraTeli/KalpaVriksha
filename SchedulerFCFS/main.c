#include "DataStructures.h"
#include "SchedularFCFS.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Event {
    enum {
        wait,
        kill,
        ready,
        stop,
    } event;
    int id;
} Event;

HashMap *Events = NULL;

void addEvent(Event *event,int time) {
    put(Events,time,event);
}




int main() {
    Events = makeMap(8);


    return 0;
}