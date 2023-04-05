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

int nusfds[2];
int ABC[3];
char type[50];
int parking = 9000;

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
	/*if(message_buffer[0] != 'F')
	{
		this did not originate from the above function 
		return -1;
	}*/
	type[0] = message_buffer[0];
	if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
	{
		/* we did not provide enough space for the ancillary element array
		*/
		return -1;
	}
	/* iterate ancillary elements */
	for(control_message = CMSG_FIRSTHDR(&socket_message);control_message != NULL;control_message = CMSG_NXTHDR(&socket_message,control_message))
	{
		if( (control_message->cmsg_level == SOL_SOCKET) && (control_message->cmsg_type == SCM_RIGHTS) )
		{
			sent_fd = *((int *) CMSG_DATA(control_message));
			return sent_fd;
		}
	}
	return -1;
}

void* handler(void* args){

	int idx = atoi(args);
	
	printf("%d\n",idx);
	
	while(1){
		int fd = recv_fd(nusfds[idx]);
		
		if(fd == -1) continue;
		
		printf("received a vehicle(fd : %d) of type %c\n",fd,type[0]);
		
		int sfd;
		struct sockaddr_in serv_addr;
		bzero(&serv_addr,sizeof(serv_addr));
		if((sfd = socket(AF_INET , SOCK_DGRAM , 0))==-1)
		perror("\n socket");

		int t = type[0] -'0';

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(parking + t - 1);
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		socklen_t serv_len = sizeof(serv_addr);
		const char* buffer = "vehicle parked successfully";
		
		sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&serv_addr,serv_len);
		
		close(sfd);
		
	}

}

static void hfunc(int sig, siginfo_t *si, void *ucontext)
{
    printf("%d is full",si->si_value);
    
    int fd = fileno(popen("pidof ./G1","r"));
	char msg[50];
	
	int s = read(fd,msg,50);
	
	msg[s] = '\0';
	
	int pid = atoi(msg);
	
	//int x = si->si_value;
	
	union sigval sv = si->si_value;
//	sv.sival_int = x;

	sigqueue(pid, SIGUSR1, sv);
	
	fd = fileno(popen("pidof ./G2","r"));
	char msg1[50];
	
	s = read(fd,msg1,50);
	
	msg1[s] = '\0';
	
	pid = atoi(msg);

	sigqueue(pid, SIGUSR1, sv);
    
}

int main(){
	
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = hfunc;
	sa.sa_flags = SA_SIGINFO; /* Important. */

	sigaction(SIGUSR1, &sa, NULL);
	
	int usfd;
	struct sockaddr_un userv_addr;
	int userv_len;
	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	if(usfd < 0){
		printf("socket creation error\n");
		return -1;
	}
	bzero(&userv_addr,sizeof(userv_addr));
	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);
	
	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
		perror("server: bind");
		
	listen(usfd, 5);
	
	for(int i=0;i<2;i++){
		struct sockaddr_un client;
		socklen_t len = sizeof(client);
		
		int nusfd = accept(usfd,(struct sockaddr*)&client,&len);
		if(nusfd < 0){
			printf("accept failed\n");
			return -1;
		}
		printf("Gate %d connected\n",i+1);
		nusfds[i] = nusfd;
	}
	
	
	pthread_t thr1,thr2;
	
	char temp[10];
	sprintf(temp,"%d",0);
	pthread_create(&thr1,NULL,handler,(void*)temp);
	
	char temp1[10];
	sprintf(temp1,"%d",1);
	pthread_create(&thr2,NULL,handler,(void*)temp1);
	
	pthread_join(thr1,NULL);
	pthread_join(thr2,NULL);
		
	/* ------------ using select -------------*/
	/*fd_set rfds;
	FD_ZERO(&rfds);
	
	
	while(1){
		int max_fd = nusfds[0];
		if(nusfds[1] > max_fd) max_fd = nusfds[1];
		
		FD_SET(nusfds[0],&rfds);
		FD_SET(nusfds[1],&rfds);
		
		int ret = select(max_fd+1,&rfds,NULL,NULL,NULL);
		
		if(ret < 0){
			printf("select error\n");
			return -1;
		}
		
		if(FD_ISSET(nusfds[0],&rfds)){
		
			int fd = recv_fd(nusfds[0]);
			if(fd == -1) continue;
			printf("received a vehicle(%d) of type (%c)",fd,type);
		}
		
		if(FD_ISSET(nusfds[1],&rfds)){
		
			int fd = recv_fd(nusfds[1]);
			if(fd == -1) continue;
			printf("received a vehicle(%d) of type (%c)",fd,type);
		}
		
		
	}*/
	
	return 0;
	
}
