#include<stdio.h>

typedef struct Node {
    int key;
    char *token;
    struct Node *next;
    struct NOde *prev;
} Node;

Node *head = NULL;
Node *tail = NULL;


Node* generateMap(int capacity) {
    Node* map = (Node *) calloc(capacity,sizeof(Node));


    head = map;
    tail = map + capacity - 1;

    map[0].prev = NULL;
    map[0].next = map + 1;

    map[capacity-1].prev = map + capacity - 2;
    map[capacity-1].next = NULL;
    

    for(int i = 1; i<capacity-1; i++) {
        map[i].next = map + i + 1;
        map[i].prev = map + i - 1;
    }

    return map;
}


int main() {

    

    return 0;
}