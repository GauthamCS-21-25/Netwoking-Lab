#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc,char *argv[]) {

    int source_fd = open(argv[1], O_RDONLY);

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    char buffer[BUF_SIZE];
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(source_fd, buffer, sizeof(buffer))) > 0)
        bytesWritten = write(dest_fd, buffer, bytesRead);

    close(source_fd);
    close(dest_fd);

    printf("File copied successfully from %s to %s\n", argv[1], argv[2]);

    return EXIT_SUCCESS;
}
