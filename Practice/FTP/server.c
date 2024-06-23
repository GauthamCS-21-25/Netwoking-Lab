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
	
	if((serverfd=socket(AF_INET, SOCK_STREAM, 0))==0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
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
	if(listen(serverfd, 3)<0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if((newsocket=accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&addrlen))<0){
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	int n;
	
	printf("Server is running...\n");
	
	recv(newsocket, buffer, BUFFERSIZE, 0);
	
	char filename[BUFFERSIZE];
	strcpy(filename, buffer);
	
	filename[strlen(filename)-1]='\0';
	
	FILE *fp=fopen(filename, "r");
	
	printf("Contents copied are: ");
	while(fgets(buffer, BUFFERSIZE, fp)!=NULL){
		printf("%s", buffer);
		send(newsocket, buffer, sizeof(buffer), 0);
	}
	
	fclose(fp);
	close(newsocket);
}
