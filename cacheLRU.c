#include<stdio.h>

typedef struct HashNode {
    int key;
    char* token;

    struct HashNode *next;
    struct HashNode *prev;
    struct HashNode *child;

} HashNode;


HashNode *head = NULL;
HashNode *tail = NULL;

HashNode **HashMap = NULL;
int maxNodes = 0;

void access(HashNode *temp) {
    if(head == NULL) {
        head = temp;
        tail = head;
    } else if(temp != head) {

        if(temp == tail) tail = temp->prev;
        
        if(temp->next) temp->next->prev = temp->prev;
        if(temp->prev) temp->prev->next = temp->next;

        temp->next = head;
        head->prev = temp;

        temp->prev = NULL;

        head = temp;  
    }
}

void pop() {
    tail = tail->prev;

    free(tail->next);
}

int hash(int key) {

    return key;
}

HashNode** generateMap(int capacity) {
    HashNode **map = (HashNode **) calloc(capacity,sizeof(HashNode*));

    for(int i = 0; i<capacity; i++) map[i] = NULL;

    return map;
}


HashNode* search(HashNode *start,int key) {
    while(start != NULL) {
        if(start->key == key ) break;
        head = start->next;
    }

    return start;
}

void put(int key,char *token) {

    int index = hash(key);

    HashNode *temp = search(HashMap[index],key);

    if(temp) {
        temp->token = token;
    } else {
        temp = (HashNode *) malloc(sizeof(HashNode));
        if(temp) {
            temp->key = key;
            temp->token = token;
            temp->child = HashMap[index];
            HashMap[index] = temp;

            temp->next = NULL;
            temp->prev = NULL;
        } else {
            printf("cout memory allocation failed :( ");
            return;
        }
    }
    access(temp);



    // if(temp == NULL) {
    //     HashMap[index] = (HashNode *) malloc(sizeof(HashNode));
    //     HashMap[index]->key = key;
    //     HashMap[index]->token = token;
    // } else {
    //     while(temp->next != NULL) {
            
    //         if(temp->key == key) {
    //             temp->token = token;
    //             return ;
    //         }

    //         temp = temp->next;
    //     }

    //     temp->next = (HashNode *) malloc(sizeof(HashNode));
    //     temp->next->key = key;
    //     temp->next->token = token;
    // }
}

HashNode* get(int key) {
    return search(HashMap[hash(key)],key);
}


int main() {

    

    return 0;
}