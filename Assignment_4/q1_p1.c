#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>

struct message_struct{

	long mtype;
	char mtext[100];

};


struct sigaction siga;
long prev = 0;
long next = 0;

void sigaction_handler(int sig,siginfo_t *siginfo,void* context){

	long sender_pid  = siginfo->si_pid;
	
	prev = sender_pid;
	
	return ;

}

int count = 0;

void usr1_handler(){
	
	++count;
	if(count > 3) return ;
	
	sleep(2);
	printf("clockwise signalling in process 1\n");
	kill(next,SIGUSR1);
	
}

void usr2_handler(){

	++count;
	if(count > 3) return ;
	
	sleep(2);
	printf("anti clockwise signalling in process 1\n");
	kill(prev,SIGUSR2);
}

int main(){
	signal(SIGUSR2,usr2_handler);
	printf("Process 1(%ld):\n",getpid());

	siga.sa_sigaction = *sigaction_handler;
	siga.sa_flags |= SA_SIGINFO;
	
	if(sigaction(SIGUSR1,&siga,NULL)!=0){
		printf("sigaction error\n");
		return 0;
	}

	key_t key = ftok("./Desktop/cn/Assignment4",100);
	
	int msgqid = msgget(key,0666 | IPC_CREAT);
	
	
	while(1){
		
		struct message_struct message_rcv;
		message_rcv.mtype = 2;
		if(msgrcv(msgqid,&message_rcv,sizeof(message_rcv),message_rcv.mtype,0)!=-1){
		
			for(int i=0;i<strlen(message_rcv.mtext);i++) next = next*10 + message_rcv.mtext[i] - '0';
			break;
		}
	}

	printf("next process in cycle is : %ld\n",next);
	printf("signalling %ld process\n", next);
	kill(next,SIGUSR1);

	sleep(10);

	printf("prev process in cycle is : %ld\n",prev);

	signal(SIGUSR1,usr1_handler);
	
	raise(SIGUSR1);
	
	while(count < 3) ;

	count = 0;
	
	
	raise(SIGUSR2);
	
	while(count < 3) ;

	return 0;

}
