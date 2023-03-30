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

#define ADDRESS "socket"

int send_fd(int socket, int fd_to_send)
{
	struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr *control_message = NULL;
	char message_buffer[1];
	/* storage space needed for an ancillary element with a paylod of
	length is CMSG_SPACE(sizeof(length)) */
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];int available_ancillary_element_buffer_space;
	/* at least one vector of one byte must be sent */
	message_buffer[0] = 'F';
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;
	/* initialize socket message */
	memset(&socket_message, 0, sizeof(struct msghdr));
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;
	/* provide space for the ancillary data */
	available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
	memset(ancillary_element_buffer, 0,
	available_ancillary_element_buffer_space);
	socket_message.msg_control = ancillary_element_buffer;
	socket_message.msg_controllen =
	available_ancillary_element_buffer_space;
	/* initialize a single ancillary data element for fd passing */
	control_message = CMSG_FIRSTHDR(&socket_message);
	control_message->cmsg_level = SOL_SOCKET;
	control_message->cmsg_type = SCM_RIGHTS;
	control_message->cmsg_len = CMSG_LEN(sizeof(int));
	*((int *) CMSG_DATA(control_message)) = fd_to_send;
	return sendmsg(socket, &socket_message, 0);
}

int available[3];
int pids[3];
int nusfds[3];

void sigaction_handler(int sig,siginfo_t *siginfo,void* context){
	
	for(int i=0;i<3;i++){
		if(pids[i] == siginfo->si_pid){
			available[i] = (available[i] == 1)?0:1;
		}
	}
	
}


int main(){

	struct sigaction siga;
	siga.sa_sigaction = *sigaction_handler;
	siga.sa_flags |= SA_SIGINFO;
	
	if(sigaction(SIGUSR1,&siga,NULL)!=0){
		printf("sigaction error\n");
		return 0;
	}	
	
	int pid = getpid();
	
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	
	if(usfd < 0){
		printf("socket failed\n");
		return -1;
	}
	
	struct sockaddr_un userver;
	bzero(&userver,sizeof(userver));
	userver.sun_family = AF_UNIX;
	strcpy(userver.sun_path,ADDRESS);
	
	unlink(ADDRESS);
	
	if(bind(usfd,(struct sockaddr*)&userver,sizeof(userver)) < 0){
		printf("bind error\n");
		return -1;
	}
	
	listen(usfd,5);
	
	for(int i=0;i<3;i++){
		struct sockaddr_un client;
		socklen_t len = sizeof(client);
		int nusfd = accept(usfd,(struct sockaddr*)&client,&len);
		
		if(nusfd < 0){
			printf("accept failed\n");
			return -1;
		}
		available[i] = 1;
		
		char msg[50];
		sprintf(msg,"%d",pid);
		send(nusfd,msg,50,0);
		
		char buff[50];
		recv(nusfd,buff,50,0);
		pids[i] = atoi(buff);
		
		printf("Platform connected(%d)\n",pids[i]);
		nusfds[i] = nusfd;
	}
	
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
	
	if(bind(sfd,(struct sockaddr*)&server,sizeof(server)) < 0){
		printf("bind failed\n");
		return -1;
	}
	
	listen(sfd,5);
	
	while(1){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int nsfd = accept(sfd,(struct sockaddr*)&client,&len);
		
		if(nsfd < 0){
			printf("accept failed\n");
			return -1;
		}
		
		//sending announcements to all platforms
		for(int i=0;i<3;i++){
			kill(pids[i],SIGUSR1);
		}
		
		for(int i=0;i<3;i++){
			if(available[i]){
				send_fd(nusfds[i],nsfd);
				break;
			}
		}
		
		close(nsfd);
		sleep(2);
	}
	
	return 0;
	
}
