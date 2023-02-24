#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(){

	struct sockaddr_in server_addr;
	
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	
	if(fd < 0){
		printf("socket failed\n");
		return -1;
	}
	
	memset(&server_addr,0,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	/*if(bind(fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in)) < 0){
		printf("bind failed\n");
		return -1;
	}*/
	
	const char* msg = "Hey there!";
	sendto(fd,(const char*) msg,strlen(msg),MSG_CONFIRM,(const struct sockaddr*) &server_addr,sizeof(server_addr));
	
	printf("msg sent\n");

	return 0;
}
