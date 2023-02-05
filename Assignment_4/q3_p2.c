#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

int main(){
	
	sem_t* s12 = sem_open("s12",O_CREAT,0666,0);
	sem_t* s23 = sem_open("s23",O_CREAT,0666,0);
	
	while(1){
	
		printf("P2 : waiting for s12\n");;
		
		sem_wait(s12);
		
		printf("P2 : s12 signalled by p1\n");
		
		printf("P2 : Enter any character to signal s23\n");
		char ch;
		scanf("%c",&ch);
		
		printf("P2 : signalling s23\n");
		
		sem_post(s23);
	
	}
	
	sem_close(s12);
	sem_close(s23);
	
	sem_unlink("s12");
	sem_unlink("s23");

	return 0;

}
