#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

int main() {
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
		perror("inet_pton");
      	exit(EXIT_FAILURE);
	}
	
	if(connect(clientfd, (struct sockaddr *)&address, sizeof(address))<0){
		perror("connect");
      	exit(EXIT_FAILURE);
	}
	
	printf("Client is running...\nConnected to Server...\n");
	
	int i, start, base=0, frames=10;
	
	while(base < frames){
		start=base; i=0;
		printf("\nSending frames...\n");
		while(i<4 && start<frames){
			sprintf(buffer, "SEQ_%d", start);
			send(clientfd, buffer, sizeof(buffer), 0);
			printf("%s sent.\n", buffer);
			memset(buffer, 0, BUFFERSIZE);
			start++; i++;
		}
		
		sleep(1);
		printf("\nWaiting for acknowledgement...\n");
		
		int seqnum;
		int flag=0, fail;
		
		while(i>0){
			recv(clientfd, buffer, BUFFERSIZE, 0);
			sscanf(buffer, "ACK_%d", &seqnum);
			if(seqnum==base){
				printf("%s received.\n", buffer);
				memset(buffer, 0, BUFFERSIZE);
			} else if(strcmp(buffer, "error")==0){
				printf("Timeout...\n");
				if(!flag){
					flag=1;
					fail=base;
				}
			}
			base++;
			i--;
		}
		
		if(flag)
			base=fail;
		
		sleep(1);
		printf("\n");
	}
	
	close(clientfd);
	return 0;
}
