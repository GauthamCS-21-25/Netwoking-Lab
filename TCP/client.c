//client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");
    while(1) {
        printf("Enter message to send to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);
        printf("Message sent\n");
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Exiting...\n");
            break;
        }

        printf("Waiting for response from server...\n");
        if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        printf("Received message from server: %s\n", buffer);
    }
    // Close the connection
    close(sock);
    return 0;
}

