#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

int main(){
	
	sem_t* s34 = sem_open("s34",O_CREAT,0666,0);
	sem_t* s41 = sem_open("s41",O_CREAT,0666,0);
	
	while(1){
	
		printf("P4 : waiting for s34\n");;
		
		sem_wait(s34);
		
		printf("P4 : s34 signalled by p1\n");
		
		printf("P4 : Enter any character to signal s41\n");
		char ch;
		scanf("%c",&ch);
		
		printf("P4 : signalling s41\n");
		
		sem_post(s41);
	
	}
	
	sem_close(s34);
	sem_close(s41);
	
	sem_unlink("s34");
	sem_unlink("s41");

	return 0;

}
