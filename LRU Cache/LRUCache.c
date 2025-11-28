
#include "LRUCache.h"
#include <stdio.h>
#include <stdlib.h>

void access(HashNode *temp,Cache *cache) {
    if(cache->head == NULL) {
        cache->tail =  cache->head = temp;
    } else if(temp != cache->head) {

        if(temp == cache->tail) cache->tail = temp->prev;
        
        if(temp->next) temp->next->prev = temp->prev;
        if(temp->prev) temp->prev->next = temp->next;

        temp->next = cache->head;
        cache->head->prev = temp;

        temp->prev = NULL;

        cache->head = temp;  
    }
}

int hash(int key,int size) {
    return key%size;
}

void pop(Cache *cache) {

    int index = hash(cache->tail->key,cache->size);
    HashNode *temp = cache->HashMap[index];

    if(temp == cache->tail) {
        cache->HashMap[index] = temp->child;
    } else {
        while(temp->child != cache->tail && temp != NULL) temp = temp->child;
        temp->child = temp->child->child;
    }

    cache->tail = cache->tail->prev;

    free(cache->tail->next);

    cache->tail->next = NULL;

    cache->nodeCount--;
}

HashNode** generateMap(int capacity) {
    HashNode **map = (HashNode **) calloc(capacity,sizeof(HashNode*));

    for(int i = 0; i<capacity; i++) map[i] = NULL;

    return map;
}

HashNode* search(HashNode *start,int key) {
    while(start != NULL) {
        if(start->key == key ) break;
        start = start->child;
    }
    return start;
}

void put(int key,char *token,Cache *cache) {

    int index = hash(key,cache->size);

    HashNode *temp = search(cache->HashMap[index],key);

    if(temp) {
        temp->token = token;
    } else {
        if(cache->nodeCount == cache->size) pop(cache);

        temp = (HashNode *) malloc(sizeof(HashNode));
        if(temp) {
            cache->nodeCount++;
            temp->key = key;
            temp->token = token;
            temp->child = cache->HashMap[index];
            cache->HashMap[index] = temp;

            temp->next = NULL;
            temp->prev = NULL;
        } else {
            printf("cout memory allocation failed :( ");
            return;
        }
    }
    access(temp,cache);
}

char* get(int key,Cache *cache) {
    HashNode *result = search(cache->HashMap[hash(key,cache->size)],key);

    if(result) return result->token;

    return "null";
}
