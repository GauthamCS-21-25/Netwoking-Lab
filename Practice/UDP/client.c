#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

int main() {
    int clientfd=0;
    struct sockaddr_in saddr; //server_address
    char buffer[BUFFERSIZE]={0};
    
    if((clientfd=socket(AF_INET, SOCK_DGRAM, 0))<0){
      perror("socket");
      exit(EXIT_FAILURE);
    }
    
    memset(&saddr, 0, sizeof(saddr));
    
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port=htons(PORT);
    
    printf("Client is running...\n");
    
    int n, len=sizeof(saddr);
    
    while(1){
      printf("\nEnter the message to be sent: ");
      fgets(buffer, BUFFERSIZE, stdin);
      sendto(clientfd, buffer, strlen(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
      recvfrom(clientfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr *)&saddr, &len);
      printf("Acknowledgement received: %s\n", buffer);
    }
    close(clientfd);
    return 0;
}

