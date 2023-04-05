#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int COUNT = 3;

int main(){
	
	int sfd;
	struct sockaddr_in serv_addr;
	if((sfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully");
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9000);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
		perror("\n bind : ");
	else printf("\n bind successful ");
	
	
	while(1){
		char buff[100];
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		
		recvfrom(sfd,buff,100,0,(struct sockaddr*)&client,&len);
		
		printf("%s\n",buff);
		
		--COUNT;
		
		if(!COUNT){
			printf("parking lot is full\n");
			
			int fd = fileno(popen("pidof ./M","r"));
			char msg[50];
			
			int s = read(fd,msg,50);
			
			msg[s] = '\0';
			
			int pid = atoi(msg);
			
			union sigval sv;
			sv.sival_int = 2;

			sigqueue(pid, SIGUSR1, sv);
			break;
		}
		
	}
	
	
	return 0;
}
