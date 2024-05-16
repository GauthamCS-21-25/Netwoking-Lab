#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to localhost
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Receive messages from the client
    for (int i = 0; i < 100; ++i) {
        int recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        printf("Message received from client: %s\n", buffer);

        // Send ACK back to client
        sendto(server_socket, "ACK\n", 4, 0, (struct sockaddr *)&client_addr, addr_len);
        printf("ACK sent for %s\n\n", buffer);

        memset(buffer, 0, BUFFER_SIZE); // Reset buffer
    }

    close(server_socket);
    return 0;
}

