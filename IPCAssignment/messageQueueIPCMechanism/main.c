#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/msg.h>

size_t gSize = 0;

#define MAX_SIZE 1024

struct msgbuf {
    long mtype;
    size_t size;
    int data[MAX_SIZE];
};

int* inputArrayFromCLI() {
    printf("Enter the size of Array : ");
    scanf("%zu",&gSize);

    int *array = calloc(gSize,sizeof(int));

    printf("\n Enter %zu elements : ",gSize);
    for(size_t i = 0; i<gSize; i++)
        scanf("%d", array + i);

    return array;
}

void printArray(int *array) {
    printf("\nPrinting the array of Size %zu :- \n -: ", gSize);
    for(size_t i = 0; i<gSize; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int comarator(const void *a, const void *b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

int main() {

    int *nums = inputArrayFromCLI();

    printf("\nBefore Sorting");
    printArray(nums);

    /* -------- System V Message Queue -------- */
    int msqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pid_t id = fork();
    if (id < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (id == 0) {
        /* ------------ CHILD ------------ */
        qsort(nums, gSize, sizeof(int), comarator);

        struct msgbuf msg;
        msg.mtype = 1;
        msg.size = gSize;
        memcpy(msg.data, nums, gSize * sizeof(int));

        msgsnd(msqid, &msg,
               sizeof(msg) - sizeof(long),
               0);

        exit(0);

    } else {
        /* ------------ PARENT ------------ */
        waitpid(id, NULL, 0);

        struct msgbuf msg;
        msgrcv(msqid, &msg,
               sizeof(msg) - sizeof(long),
               1, 0);

        gSize = msg.size;
        memcpy(nums, msg.data, gSize * sizeof(int));

        msgctl(msqid, IPC_RMID, NULL);

        printf("\nAfter Sorting");
        printArray(nums);
    }

    free(nums);
    return 0;
}
