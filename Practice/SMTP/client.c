#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void main() {
	int clientfd;
	struct sockaddr_in saddr;
	char buffer[BUFFERSIZE]={0};
	
	if((clientfd=socket(AF_INET, SOCK_DGRAM, 0))<=0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&saddr, 0, sizeof(saddr));
	
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=INADDR_ANY;
	saddr.sin_port=htons(PORT);
	
	printf("Client is running...\n");
	
	int n, len=sizeof(saddr);
	
	printf("Enter HELO to start mail: ");
	fgets(buffer, BUFFERSIZE, stdin);
	sendto(clientfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
	
	if(strcmp(buffer, "HELO\n")==0){
		printf("Connected to the server...\n");
		while(1){
			printf("1. MAIL_FROM\t2. RCPT_TO\t3. DATA\t4. QUIT\n");
			memset(buffer, 0, BUFFERSIZE);
			printf("Enter the command: ");
			fgets(buffer, BUFFERSIZE, stdin);
			sendto(clientfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
			
			if(strcmp(buffer, "MAIL_FROM\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				printf("Enter your mail id: ");
				fgets(buffer, BUFFERSIZE, stdin);
				sendto(clientfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
			} else if(strcmp(buffer, "RCPT_TO\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				printf("Enter recipient mail id: ");
				fgets(buffer, BUFFERSIZE, stdin);
				sendto(clientfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
			} else if(strcmp(buffer, "DATA\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				printf("Enter the body: ");
				fgets(buffer, BUFFERSIZE, stdin);
				sendto(clientfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
			} else if(strcmp(buffer, "QUIT\n")==0){
				printf("Exiting...\n");
				sendto(clientfd, "EXIT", strlen("EXIT"), MSG_CONFIRM, (struct sockaddr *)&saddr, len);
				break;
			} else
				printf("Invalid command. Try again...\n");
		}
	}
}
