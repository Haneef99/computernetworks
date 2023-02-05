#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<string.h>

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

	sprintf(shared_mem2,"%d",pid);
	
	sleep(3);
	
	write(1,shared_mem1,sizeof(shared_mem1));

	return 0;

}
