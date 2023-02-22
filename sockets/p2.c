#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
int main(){
	struct sockaddr_in server_addr;
	
	int fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(fd < 0){
		printf("Socket creation failed\n");
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr) < 0){
		printf("INvalid ip\n");
		return -1;
	}
	
	if(connect(fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in)) < 0){
		printf("Connect failed\n");
		return -1;
	}
	
	printf("Socket is connected\n");
	
	sleep(5);
	
	const char* msg = "Tesing sockets on local host.........";
	
	send(fd,msg,strlen(msg),0);
	
	printf("msg sent!\n");
	
	close(fd);
	
	return 0;
}
