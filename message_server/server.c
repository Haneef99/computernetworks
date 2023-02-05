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
int total_client=0;
long pids[100];

int check(long pid)
{
	for(int i=0;i<total_client;i++)
		if(pids[i]==pid)
			return 1;
			
	return 0;
}

int main()
{
	key1=ftok("./Desktop/cn/message_server",102);
	key2=ftok("./Desktop/cn/message_server",101);
	msgqid1=msgget(key1,0666 | IPC_CREAT);
	msgqid2=msgget(key2,0666 | IPC_CREAT);

	while(1)
	{
		struct message_buf message,message_write;
		message.mtype=0;
		if(msgrcv(msgqid1,&message,sizeof(message),message.mtype,0)!=-1)
		{
			
			if(!check(message.mtype)){
				pids[total_client++]= message.mtype;
			}
			
			for(int i=0;i<total_client;i++)
			{
				if(pids[i]==message.mtype)
					continue;

				struct message_buf message_write;
				message_write.mtype=pids[i];
				strcpy(message_write.mtext,message.mtext);
				msgsnd(msgqid2,&message_write,sizeof(message_write),0);
			}
			
			
		}
	}

}
