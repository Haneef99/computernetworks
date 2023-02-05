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


int main(){
	printf("process 4(%ld):\n",getpid());
	
	siga.sa_sigaction = *sigaction_handler;
	siga.sa_flags |= SA_SIGINFO;
	
	if(sigaction(SIGUSR1,&siga,NULL)!=0){
		printf("sigaction error\n");
		return 0;
	}
	
	key_t key = ftok("./Desktop/cn",100);
	
	int msgqid = msgget(key,0666 | IPC_CREAT);
	
	long pid = getpid();
	
	struct message_struct message_snd;
	message_snd.mtype = 4;
	sprintf(message_snd.mtext,"%ld",pid);
	
	msgsnd(msgqid,&message_snd,sizeof(message_snd),0);
	
	sleep(10);
	
	printf("prev process in cycle is : %ld\n",prev);
	
	struct message_struct pid2;
	pid2.mtype = 18;
	sprintf(pid2.mtext,"%ld",pid);
	msgsnd(msgqid,&pid2,sizeof(pid2),0);
	
	while(1){
		
		struct message_struct message_rcv;
		message_rcv.mtype = 1;
		if(msgrcv(msgqid,&message_rcv,sizeof(message_rcv),message_rcv.mtype,0)!=-1){
		
			for(int i=0;i<strlen(message_rcv.mtext);i++) next = next*10 + message_rcv.mtext[i] - '0';
			break;
		}
	}
	
	printf("next process in cycle is : %ld\n",next);

	printf("signalling %ld\n",next);
	kill(next,SIGUSR1);
}
