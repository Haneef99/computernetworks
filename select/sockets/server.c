#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

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
	
	if(bind(sfd,(struct sockaddr*)&server,sizeof(server)) < 0){
		printf("bindf failed\n");
		return -1;
	}
	
	listen(sfd,10);
	
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	
	int nsfd1 = accept(sfd,(struct sockaddr*)&client,&len);
	if(nsfd1 < 0){
		printf("accept1 failed\n");
		return -1;
	}

	int nsfd2 = accept(sfd,(struct sockaddr*)&client,&len);
	if(nsfd2 < 0){
		printf("accept1 failed\n");
		return -1;
	}
	
	int nsfd3 = accept(sfd,(struct sockaddr*)&client,&len);
	if(nsfd3 < 0){
		printf("accept1 failed\n");
		return -1;
	}
	
	
	char buff[1024];
	
	fd_set readfds;
	int max_fd = nsfd1;
	max_fd = (max_fd > nsfd2)?max_fd:nsfd2;
	max_fd = (max_fd > nsfd3)?max_fd:nsfd3;
	FD_ZERO(&readfds);
	struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;
	while(1){
	
		FD_SET(nsfd1,&readfds);
		FD_SET(nsfd2,&readfds);
		FD_SET(nsfd3,&readfds);
		

		int ret = select(max_fd+1,&readfds,NULL,NULL,&tv);
		//printf("%d\n",ret);
		if(ret < 0){
			printf("select error\n");
			return -1;
		}		
		
		if(ret == 0) break;
		
		if(FD_ISSET(nsfd1,&readfds)){
			if(read(nsfd1,buff,50)>0)
			printf("c1 : %s\n",buff);
		}
		
		if(FD_ISSET(nsfd2,&readfds)){
			if(read(nsfd2,buff,50)>0)
			printf("c2 : %s\n",buff);
		}
		if(FD_ISSET(nsfd3,&readfds)){
			if(read(nsfd3,buff,50)>0)
			printf("c3 : %s\n",buff);
		}
		sleep(1);
	}
	
	
	return 0;
}
