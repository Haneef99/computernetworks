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

int recv_fd(int socket)
{
	int sent_fd, available_ancillary_element_buffer_space;
	struct msghdr socket_message;
	struct iovec io_vector[1];
	struct cmsghdr *control_message = NULL;
	char message_buffer[1];
	char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
	/* start clean */
	memset(&socket_message, 0, sizeof(struct msghdr));
	memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));
	/* setup a place to fill in message contents */
	io_vector[0].iov_base = message_buffer;
	io_vector[0].iov_len = 1;
	socket_message.msg_iov = io_vector;
	socket_message.msg_iovlen = 1;
	/* provide space for the ancillary data */
	socket_message.msg_control = ancillary_element_buffer;
	socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
	if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
		return -1;
	if(message_buffer[0] != 'F')
	{
		/* this did not originate from the above function */
		return -1;
	}
	if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
	{
		/* we did not provide enough space for the ancillary element array
		*/
		return -1;
	}
	/* iterate ancillary elements */
	for(control_message = CMSG_FIRSTHDR(&socket_message);control_message != NULL;control_message = CMSG_NXTHDR(&socket_message,	control_message))
	{
		if( (control_message->cmsg_level == SOL_SOCKET) &&(control_message->cmsg_type == SCM_RIGHTS) )
		{
			sent_fd = *((int *) CMSG_DATA(control_message));
			return sent_fd;
		}
	}
	return -1;
}

#define ADDRESS "socket"

void handler(){
	printf("Train is ready to arrive\n");
	return ;
}

int main(){

	signal(SIGUSR1,handler);

	int usfd;
	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd < 0){
		printf("error creating socket\n");
		return -1;
	}
	
	struct sockaddr_un userver;
	bzero(&userver,sizeof(userver));
	userver.sun_family = AF_UNIX;
	strcpy(userver.sun_path,ADDRESS);
	
	if(connect(usfd,(struct sockaddr*)&userver,sizeof(userver)) == -1){
		printf("couldnt connect\n");
		return -1;
	}
	
	char buff[50];
	recv(usfd,buff,50,0);
	
	int sm_pid = atoi(buff);
	printf("connected to station with pid %d\n",sm_pid);
	
	int pid = getpid();
	char msg[50];
	sprintf(msg,"%d",pid);
	send(usfd,msg,50,0);
	
	while(1){
		int fd = recv_fd(usfd);
		if(fd == -1) continue ;
		kill(sm_pid,SIGUSR1);
		printf("Platform 1 is occupied\n");
		
		sleep(5);
		
		kill(sm_pid,SIGUSR1);
		printf("Platform 1 is free\n");
	}
	
	return 0;
}
