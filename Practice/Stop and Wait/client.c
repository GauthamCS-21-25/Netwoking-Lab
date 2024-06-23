#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

int main() {
    int clientfd;
    struct sockaddr_in address;
    char buffer[BUFFERSIZE]={0};
    
    if((clientfd=socket(AF_INET, SOCK_STREAM, 0))<0){
    	perror("socket");
      	exit(EXIT_FAILURE);
    }
    
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    
    if((inet_pton(AF_INET, "127.0.0.1", &address.sin_addr))<=0){
    	perror("inet");
      	exit(EXIT_FAILURE);
    }
    
    if(connect(clientfd, (struct sockaddr *)&address, sizeof(address))){
    	perror("connect");
      	exit(EXIT_FAILURE);
    }
    
    printf("Client is running...\nConnected to the server...\n");
    
    int i=0, np=10; //no of packets
    
    while(i < np){
    	memset(buffer, 0, BUFFERSIZE);
    	sprintf(buffer, "Message_%d", i);
    	send(clientfd, buffer, BUFFERSIZE, 0);
    	printf("\nWaiting ACK for %s...\n", buffer);
    	
    	memset(buffer, 0, BUFFERSIZE);
    	recv(clientfd, buffer, BUFFERSIZE, 0);
    	if(strlen(buffer)==1)
    		printf("Timeout. Retransmitting...\n");
    	else{
    		printf("%s received.\n", buffer);
    		i++;
    	}
    	
    }
    
    send(clientfd, "EXIT", strlen("EXIT"), 0);
    
    close(clientfd);
    return 0;
}

