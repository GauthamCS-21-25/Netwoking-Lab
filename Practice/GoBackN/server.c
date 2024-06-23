#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

int main() {
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
	
	if((bind(serverfd, (struct sockaddr *)&address, sizeof(address)))<0){
		perror("bind");
      	exit(EXIT_FAILURE);
	}
	
	if((listen(serverfd, 3))<0){
		perror("listen");
      	exit(EXIT_FAILURE);
	}
	
	if((newsocket=accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&addrlen))<0){
		perror("accept");
      	exit(EXIT_FAILURE);
	}
	
	printf("Server is running...\nConnected to client...\n");
	
	int i, start, base=0, frames=10;
	
	while(base < frames){
		start=base; i=0;
		printf("\nWaiting for frames...\n");
		while(i<4 && start<frames){
			recv(newsocket, buffer, BUFFERSIZE, 0);
			printf("%s received.\n", buffer);
			memset(buffer, 0, BUFFERSIZE);
			start++; i++;
		}
		
		sleep(1);
		printf("\nSending acknowledgement...\n");
		
		int index=start-i;
		int flag=0, fail;
		
		while(i>0){
			if(rand()%3 != 0){
				memset(buffer, 0, BUFFERSIZE);
				sprintf(buffer, "ACK_%d", index);
				send(newsocket, buffer, sizeof(buffer), 0);
				printf("%s sent.\n", buffer);
			} else {
				memset(buffer, 0, BUFFERSIZE);
				strcpy(buffer, "error");
				send(newsocket, buffer, sizeof(buffer), 0);
				printf("Simulating timeout...\n");
				
				if(!flag){
					flag=1;
					fail=index;
				}
			}
			index++;
			base++;
			i--;
		}
		
		if(flag)
			base=fail;
		
		sleep(1);
		printf("\n");
	}
	
	close(newsocket);
	return 0;
}

