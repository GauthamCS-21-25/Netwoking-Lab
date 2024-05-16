#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 1

// Function to generate a random number between min and max (inclusive)
int generate_random_number(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    srand(time(NULL)); // Initialize random number generator

    // Create client socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Send 20 messages to the server
    for (int i = 0; i < 10; ++i) {
        sprintf(buffer, "Message %d", i+1);
        if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        printf("Message %d sent to server: %s\n", i+1, buffer);

        // Set timeout for receiving acknowledgment
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        // Decide whether to introduce timeout for this message randomly
        if (generate_random_number(0, 1)) {
            printf("Timeout simulated for message %d. Retransmitting...\n", i+1);
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            printf("Retransmitted\n");
            continue;
        }

        // Wait for acknowledgment from server or timeout
        int select_result = select(client_socket + 1, &readfds, NULL, NULL, &timeout);
        if (select_result == -1) {
            perror("Error in select");
            exit(EXIT_FAILURE);
        } else if (select_result == 0) {
            printf("Timeout: acknowledgment not received. Retransmitting message %d...\n", i+1);
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            printf("Retransmitted\n\n");
            continue; // Retry sending the message
        } else {
            // Acknowledgment received from server
            memset(buffer, 0, BUFFER_SIZE);
            int ack_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
            if (ack_received > 0 && strcmp(buffer, "ACK\n") == 0) {
                printf("Received acknowledgment from server for message %d: %s\n", i+1, buffer);
            } else {
                printf("Invalid acknowledgment received for message %d.\n", i+1);
            }
        }
    }

    close(client_socket);
    return 0;
}

