#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_THREADS 4
#define QUEUE_SIZE 10
#define DB_FILE "../resource/accountDB.txt"

#define STATUS_OK                 0
#define STATUS_INSUFFICIENT_FUNDS 1


typedef enum {
    OP_WITHDRAW = 1,
    OP_DEPOSIT,
    OP_BALANCE
} Operation;

typedef struct {
    int userId;
    Operation op;
    int amount;
} Request;

/* ---------- Task Queue ---------- */

typedef struct {
    int queue[QUEUE_SIZE];
    int front, rear, count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} task_queue;

task_queue tq;
pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

/* ---------- Queue Ops ---------- */

void queue_init(task_queue *tq) {
    tq->front = tq->rear = tq->count = 0;
    pthread_mutex_init(&tq->mutex, NULL);
    pthread_cond_init(&tq->cond, NULL);
}

void queue_push(task_queue *tq, int fd) {
    pthread_mutex_lock(&tq->mutex);
    tq->queue[tq->rear] = fd;
    tq->rear = (tq->rear + 1) % QUEUE_SIZE;
    tq->count++;
    pthread_cond_signal(&tq->cond);
    pthread_mutex_unlock(&tq->mutex);
}

int queue_pop(task_queue *tq) {
    pthread_mutex_lock(&tq->mutex);
    while (tq->count == 0)
        pthread_cond_wait(&tq->cond, &tq->mutex);

    int fd = tq->queue[tq->front];
    tq->front = (tq->front + 1) % QUEUE_SIZE;
    tq->count--;
    pthread_mutex_unlock(&tq->mutex);
    return fd;
}

int process_request(Request *req, int *result) {
    FILE *fp;
    int id, balance;
    int found = 0;
    int status = STATUS_OK;

    pthread_mutex_lock(&db_mutex);

    fp = fopen(DB_FILE, "r");
    if (!fp) {
        fp = fopen(DB_FILE, "w");
        fclose(fp);
        fp = fopen(DB_FILE, "r");
    }

    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d %d", &id, &balance) != EOF) {
        if (id == req->userId) {
            found = 1;

            if (req->op == OP_DEPOSIT) {
                balance += req->amount;
            }
            else if (req->op == OP_WITHDRAW) {
                if (req->amount > balance) {
                    status = STATUS_INSUFFICIENT_FUNDS;
                    /* balance unchanged */
                } else {
                    balance -= req->amount;
                }
            }

            *result = balance;
        }
        fprintf(temp, "%d %d\n", id, balance);
    }

    /* New account case */
    if (!found) {
        int new_balance = 0;

        if (req->op == OP_DEPOSIT) {
            new_balance = req->amount;
        }
        else if (req->op == OP_WITHDRAW) {
            status = STATUS_INSUFFICIENT_FUNDS;
        }

        fprintf(temp, "%d %d\n", req->userId, new_balance);
        *result = new_balance;
    }

    fclose(fp);
    fclose(temp);
    rename("temp.txt", DB_FILE);

    pthread_mutex_unlock(&db_mutex);
    return status;
}

/* ---------- Worker Thread ---------- */

void *worker(void *arg) {
    (void)arg;

    while (1) {
        int client_fd = queue_pop(&tq);

        Request req;
        int balance = 0;

        read(client_fd, &req, sizeof(req));

        int status = process_request(&req, &balance);

        write(client_fd, &status, sizeof(status));
        write(client_fd, &balance, sizeof(balance));

        close(client_fd);
    }
}


/* ---------- Main ---------- */

int main() {
    int server_fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    queue_init(&tq);

    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    printf("Server running on port %d\n", PORT);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *)&addr, &len);
        queue_push(&tq, client_fd);
    }
}
