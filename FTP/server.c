//FTP Server

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

void main() {
    struct sockaddr_in clientaddr, serveraddr;
    int serversock, newserversock, clientsize, n, f, rc;
    char filename[100], filedata[300];
    int output_fd; // File descriptor for output file

    // Clear stdin buffer
    fflush(stdin);

    // Create socket
    serversock = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4040);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket
    if (bind(serversock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Listen for connections
    listen(serversock, 5);

    while (1) {
        clientsize = sizeof(clientaddr);
        newserversock = accept(serversock, (struct sockaddr *) &clientaddr, &clientsize);
        if (newserversock < 0) {
            perror("Error accepting connection");
            exit(1);
        }

        // Read filename from client
        n = read(newserversock, filename, 100);
        filename[n] = 0;
        printf("\nThe requested file from the client is %s.\n", filename);

        // Open the requested file
        f = open(filename, O_RDONLY);
        if (f < 0) {
            perror("Error opening file");
            close(newserversock);
            continue;
        }

        // Read file data
        n = read(f, filedata, sizeof(filedata));
        if (n < 0) {
            perror("Error reading file");
            close(f);
            close(newserversock);
            continue;
        }
        close(f);

        // Print file contents to server console (optional)
        printf("\nThe contents of the file: \n\n%s", filedata);

        // Send file data to client
        write(newserversock, filedata, n);

        // Save received data to output.txt
        output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (output_fd < 0) {
            perror("Error opening output file");
            close(newserversock);
            continue;
        }

        if (write(output_fd, filedata, n) < 0) {
            perror("Error writing to output file");
        }

        close(output_fd);
        close(newserversock);
    }

    close(serversock);
}

