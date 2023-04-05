#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

int main(){
	
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	
	if(sfd < 0){
		printf("socket creation failed\n");
		return -1;
	}	
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	
	if(inet_pton(AF_INET,"192.168.68.114",&server.sin_addr) < 0){
		printf("INvalid ip\n");
		return -1;
	}
	
	if(connect(sfd,(struct sockaddr*)&server,sizeof(server))< 0){
		printf("connect failed\n");
		return -1;
	}
	
	const char* msg = "this is client one";
	send(sfd,msg,strlen(msg)+1,0);

	return 0;
}
