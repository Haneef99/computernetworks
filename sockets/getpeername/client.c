#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>


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
		printf("Invalid address\n");
		return -1;
	}
	socklen_t len = sizeof(server);
	if(connect(sfd,(struct sockaddr*)&server,len) < 0){
		printf("connect failed\n");
		return -1;
	}
	
	printf("connected\n");
	
	return 0;
}
