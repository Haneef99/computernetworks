#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

int main(){
	
	sem_t* s12 = sem_open("s12",O_CREAT,0666,0);
	sem_t* s41 = sem_open("s41",O_CREAT,0666,0);
	
	while(1){
	
		printf("P1 : Enter any character to signal s12\n");;
		char ch;
		scanf("%c",&ch);
		
		printf("P1 : Signalling  s12\n");
		
		sem_post(s12);
		
		printf("P1 : waiting for s41\n");
		
		sem_wait(s41);
		
		printf("P1 : s41 signalled by p4\n");
		
	
	}
	
	sem_close(s12);
	sem_close(s41);
	
	sem_unlink("s12");
	sem_unlink("s41");

	return 0;

}
