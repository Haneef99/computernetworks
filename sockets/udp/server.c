#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(){
	
	struct sockaddr_in server_addr,client_addr;
	
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	
	if(fd < 0){
		printf("socket creation failed\n");
		return -1;
	}
	
	memset(&server_addr,0,sizeof(server_addr));
	memset(&client_addr,0,sizeof(client_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	
	if(bind(fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in)) < 0){
		printf("bind failed\n");
		return -1;
	}
	
	printf("Socket is binded \n");
	
	char buff[1000];
	int len = sizeof(client_addr);
	int n = recvfrom(fd,(char*)buff,1000,MSG_WAITALL, (struct sockaddr*)&client_addr,&len);
	
	buff[sizeof(client_addr)] = '\0';
	
	printf("received : %s\n",buff);
	

}
