#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

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
	
	if(bind(fd,(struct sockaddr*)& server_addr,sizeof(struct sockaddr_in)) < 0){
		printf("Bind failed\n");
		return -1;
	}
	
	printf("Listening to requests\n");
	
	if(listen(fd,5) < 0){
		printf("Listen failed\n");
		return -1;
	}
	
	
	while(1){
	
		struct sockaddr_in client_addr;
		socklen_t addr_len;
		int newfd = accept(fd,(struct sockaddr*)&client_addr,&addr_len);	
		
		printf("New connection accepted\n");
		
		int  pid = fork();
		
		if(pid == 0){
			const char* msg = "connected to a server child\n";
			
			send(newfd,msg,strlen(msg),0);

			while(1){
			
				char buff[100];
				printf("Enter message : \n");
				fgets(buff,100,stdin);
				
				send(newfd,buff,strlen(buff),0);
				printf("Msg sent\n");
				
				char in[100];
				recv(newfd,in,100,0);
				if(strlen(in) == 1) break;
				printf("Received : %s\n",in);				
								
			}
			
			return 0;
		}
		
		
	}
	
	
	return 0;
	
}
