#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<signal.h>

int main(){
	
	int sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sfd < 0){
		printf("sokcet creation failed\n");
		return -1;
	}
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	if(inet_pton(AF_INET,"192.168.68.114",&server.sin_addr) < 0){
		printf("Invalid ip\n");
		return -1;
	}
	
	if(connect(sfd,(struct sockaddr*)&server,sizeof(server)) < 0){
		printf("couldnt connect\n");
		return -1;
	}
	
	printf("connected\n");
	
	return 0;
}
