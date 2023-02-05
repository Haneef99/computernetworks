#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

struct message_buf{
	long mtype;
	char mtext[100];
};

key_t key1,key2;
int msgqid1,msgqid2;
int pid;


int main()
{
	key1=ftok("./Desktop/cn/message_server",102);
	msgqid1 =msgget(key1,0666 | IPC_CREAT);
	key2=ftok("./Desktop/cn/message_server",101);
	msgqid2=msgget(key2,0666 | IPC_CREAT);
	pid=getpid();


	while(1)
	{
	
		printf("C1(%d):",pid);
		struct message_buf message;
		fgets(message.mtext,100,stdin);
		message.mtype=pid;
		msgsnd(msgqid1,&message,sizeof(message),0);	
		
		struct message_buf message_read;
		msgrcv(msgqid2,&message_read,sizeof(message_read),pid,0);
		printf("received %s\n",message_read.mtext);
	}

}
