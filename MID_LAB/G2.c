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

#define ADDRESS "myparking"


int sfds[3];
	fd_set rfds;
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

static void hfunc(int sig, siginfo_t *si, void *ucontext)
{
    int val = si->si_value.sival_int;
    
    FD_CLR(sfds[val-1],&rfds);
    close(sfds[val-1]);
    printf("sfd %d closed\n",val);
    
}

int main(){

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = hfunc;
	sa.sa_flags = SA_SIGINFO; /* Important. */

	sigaction(SIGUSR1, &sa, NULL);
	
	int usfd;
	struct sockaddr_un userv_addr;
	int userv_len,ucli_len;
	usfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(usfd==-1)
		perror("\nsocket");
	bzero(&userv_addr,sizeof(userv_addr));
	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	userv_len = sizeof(userv_addr);
	if(connect(usfd,(struct sockaddr *)&userv_addr,userv_len)==-1)
	perror("\n connect ");
	else printf("Connected to parking M\n");
	
	int port = 8095;
	
	for(int i=0;i<3;i++){
		int sfd = socket(AF_INET,SOCK_STREAM,0);
		
		if(sfd < 0){
			printf("socket error\n");
			return -1;
		}
		
		
		
		struct sockaddr_in server;
		
		server.sin_family = AF_INET;
		server.sin_port = htons(port + i);
		server.sin_addr.s_addr = INADDR_ANY;
		
		if(bind(sfd,(struct sockaddr*)&server,sizeof(server)) < 0){
			printf("bind error\n");
			return -1;
		}
		
		listen(sfd,5);
		printf("socket opened on port %d\n",port+i);
	
		sfds[i] = sfd;
	}
	

	FD_ZERO(&rfds);
	
	while(1){
		
		int max_fd = sfds[0];
	
		for(int i=0;i<3;i++){
			FD_SET(sfds[i],&rfds);
			if(sfds[i] > max_fd) max_fd = sfds[i];
		}
		
		int ret = select(max_fd+1,&rfds,NULL,NULL,NULL);
		
		if(ret < 0){
			printf("select error\n");
			return -1;
		}
		
		for(int i=0;i<3;i++){
			if(FD_ISSET(sfds[i],&rfds)){
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int nsfd = accept(sfds[i],(struct sockaddr*)&client,&len);
				
				if(nsfd < 0){
					printf("accept error\n");\
					return -1;
				}
				
				printf("vehicle of type %d accepted\n",i+1);
			}
		}
	}

	return 0;
}
