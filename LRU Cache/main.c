#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "LRUCache.h"

#define TOKEN_LENGTH 20



void showChildList(HashNode *head) {
    while(head) {
        printf("%d %s _ ",head->key,head->token);
        head = head->child;
    }
}

void showMap(Cache *cache) {
    for(int i = 0; i<cache->size; i++) {
        showChildList(cache->HashMap[i]);
        printf("null \n");
    }
}

void showList(HashNode *head) {
     while(head) {
        printf("%d %s -> ",head->key,head->token);
        head = head->next;
    }
    printf("NULL\n");
}


void handlePut(Cache *cache,int key) {
    char *token = (char *) malloc(TOKEN_LENGTH*sizeof(char));

    scanf("%19s",token);

    put(key,token,cache);

    showMap(cache);

    showList(cache->head);

    printf("tail = %d\n",cache->tail->key);
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