#include<stdio.h>

typedef struct Node {
    int key;

    struct Node *next;
    struct NOde *prev;
} Node;

typedef struct HashNode {
    int key;
    char* token;

    struct HashNode *next;
    struct HashNode *prev;
    struct HashNode *child;

} HashNode;


Node *head = NULL;
Node *tail = NULL;

HashNode **HashMap = NULL;

void access(int key) {
    
}

int hash(int key) {

    return key;
}

HashNode** generateMap(int capacity) {
    HashNode **map = (HashNode **) calloc(capacity,sizeof(HashNode*));

    for(int i = 0; i<capacity; i++) map[i] = NULL;

    return map;
}



void put(int key,char *token) {
    int index = hash(key);

    HashNode *temp = HashMap[index];

    if(temp == NULL) {
        HashMap[index] = (HashNode *) malloc(sizeof(HashNode));
        HashMap[index]->key = key;
        HashMap[index]->token = token;
    } else {
        while(temp->next != NULL) {
            
            if(temp->key == key) {
                temp->token = token;
                return ;
            }

            temp = temp->next;
        }

        temp->next = (HashNode *) malloc(sizeof(HashNode));
        temp->next->key = key;
        temp->next->token = token;
    }
}

HashNode* get(int key) {
    int index = hash(key);

    HashNode *temp = HashMap[index];
    
    while(temp != NULL) {
        if(temp->key == key) {
            access(key);
            break;
        }
        temp = temp->next;
    }

    return temp;
}


int main() {

    

    return 0;
}