#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;
    printf("Server listening on port %d...\n", PORT);
    len = sizeof(cliaddr);
    while (1) {
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Message from client: %s\n", buffer);
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Client requested to exit. Closing connection.\n");
            break;
        }
        
        sendto(sockfd, (const char *)"Message received", strlen("Message received"), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Response sent to client.\n");
    }

    close(sockfd);
    return 0;
}

