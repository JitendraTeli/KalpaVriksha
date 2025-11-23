#ifndef LRU_HASH_H

#define LRU_HASH_H


typedef struct HashNode {
    int key;
    char* token;

    struct HashNode *next;
    struct HashNode *prev;
    struct HashNode *child;

} HashNode;

typedef struct Cache {
    HashNode *head;
    HashNode *tail;
    HashNode **HashMap;
    int nodeCount;
    int size;
} Cache;


void access(HashNode *temp,Cache *cache);

int hash(int key,int size);

void pop(Cache *cache);

HashNode** generateMap(int capacity);

HashNode* search(HashNode *start,int key);

void put(int key,char *token,Cache *cache);

char* get(int key,Cache *cache);

#endif