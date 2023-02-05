#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<string.h>

void p2(){

	sem_t* s1 = sem_open("s1",O_EXCL);
	sem_t* s2 = sem_open("s2",O_EXCL);
	
	key_t key1 = ftok("x",100);
	int shmid1 = shmget(key1,1024,0666 | IPC_CREAT);
	int* x = (int*) shmat(shmid1,(void*)0,0);
	
	key_t key2 = ftok("y",101);
	int shmid2 = shmget(key2,1024,0666 | IPC_CREAT);
	int* y = (int*) shmat(shmid2,(void*)0,0);
	
	while(1){

		printf("P2 : Im waiting for S1\n");
		sem_wait(s1);
		
		printf("P2 : %d\n",x[0]);
		y[0] = x[0] + 1;
		
		printf("Enter any char to signal s2\n");
		char ch;
		scanf("%c",&ch);
		
		sem_post(s2);

	}

}

int main(){

	p2();	
	
	return 0;	
}

