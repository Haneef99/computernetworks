#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>

int main(){

	char buff[100];
	
	char* shared_mem1;
	char* shared_mem2;
	
	
	key_t key1 = ftok("p1r",101);
	key_t key2 = ftok("p2r",102);
	
	int shmid1 = shmget(key1,1024,0666 | IPC_CREAT);
	int shmid2 = shmget(key2,1024,0666 | IPC_CREAT);
	
	shared_mem1 = (char*) shmat(shmid1,(void*) 0, 0);
	shared_mem2 = (char*) shmat(shmid2,(void*) 0, 0);
	
	int pid = getpid();
	
	printf("%d\n",pid);
	
	sprintf(shared_mem1,"%d",pid);
	
	sleep(3);
	
	write(1,shared_mem2,sizeof(shared_mem2));
	
	
//	printf("%s",)
	
	
	return 0;

}
