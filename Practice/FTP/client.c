#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void main(){
	int clientfd;
	struct sockaddr_in address;
	char buffer[BUFFERSIZE]={0};
  
	if((clientfd=socket(AF_INET, SOCK_STREAM, 0))==0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
  
	address.sin_family=AF_INET;
	address.sin_port=htons(PORT);
  
	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0){
  		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	if(connect(clientfd, (struct sockaddr *)&address, sizeof(address))<0){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	
	printf("Client is running...\n");
	
	printf("Enter the filename: ");
	fgets(buffer, BUFFERSIZE, stdin);
	send(clientfd, buffer, sizeof(buffer), 0);
	
	FILE *fp=fopen("copy.txt", "w");
	
	while(recv(clientfd, buffer, BUFFERSIZE, 0)>0){
		fprintf(fp, "%s", buffer);
	}
	
	fclose(fp);
	close(clientfd);
  
}
