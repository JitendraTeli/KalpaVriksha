#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define STATUS_OK                 0
#define STATUS_INSUFFICIENT_FUNDS 1

#define PORT 8080

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


Operation parse_operation(char *op) {
    if (strcmp(op, "withdraw") == 0) return OP_WITHDRAW;
    if (strcmp(op, "deposit") == 0)  return OP_DEPOSIT;
    if (strcmp(op, "balance") == 0)  return OP_BALANCE;
    return -1;
}

int main() {
    int sock;
    struct sockaddr_in server;
    Request req;
    char opStr[20];

    printf("Enter: userId operation amount\n");
    printf("Example: 1 withdraw 500\n");

    while (1) {
        printf("> ");

        if (scanf("%d %s", &req.userId, opStr) != 2) {
            scanf("%s",opStr);
            if (strcmp(opStr, "exit") == 0) break;
            else {
                printf("invalid Input");
                continue;
            }
        }
        req.op = parse_operation(opStr);

        if(req.op == -1) {
            printf("invalid operation: allowed oprations are deposit, withdraw, balance");
            continue;
        }
        if (req.op == OP_BALANCE) req.amount = 0;
        else scanf("%d", &req.amount);


        int sock = socket(AF_INET, SOCK_STREAM, 0);

        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

        connect(sock, (struct sockaddr *)&server, sizeof(server));

        write(sock, &req, sizeof(req));

        int status, balance;

        read(sock, &status, sizeof(status));
        read(sock, &balance, sizeof(balance));

        if (status == STATUS_OK) {
            printf("Transaction successful.\n");
            printf("Current Balance: %d\n", balance);
        }
        else if (status == STATUS_INSUFFICIENT_FUNDS) {
            printf("Transaction failed: Insufficient funds.\n");
            printf("Current Balance: %d\n", balance);
        }
        else {
            printf("Unknown error.\n");
        }

                close(sock);
            }

             
    return 0;
}
