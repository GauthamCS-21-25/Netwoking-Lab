#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void main(){
  int sock=0;
  struct sockaddr_in address;
  char buffer[BUFFERSIZE]={0};
  
  if((sock=socket(AF_INET, SOCK_STREAM, 0)) <0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  
  address.sin_family=AF_INET;
  address.sin_port=htons(PORT);
  
  if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0){
    perror("inet_ptons");
    exit(EXIT_FAILURE);
  }
  
  if(connect(sock, (struct sockaddr *)&address, sizeof(address))<0){
    perror("connect");
    exit(EXIT_FAILURE);
  }
  
  printf("Client is running...\nConnected to the server...\n");
  
  while(1){
    printf("Enter the message to be sent: ");
    fgets(buffer, BUFFERSIZE, stdin);
    send(sock, buffer, sizeof(buffer), 0);
    
    if(strcmp(buffer, "EXIT\n")==0){
      printf("Exiting...\n");
      break;
    }
    
    memset(&buffer, 0, BUFFERSIZE);
    if(recv(sock, buffer, BUFFERSIZE, 0) < 0){
      perror("recv");
      exit(EXIT_FAILURE);
    }
    printf("Message received: %s\n", buffer);
    memset(&buffer, 0, BUFFERSIZE);
    
  }
  
  close(sock);
}
