#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

size_t gSize = 0;

#define MAX_SIZE 1024

struct shm_data {
    size_t size;
    int data[MAX_SIZE];
};

int* inputArrayFromCLI() {
    printf("Enter the size of Array : ");
    scanf("%zu", &gSize);

    int *array = calloc(gSize, sizeof(int));

    printf("\n Enter %zu elements : ", gSize);
    for (size_t i = 0; i < gSize; i++)
        scanf("%d", array + i);

    return array;
}

void printArray(int *array) {
    printf("\nPrinting the array of Size %zu :- \n -: ", gSize);
    for (size_t i = 0; i < gSize; i++)
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

    /* -------- System V Shared Memory -------- */
    int shmid = shmget(IPC_PRIVATE,
                       sizeof(struct shm_data),
                       0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    struct shm_data *shm =
        (struct shm_data *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
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

        shm->size = gSize;
        memcpy(shm->data, nums, gSize * sizeof(int));

        shmdt(shm);
        exit(0);

    } else {
        /* ------------ PARENT ------------ */
        waitpid(id, NULL, 0);

        gSize = shm->size;
        memcpy(nums, shm->data, gSize * sizeof(int));

        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);

        printf("\nAfter Sorting");
        printArray(nums);
    }

    free(nums);
    return 0;
}
