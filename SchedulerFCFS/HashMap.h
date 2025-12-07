#ifndef HASH_MAP_H

#define HASH_MAP_H



typedef struct HashNode {
    int key;
    void *value;

    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    HashNode **map;
    int size;
    int capacity;
} HashMap;

int hash(int key,int size);

float sizeFactor(int capacity,int size);

HashMap* makeMap(int size);

void insert(HashNode **head,HashNode *node);

void reshape(HashMap *Map);

void put(HashMap *Map,int key,void *val);

void* get(HashMap *Map,int key);

#endif
