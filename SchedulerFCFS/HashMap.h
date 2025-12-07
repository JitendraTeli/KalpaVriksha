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

int hash(int key,int size) {
    key = key ^ (key >> (sizeof(key)*4));
    return (key & size);
}



#endif
