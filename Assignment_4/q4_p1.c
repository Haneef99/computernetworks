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
#include<unistd.h>

void parent(){

	sem_t* s1 = sem_open("s1",O_EXCL);
	sem_t* s2 = sem_open("s2",O_EXCL);
	
	key_t key1 = ftok("x",100);
	int shmid1 = shmget(key1,1024,0666 | IPC_CREAT);
	int* x = (int*) shmat(shmid1,(void*)0,0);
	
	key_t key2 = ftok("y",101);
	int shmid2 = shmget(key2,1024,0666 | IPC_CREAT);
	int* y = (int*) shmat(shmid2,(void*)0,0);
	
	const char* X = "1";
	const char* Y = "1";
	
	x[0] = 1;
	y[0] = 1;
	
	while(1){
	
		printf("P1 : Im reading shm y\n");
		printf("P1 : %d\n",y[0]);
		
		
		x[0] = y[0] + 1;
		
		printf("Enter any char to signal s1\n");
		char ch;
		scanf("%c",&ch);
		
		sem_post(s1);
		
		printf("P1 : Im waiting for S2\n");
		sem_wait(s2);
	
	}
	

}


int main(){

	sem_t* mutex1 = sem_open("s1",O_CREAT | O_EXCL, 0666, 0);
	sem_t* mutex2 = sem_open("s2",O_CREAT | O_EXCL, 0666, 0);
	
	int c = fork();
	
	if(c > 0){
		parent();
	}
	else{
		char* args[] = {"./q4_p2",NULL};
		execvp(args[0],args);
	}
	
	sem_unlink("s1");
	sem_unlink("s2");
	
	return 0;

}
