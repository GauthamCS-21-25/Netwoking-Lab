//SMTP Client

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
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connection successful.\n");

    char command[100];
    char sender_mail_addr[100] = {0};
    char recipient_mail_addr[100] = {0};
    char body[200] = {0};

    printf("Enter HELO command to start communication with server: ");
    fgets(command, sizeof(command), stdin);
    write(sockfd, command, sizeof(command));
    if (strncmp(command, "HELO", 4) == 0) {
        printf("HELO command sent. Communication established.\n");
    }

    while (1) {
        printf("\n1. MAIL FROM - Define sender mail address\n");
        printf("2. RCPT TO - Define receiver's mail address\n");
        printf("3. DATA - Define body of the mail\n");
        printf("4. QUIT - Terminate connection\n");
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);
        write(sockfd, command, sizeof(command));

        if (strncmp(command, "MAIL FROM:", 10) == 0) {
            printf("Enter sender's mail address: ");
            fgets(sender_mail_addr, sizeof(sender_mail_addr), stdin);
            write(sockfd, sender_mail_addr, sizeof(sender_mail_addr));
        } else if (strncmp(command, "RCPT TO:", 8) == 0) {
            printf("Enter receiver's mail address: ");
            fgets(recipient_mail_addr, sizeof(recipient_mail_addr), stdin);
            write(sockfd, recipient_mail_addr, sizeof(recipient_mail_addr));
        } else if (strncmp(command, "DATA", 4) == 0) {
            printf("Enter mail body: ");
            fgets(body, sizeof(body), stdin);
            write(sockfd, body, sizeof(body));
        } else if (strncmp(command, "QUIT", 4) == 0) {
            printf("Terminating connection.\n");
            break;
        } else {
            printf("Invalid command.\n");
        }

        if (strlen(sender_mail_addr) > 0 && strlen(recipient_mail_addr) > 0 && strlen(body) > 0) {
            printf("Mail sent.\n");
            memset(sender_mail_addr, 0, sizeof(sender_mail_addr));
            memset(recipient_mail_addr, 0, sizeof(recipient_mail_addr));
            memset(body, 0, sizeof(body));
        }
    }

    close(sockfd);
    return 0;
}

