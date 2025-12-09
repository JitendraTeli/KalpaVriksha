
#include "DataStructures.h"

#include<stdio.h>
#include<stdlib.h>

#define FACTOR 7.5

int hash(int key,int size) {
    key = key ^ (key >> (sizeof(key)*4));
    return (key & size);
}

float sizeFactor(int capacity,int size) {
    return (float) size / (float) capacity;
}

HashMap* makeMap(int size) {
    HashMap *map = malloc(sizeof(HashMap));
    map->map = (HashNode ** ) malloc(size * sizeof(HashNode *));
    map->capacity = size;
    map->size = 0;
    return map;
}

void insert(HashNode **head,HashNode *node) {
    node->next = NULL;
    if(*head == NULL) {
        *head = node;
    } else {
        HashNode *temp = *head;
        while(temp && temp->next ) {
            temp = temp->next;
        }
        temp->next = node;
    }
}

void reshape(HashMap *Map) {   
    HashNode **oldMap = Map->map; 
    int oldCapacity = Map->capacity;

    Map->map = malloc(2 * Map->capacity * sizeof(HashNode*));

    for(int i = 0; i<Map->capacity; i++) {
        HashNode *temp = oldMap[i];

        while(temp) {
            HashNode *node = temp;
            temp = temp->next;

            insert(Map->map + hash(node->key,2 * Map->capacity),node);  
        }
    }
    Map->capacity *= 2;
}

void put(HashMap *Map,int key,void *val) {
    int index = hash(key,Map->capacity);

    HashNode *temp = Map->map[index];

    while(temp && temp->next) {
        if(temp->key == key)  {
            temp->value = val;
            return;
        }

        temp = temp->next;
    }

    HashNode *newNode = malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = val;
    newNode->next = NULL;

    if(temp == NULL) {
        Map->map[index] = newNode;
    } else {
        temp->next = newNode;
    }

    if(sizeFactor(Map->capacity,Map->size) >= FACTOR) reshape(Map);
}

void* pop(HashMap *Map,int key) {
    int index = hash(key,Map->capacity);

    HashNode *temp = Map->map[index];
    HashNode *prev = NULL;
    while(temp) {
        if(temp->key == key) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    
    if(temp == NULL) return NULL;

    if(prev) prev->next = temp->next; else Map->map[index] = temp->next;

    void *value = temp->value;

    free(temp);

    return value;
}

void* get(HashMap *Map,int key) {
    HashNode *temp = Map->map[hash(key,Map->capacity)];

    while(temp) {
        if(temp->key == key) {
            return temp->value;
        }

        temp = temp->next;
    }

    return NULL;
}


void enque(List *queue,Node *newNode) {
    if(queue->head == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        newNode->prev = queue->tail;
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}

Node* deque(List *queue) {
    if(queue->head == NULL) return NULL;

    Node *temp = queue->head;
    queue->head = queue->head->next;
    queue->head->prev = NULL;

    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}

void remove(List *queue,Node *node) {
    if(node == queue->head) queue->head = queue->head->next;
    if(node == queue->tail) queue->tail = queue->tail->prev;

    if(node->next) node->next->prev = node->prev;
    if(node->prev) node->prev->next = node->next;

    node->next = NULL;
    node->prev = NULL;
} 