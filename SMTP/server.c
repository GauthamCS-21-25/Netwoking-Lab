//SMTP Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SA struct sockaddr
#define PORT 8080

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Bind successful.\n");

    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Listen successful.\n");

    len = sizeof(cliaddr);
    connfd = accept(sockfd, (SA *)&cliaddr, &len);
    if (connfd < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connection successful.\n");

    char command[100];
    char sender_mail_addr[100] = {0};
    char recipient_mail_addr[100] = {0};
    char body[200] = {0};

    printf("Waiting for HELO command from client...\n");
    read(connfd, command, sizeof(command));
    if (strncmp(command, "HELO", 4) == 0) {
        printf("HELO command received. Communication established.\n");
    }

    while (1) {
        memset(command, 0, sizeof(command));
        read(connfd, command, sizeof(command));
        if (strncmp(command, "MAIL FROM:", 10) == 0) {
            read(connfd, sender_mail_addr, sizeof(sender_mail_addr));
            printf("MAIL FROM: %s", sender_mail_addr);
        } else if (strncmp(command, "RCPT TO:", 8) == 0) {
            read(connfd, recipient_mail_addr, sizeof(recipient_mail_addr));
            printf("RCPT TO: %s", recipient_mail_addr);
        } else if (strncmp(command, "DATA", 4) == 0) {
            read(connfd, body, sizeof(body));
            printf("DATA received.\n");
        } else if (strncmp(command, "QUIT", 4) == 0) {
            printf("Client requested to terminate the connection.\n");
            break;
        } else {
            printf("Invalid command: %s", command);
        }

        if (strlen(sender_mail_addr) > 0 && strlen(recipient_mail_addr) > 0 && strlen(body) > 0) {
            printf("\nMail received.\n");
            printf("Sender mail address: %s", sender_mail_addr);
            printf("Recipient mail address: %s", recipient_mail_addr);
            printf("Body:\n%s\n", body);

            memset(sender_mail_addr, 0, sizeof(sender_mail_addr));
            memset(recipient_mail_addr, 0, sizeof(recipient_mail_addr));
            memset(body, 0, sizeof(body));
        }
    }

    close(connfd);
    close(sockfd);
    return 0;
}

