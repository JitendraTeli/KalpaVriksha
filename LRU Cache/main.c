#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "LRUCache.h"

#define TOKEN_LENGTH 20


void handlePut(Cache *cache,int key) {
    char *token = (char *) malloc(TOKEN_LENGTH*sizeof(char));

    scanf("%19s",token);

    put(key,token,cache);
}

void handleGet(Cache *cache,int key) {
    printf("%s\n",get(key,cache));
}

void clearBuffer() {
    int thing; 

    while((thing = getchar() != '\n' && thing != EOF) );
}

void startInput(Cache *cache) {
    char command[10];
    int key;
    while(1) {
        scanf("%9s",command);
        if(strcmp(command,"exit") == 0) break;

        scanf("%d",&key);

        if(strcmp(command,"get") == 0) handleGet(cache,key);
        else if(strcmp(command,"put") == 0) handlePut(cache,key);
        else printf("invalid input\n");
        clearBuffer();
    }
}


int main() {

    Cache cache = {NULL,NULL,NULL,0,0};

    int capacity = 0;

    scanf("%d",&capacity);
    
    cache.HashMap = generateMap(capacity);
    cache.size = capacity;

    startInput(&cache);

}