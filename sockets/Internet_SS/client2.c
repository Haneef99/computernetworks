#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(){

	struct sockaddr_in server_addr;
	
	int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(fd < 0){
		printf("Socket failed\n");
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	
	if(inet_pton(AF_INET,"192.168.68.131",&server_addr.sin_addr) < 0){
		printf("Invalid ip\n");
		return -1;
	}
	
	if(connect(fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) <0){
		printf("Connect failed\n");
		return -1;
	}
	
	printf("Connected!\n");
	
	char buff[100];
	while(recv(fd,buff,100,0) < 0) ;
	printf("%s\n",buff);
	
	while(1){
		
		char r[100];
		recv(fd,r,100,0);
		printf("received : %s\n",r);
		
		char s[100];
		printf("Enter message : \n");
		fgets(s,100,stdin);
		if(strlen(s) == 1) break;
		send(fd,s,strlen(s),0);
		
	}
	
	return 0;
	
}
