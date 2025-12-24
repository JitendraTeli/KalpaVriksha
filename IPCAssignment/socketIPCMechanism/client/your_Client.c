#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    scanf("%d %s %d", &req.userId, opStr, &req.amount);

    req.op = parse_operation(opStr);
    if (req.op == OP_BALANCE)
        req.amount = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    write(sock, &req, sizeof(req));

    int status, balance;
    read(sock, &status, sizeof(status));

    if (status == 0) {
        read(sock, &balance, sizeof(balance));
        printf("Current Balance: %d\n", balance);
    } else {
        printf("User not found\n");
    }

    close(sock);
    return 0;
}
