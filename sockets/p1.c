#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
int main(){
	struct sockaddr_in server_addr,client_addr;
	int fd;
	socklen_t addrlen;
	char buff[100];
	
	fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(fd == -1){
		printf("Socket creation failed\n");
		return -1;
	}
	
	printf("Socket fd : %d\n",fd);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	
	if(inet_pton(AF_INET,"192.168.68.131",&server_addr.sin_addr) < 0){
		printf("INvalid ip\n");
		return -1;
	}
	
	if(bind(fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in)) < 0){
		printf("bind failed\n");
		return -1;
	} 
	
	if(listen(fd,5) < 0){
		printf("Listen failed\n");
		return -1;
	}
	
	int newfd = accept(fd,(struct sockaddr*)&client_addr,&addrlen);
	
	if(newfd < -1){
		printf("accept failed\n");
		return -1;
	}
	
	printf("Accepted connection with fd  : %d\n",newfd);
	
	while(1){
		while(recv(newfd,buff,50,0) > 0){
			printf("Received : %s\n",buff);
			break;
		}
		
		char msg[100];
		printf("Enter msg: \n");
		fgets(msg,100,stdin);
		
		send(newfd,msg,strlen(msg),0);
	}
	close(fd);
	close(newfd);
	
	return 0;
	
}
