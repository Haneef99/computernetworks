#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int main(){


	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		printf("socket error\n");
		return -1;
	}
	struct sockaddr_in server;
	

	server.sin_family = AF_INET;
	server.sin_port = htons(8090);
	server.sin_addr.s_addr = INADDR_ANY;

	if(connect(sfd,(struct sockaddr*)&server,sizeof(server))<0){
		printf("couldnt connect\n");
		return -1;
	}
	
	printf("connected successfully\n");

	return 0;	
	
}
