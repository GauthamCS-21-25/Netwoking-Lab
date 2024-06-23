#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void main() {
  int serverfd, newsocket, opt=1;
  struct sockaddr_in address;
  int addrlen=sizeof(address);
  char buffer[BUFFERSIZE]={0};
  
  if((serverfd=socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  
  if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  
  address.sin_family=AF_INET;
  address.sin_addr.s_addr=INADDR_ANY;
  address.sin_port=htons(PORT);
  
  if(bind(serverfd, (struct sockaddr *)&address, sizeof(address))<0){
    perror("bind");
    exit(EXIT_FAILURE);
  }
  
  if(listen(serverfd,3)<0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  
  if((newsocket=accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
  printf("Server is running...\nConnected to the client...\n");
  
  while(1){
    memset(&buffer, 0, BUFFERSIZE);
    printf("\nWaiting for message...\n");
    if(recv(newsocket, buffer, BUFFERSIZE, 0) < 0){
      perror("recv");
      exit(EXIT_FAILURE);
    }
    
    if(strcmp(buffer, "EXIT\n")==0){
      printf("Client initiated exit. Exiting...\n");
      break;
    }
    
    printf("Message received: %s\n", buffer);
    memset(&buffer, 0, BUFFERSIZE);
    printf("Enter the message to be sent: ");
    fgets(buffer, BUFFERSIZE, stdin);
    send(newsocket, buffer, sizeof(buffer), 0);
  }
  
  close(newsocket);
}
