//server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Server is running\n");
    printf("Connected to client\n");
    while(1) {
        printf("Waiting for message from client...\n");
        if (recv(new_socket, buffer, BUFFER_SIZE, 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        printf("Received message from client: %s\n", buffer);
        
        // Check if client wants to exit
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Client requested to exit. Closing connection.\n");
            break;
        }
        
        printf("Enter message to send to client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
        printf("Message sent to client\n");
    }
    // Close the connection
    close(new_socket);
    return 0;
}

