#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/types.h>

int main(){

	int sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(sfd < 0){
		printf("socket failed\n");
		return -1;
	}
	
	struct sockaddr_in server;
	
	server.sin_family = AF_INET;
	server.sin_port = htons(8001);
	
	if(inet_pton(AF_INET,"192.168.1.6",&server.sin_addr) == -1){
		printf("Invalid ip\n");
		return -1;
	}
	
	socklen_t len = sizeof(server);
	
	if(bind(sfd,(struct sockaddr*)&server,len) == -1){
		printf("bind failed\n");
		return -1;
	}
	
	struct sockaddr_in cli;
	socklen_t clilen = sizeof(cli);
	
	listen(sfd,5);
	
	int nsfd = accept(sfd,(struct sockaddr*)&cli,&clilen);
	
	if(nsfd == -1){
		printf("accept failed\n");
		return -1;
	}
	
	printf("accepted connection\n");
	
	struct sockaddr_in peeraddr;
	int plen = sizeof(peeraddr);
	
	if(getpeername(nsfd,(struct sockaddr*)&peeraddr,&plen) == -1){
		printf("peername failed\n");
		return -1;
	}
	
	printf("peer's ip address is : %s\n",inet_ntoa(peeraddr.sin_addr));
	printf("peer's port is : %d\n",(int) ntohs(peeraddr.sin_port));
	
	return 0;
}
