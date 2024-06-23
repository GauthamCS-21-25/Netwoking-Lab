#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void main() {
	int serverfd=0;
	struct sockaddr_in saddr, caddr;
	char buffer[BUFFERSIZE]={0};
	
	if((serverfd=socket(AF_INET, SOCK_DGRAM, 0))<=0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&saddr, 0, sizeof(saddr));
	memset(&caddr, 0, sizeof(caddr));
	
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=INADDR_ANY;
	saddr.sin_port=htons(PORT);
	
	if(bind(serverfd, (struct sockaddr *)&saddr, sizeof(saddr))<0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	printf("Server is running...\n");
	
	int n, len=sizeof(caddr);
	char mail[100]={0}, rcpt[100]={0}, data[100]={0};
	
	n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr*)&caddr, &len);
	
	if(strcmp(buffer, "HELO\n")==0){
		printf("Connected to the client...\n");
		
		while(1){
			memset(buffer, 0, BUFFERSIZE);
			n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr*)&caddr, &len);
			buffer[n]='\0';
			
			if(strcmp(buffer, "MAIL_FROM\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr*)&caddr, &len);
				buffer[n]='\0';
				sscanf(buffer, "%s", mail);
			} else if(strcmp(buffer, "RCPT_TO\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr*)&caddr, &len);
				buffer[n]='\0';
				sscanf(buffer, "%s", rcpt);
			} else if(strcmp(buffer, "DATA\n")==0){
				memset(buffer, 0, BUFFERSIZE);
				n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr*)&caddr, &len);
				buffer[n]='\0';
				strcpy(data, buffer);
			} else if(strcmp(buffer, "QUIT\n")==0){
				printf("Exiting...\n");
				break;
			} else
				printf("Invalid command. Try again...\n");
				
			if(strcmp(mail,"")!=0 && strcmp(rcpt,"")!=0 && strcmp(data,"")!=0)
				printf("Mail id:%s\nRecipient id:%s\nBody:%s\n\n", mail, rcpt, data);
		}
	}
}
